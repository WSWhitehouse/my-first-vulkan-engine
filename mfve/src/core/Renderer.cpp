#include "core/Renderer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

// stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace MFVE
{
  using namespace MFVE::Vulkan;

  void Renderer::CreateRenderer(const AppProperties& _appProperties, Window* _window,
                                const VkAllocationCallbacks* _allocator)
  {
    m_window = _window;
    if (!m_window->CreateWindow(_appProperties.name))
    {
      MFVE_LOG_FATAL("Failed to create Window!");
    }

    // Window Extensions
    m_extensions = m_window->GetRequiredWindowExtensions();

    // Validation Layers Extensions
    if (ValidationLayers::Enabled())
    {
      m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (!Extensions::CheckExtensionSupport(m_extensions))
    {
      MFVE_LOG_FATAL("Extensions unsupported!");
    }

    if (ValidationLayers::Enabled() && !ValidationLayers::CheckLayerSupport())
    {
      MFVE_LOG_FATAL("Validation Layers requested, but not available!");
    }

    m_instance.CreateInstance(_appProperties, m_extensions, _allocator);
    VkCheck(ValidationLayers::CreateDebugMessenger(m_instance, m_debugMessenger, _allocator));
    VkCheck(m_window->CreateSurface(m_instance, _allocator));
    m_device.CreateDevice(m_instance, m_window, _allocator);

    m_swapchain.CreateSwapchain(m_device, _window, _allocator);
    m_renderPass.CreateRenderPass(m_device, m_swapchain, m_depthBuffer, _allocator);

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding            = 0;
    uboLayoutBinding.descriptorCount    = 1;
    uboLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding            = 1;
    samplerLayoutBinding.descriptorCount    = 1;
    samplerLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

    std::vector<VkDescriptorSetLayoutBinding> layoutBindings{ uboLayoutBinding,
                                                              samplerLayoutBinding };

    m_descriptor.CreateDescriptorSetLayout(m_device, layoutBindings, _allocator);

    VkCheck(
      m_pipeline.CreatePipeline(m_device, m_swapchain, m_renderPass, { m_descriptor }, _allocator));

    VkCheck(
      m_graphicsCommandPool.CreateCommandPool(m_device, m_device.GetGraphicsQueue(), _allocator));
    VkCheck(
      m_transferCommandPool.CreateCommandPool(m_device, m_device.GetTransferQueue(), _allocator));

    m_depthBuffer.CreateDepthBuffer(m_device, m_swapchain, m_graphicsCommandPool, _allocator);
    VkCheck(m_framebuffer.CreateFramebuffers(
      m_device, m_swapchain, m_renderPass, m_depthBuffer, _allocator));

    CreateTestTexture();

    CreateVertexBuffer(_allocator);
    CreateIndexBuffer(_allocator);

    CreateUniformBuffers(_allocator);
    CreateDescriptorPool(_allocator);
    CreateDescriptorSets(_allocator);

    SetUpGraphicsCommandBuffer();
    CreateSyncObjects(_allocator);
  }

  void Renderer::DestroyRenderer(const VkAllocationCallbacks* _allocator)
  {
    CleanUpSwapchain(_allocator);

    m_testTexture.DestroyTexture(m_device, _allocator);

    m_descriptor.DestroyDescriptorSetLayout(m_device, _allocator);
    m_vertexBuffer.DestroyBuffer(m_device, _allocator);
    m_indexBuffer.DestroyBuffer(m_device, _allocator);
    DestroySyncObjects(_allocator);
    m_graphicsCommandPool.DestroyCommandPool(m_device, _allocator);
    m_transferCommandPool.DestroyCommandPool(m_device, _allocator);
    m_device.DestroyDevice(_allocator);
    ValidationLayers::DestroyDebugMessenger(m_instance, m_debugMessenger, _allocator);
    m_window->DestroySurface(m_instance, _allocator);
    m_instance.DestroyInstance(_allocator);

    // Window
    m_window->DestroyWindow();
    delete m_window;
  }

  void Renderer::RecreateSwapchain(const VkAllocationCallbacks* _allocator)
  {
    m_window->WaitWhileMinimised();

    DeviceWaitIdle();

    CleanUpSwapchain(_allocator);

    m_swapchain.CreateSwapchain(m_device, m_window, _allocator);
    m_renderPass.CreateRenderPass(m_device, m_swapchain, m_depthBuffer, _allocator);
    VkCheck(
      m_pipeline.CreatePipeline(m_device, m_swapchain, m_renderPass, { m_descriptor }, _allocator));
    m_depthBuffer.CreateDepthBuffer(m_device, m_swapchain, m_graphicsCommandPool, _allocator);
    VkCheck(m_framebuffer.CreateFramebuffers(
      m_device, m_swapchain, m_renderPass, m_depthBuffer, _allocator));

    CreateUniformBuffers(_allocator);
    CreateDescriptorPool(_allocator);
    CreateDescriptorSets(_allocator);

    SetUpGraphicsCommandBuffer();

    m_imagesInFlight.resize(m_swapchain.GetImages().size(), VK_NULL_HANDLE);
  }

  void Renderer::CleanUpSwapchain(const VkAllocationCallbacks* _allocator)
  {
    m_depthBuffer.DestroyDepthBuffer(m_device, _allocator);
    m_framebuffer.DestroyFramebuffers(m_device, _allocator);
    m_graphicsCommandBuffer.FreeCommandBuffers(m_device, m_graphicsCommandPool);
    m_pipeline.DestroyPipeline(m_device, _allocator);
    m_renderPass.DestroyRenderPass(m_device, _allocator);
    m_swapchain.DestroySwapchain(m_device, _allocator);

    for (auto& buffer : m_uniformBuffers)
    {
      buffer.DestroyBuffer(m_device, _allocator);
    }

    m_descriptor.DestroyDescriptorPool(m_device, _allocator);
  }

  void Renderer::SetUpGraphicsCommandBuffer()
  {
    m_graphicsCommandBuffer.AllocateCommandBuffers(
      m_device,
      m_graphicsCommandPool,
      static_cast<uint32_t>(m_framebuffer.GetFramebuffers().size()));

    const auto& commandBuffers = m_graphicsCommandBuffer.GetCommandBuffers();

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags            = 0;
      beginInfo.pInheritanceInfo = nullptr;

      VkCheck(vkBeginCommandBuffer(commandBuffers[i], &beginInfo));

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass        = m_renderPass.GetRenderPass();
      renderPassInfo.framebuffer       = m_framebuffer.GetFramebuffers()[i];
      renderPassInfo.renderArea.offset = { 0, 0 };
      renderPassInfo.renderArea.extent = m_swapchain.GetExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color        = { { 0.0f, 0.0f, 0.0f, 1.0f } };
      clearValues[1].depthStencil = { 1.0f, 0 };

      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues    = clearValues.data();

      vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      vkCmdBindPipeline(
        commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipeline());

      // Bind Vertex Buffer
      VkBuffer vertexBuffers[] = { m_vertexBuffer.GetBuffer() };
      VkDeviceSize offsets[]   = { 0 };
      vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

      // Bind Index Buffer
      vkCmdBindIndexBuffer(commandBuffers[i], m_indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

      // Bind Descriptor Sets
      vkCmdBindDescriptorSets(commandBuffers[i],
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_pipeline.GetPipelineLayout(),
                              0,
                              1,
                              &m_descriptor.GetDescriptorSets()[i],
                              0,
                              nullptr);

      // Draw
      vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

      vkCmdEndRenderPass(commandBuffers[i]);

      VkCheck(vkEndCommandBuffer(commandBuffers[i]));
    }
  }

  void Renderer::CreateVertexBuffer(const VkAllocationCallbacks* _allocator)
  {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    Buffer stagingBuffer;
    stagingBuffer.CreateBuffer(m_device,
                               bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               _allocator);

    stagingBuffer.FillBuffer(m_device, vertices.data(), bufferSize, 0, 0);

    m_vertexBuffer.CreateBuffer(m_device,
                                bufferSize,
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                _allocator);

    m_vertexBuffer.CopyFromBuffer(m_device, m_transferCommandPool, stagingBuffer, bufferSize);

    stagingBuffer.DestroyBuffer(m_device, _allocator);
  }

  void Renderer::CreateIndexBuffer(const VkAllocationCallbacks* _allocator)
  {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    Buffer stagingBuffer;
    stagingBuffer.CreateBuffer(m_device,
                               bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               _allocator);

    stagingBuffer.FillBuffer(m_device, indices.data(), bufferSize, 0, 0);

    m_indexBuffer.CreateBuffer(m_device,
                               bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               _allocator);

    m_indexBuffer.CopyFromBuffer(m_device, m_transferCommandPool, stagingBuffer, bufferSize);

    stagingBuffer.DestroyBuffer(m_device, _allocator);
  }

  void Renderer::CreateUniformBuffers(const VkAllocationCallbacks* _allocator)
  {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_uniformBuffers.resize(m_swapchain.GetImages().size());

    for (auto& buffer : m_uniformBuffers)
    {
      buffer.CreateBuffer(m_device,
                          bufferSize,
                          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          _allocator);
    }
  }

  void Renderer::CreateDescriptorPool(const VkAllocationCallbacks* _allocator)
  {
    const auto descriptorCount = static_cast<uint32_t>(m_swapchain.GetImages().size());

    VkDescriptorPoolSize uboPoolSize{};
    uboPoolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboPoolSize.descriptorCount = descriptorCount;

    VkDescriptorPoolSize samplerPoolSize{};
    samplerPoolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerPoolSize.descriptorCount = descriptorCount;

    std::vector<VkDescriptorPoolSize> poolSizes{ uboPoolSize, samplerPoolSize };

    m_descriptor.CreateDescriptorPool(
      m_device, poolSizes, static_cast<uint32_t>(m_swapchain.GetImages().size()), _allocator);
  }

  void Renderer::CreateDescriptorSets(const VkAllocationCallbacks* _allocator)
  {
    m_descriptor.AllocateDescriptorSets(
      m_device, static_cast<uint32_t>(m_swapchain.GetImages().size()), _allocator);

    const auto imageInfo = m_testTexture.GetDescriptorImageInfo();

    for (size_t i = 0; i < m_swapchain.GetImages().size(); i++)
    {
      VkDescriptorBufferInfo uboBufferInfo{};
      uboBufferInfo.buffer = m_uniformBuffers[i].GetBuffer();
      uboBufferInfo.offset = 0;
      uboBufferInfo.range  = sizeof(UniformBufferObject);

      VkWriteDescriptorSet uboDescriptorWrite{};
      uboDescriptorWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      uboDescriptorWrite.dstSet           = m_descriptor.GetDescriptorSets()[i];
      uboDescriptorWrite.dstBinding       = 0;
      uboDescriptorWrite.dstArrayElement  = 0;
      uboDescriptorWrite.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      uboDescriptorWrite.descriptorCount  = 1;
      uboDescriptorWrite.pBufferInfo      = &uboBufferInfo;
      uboDescriptorWrite.pImageInfo       = nullptr;
      uboDescriptorWrite.pTexelBufferView = nullptr;

      VkWriteDescriptorSet textureDescriptorWrite{};
      textureDescriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      textureDescriptorWrite.dstSet          = m_descriptor.GetDescriptorSets()[i];
      textureDescriptorWrite.dstBinding      = 1;
      textureDescriptorWrite.dstArrayElement = 0;
      textureDescriptorWrite.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      textureDescriptorWrite.descriptorCount = 1;
      textureDescriptorWrite.pImageInfo      = &imageInfo;

      m_descriptor.UpdateDescriptorSets(m_device, { uboDescriptorWrite, textureDescriptorWrite });
    }
  }

  void Renderer::UpdateUniformBuffer(uint32_t _currentImage)
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time =
      std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model =
      glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(
      glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj =
      glm::perspective(glm::radians(45.0f),
                       m_swapchain.GetExtent().width / (float)m_swapchain.GetExtent().height,
                       0.1f,
                       10.0f);

    // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is
    // inverted. The easiest way to compensate for that is to flip the sign on the scaling factor of
    // the Y axis in the projection matrix. If you don't do this, then the image will be rendered
    // upside down.
    ubo.proj[1][1] *= -1;

    auto& buffer = m_uniformBuffers[_currentImage];

    void* data;
    vkMapMemory(m_device.GetDevice(), buffer.GetBufferMemory(), 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(m_device.GetDevice(), buffer.GetBufferMemory());
  }

  void Renderer::DrawFrame(const VkAllocationCallbacks* _allocator)
  {
    vkWaitForFences(
      m_device.GetDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    // Acquire image from swap chain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_device.GetDevice(),
                                            m_swapchain.GetSwapchain(),
                                            UINT64_MAX,
                                            m_imageAvailableSemaphore[m_currentFrame],
                                            VK_NULL_HANDLE,
                                            &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
      RecreateSwapchain(_allocator);
      return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
      VkCheck(result);
    }

    UpdateUniformBuffer(imageIndex);

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
      vkWaitForFences(m_device.GetDevice(), 1, &m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }

    // Mark the image as now being in use by this frame
    m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];

    // Submitting the command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[]      = { m_imageAvailableSemaphore[m_currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores    = waitSemaphores;
    submitInfo.pWaitDstStageMask  = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &m_graphicsCommandBuffer.GetCommandBuffers()[imageIndex];

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore[m_currentFrame] };

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    vkResetFences(m_device.GetDevice(), 1, &m_inFlightFences[m_currentFrame]);

    VkCheck(vkQueueSubmit(
      m_device.GetGraphicsQueue().queue, 1, &submitInfo, m_inFlightFences[m_currentFrame]));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapchain.GetSwapchain() };

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains    = swapChains;
    presentInfo.pImageIndices  = &imageIndex;
    presentInfo.pResults       = nullptr;

    result = vkQueuePresentKHR(m_device.GetPresentQueue().queue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        m_window->HasWindowResized())
    {
      m_window->ResetWindowResized();
      RecreateSwapchain(_allocator);
    }
    else
    {
      VkCheck(result);
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  void Renderer::CreateSyncObjects(const VkAllocationCallbacks* _allocator)
  {
    m_imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_imagesInFlight.resize(m_swapchain.GetImages().size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
      VkCheck(vkCreateSemaphore(
        m_device.GetDevice(), &semaphoreInfo, _allocator, &m_imageAvailableSemaphore[i]));

      VkCheck(vkCreateSemaphore(
        m_device.GetDevice(), &semaphoreInfo, _allocator, &m_renderFinishedSemaphore[i]));

      VkCheck(vkCreateFence(m_device.GetDevice(), &fenceInfo, _allocator, &m_inFlightFences[i]));
    }
  }

  void Renderer::DestroySyncObjects(const VkAllocationCallbacks* _allocator)
  {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
      vkDestroySemaphore(m_device.GetDevice(), m_renderFinishedSemaphore[i], _allocator);
      vkDestroySemaphore(m_device.GetDevice(), m_imageAvailableSemaphore[i], _allocator);
      vkDestroyFence(m_device.GetDevice(), m_inFlightFences[i], _allocator);
    }
  }

  void Renderer::CreateTestTexture()
  {
    m_testTexture.LoadTexture(
      "textures/test-texture.jpg", m_device, m_graphicsCommandPool, m_transferCommandPool, nullptr);
  }
} // namespace MFVE::Vulkan
