#include "vulkan/Renderer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
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

    VkCheck(m_swapchain.CreateSwapchain(m_device, _window, _allocator));
    VkCheck(m_swapchain.CreateImageViews(m_device, _allocator));
    VkCheck(m_pipeline.CreateRenderPasses(m_device, m_swapchain, _allocator));
    VkCheck(m_pipeline.CreateDescriptorSetLayout(m_device, m_swapchain, _allocator));
    VkCheck(m_pipeline.CreatePipeline(m_device, m_swapchain, _allocator));
    VkCheck(m_framebuffer.CreateFramebuffers(m_device, m_swapchain, m_pipeline, _allocator));

    VkCheck(
      m_graphicsCommandPool.CreateCommandPool(m_device, m_device.GetGraphicsQueue(), _allocator));
    VkCheck(
      m_transferCommandPool.CreateCommandPool(m_device, m_device.GetTransferQueue(), _allocator));

    CreateVertexBuffer(_allocator);
    CreateIndexBuffer(_allocator);

    CreateUniformBuffers(_allocator);
    CreateDescriptorPool(_allocator);
    CreateDescriptorSets();

    SetUpGraphicsCommandBuffer();
    CreateSyncObjects(_allocator);
  }

  void Renderer::DestroyRenderer(const VkAllocationCallbacks* _allocator)
  {
    CleanUpSwapchain(_allocator);

    m_pipeline.DestroyDescriptorSetLayout(m_device, _allocator);
    m_vertexBuffer.DestroyBuffer(m_device, _allocator);
    m_indexBuffer.DestroyBuffer(m_device, _allocator);
    DestroySyncObjects(_allocator);
    m_graphicsCommandPool.DestroyCommandPool(m_device, _allocator);
    m_transferCommandPool.DestroyCommandPool(m_device, _allocator);
    m_device.DestroyDevice(nullptr);
    ValidationLayers::DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
    m_window->DestroySurface(m_instance, nullptr);
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

    VkCheck(m_swapchain.CreateSwapchain(m_device, m_window, _allocator));
    VkCheck(m_swapchain.CreateImageViews(m_device, _allocator));
    VkCheck(m_pipeline.CreateRenderPasses(m_device, m_swapchain, _allocator));
    VkCheck(m_pipeline.CreatePipeline(m_device, m_swapchain, _allocator));
    VkCheck(m_framebuffer.CreateFramebuffers(m_device, m_swapchain, m_pipeline, _allocator));

    CreateUniformBuffers(_allocator);
    CreateDescriptorPool(_allocator);
    CreateDescriptorSets();

    SetUpGraphicsCommandBuffer();

    m_imagesInFlight.resize(m_swapchain.GetImages().size(), VK_NULL_HANDLE);
  }

  void Renderer::CleanUpSwapchain(const VkAllocationCallbacks* _allocator)
  {
    m_framebuffer.DestroyFramebuffers(m_device, _allocator);
    m_graphicsCommandBuffer.FreeCommandBuffers(m_device, m_graphicsCommandPool);
    m_pipeline.DestroyPipeline(m_device, _allocator);
    m_pipeline.DestroyRenderPasses(m_device, _allocator);
    m_swapchain.DestroyImageViews(m_device, _allocator);
    m_swapchain.DestroySwapchain(m_device, _allocator);

    for (auto& buffer : m_uniformBuffers)
    {
      buffer.DestroyBuffer(m_device, _allocator);
    }

    vkDestroyDescriptorPool(m_device.GetDevice(), m_descriptorPool, _allocator);
  }

  void Renderer::SetUpGraphicsCommandBuffer()
  {
    m_graphicsCommandBuffer.AllocateCommandBuffers(
      m_device, m_graphicsCommandPool, m_framebuffer.GetFramebuffers().size());

    auto& commandBuffers = m_graphicsCommandBuffer.GetCommandBuffers();

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags            = 0;
      beginInfo.pInheritanceInfo = nullptr;

      VkCheck(vkBeginCommandBuffer(commandBuffers[i], &beginInfo));

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass        = m_pipeline.GetRenderPass();
      renderPassInfo.framebuffer       = m_framebuffer.GetFramebuffers()[i];
      renderPassInfo.renderArea.offset = { 0, 0 };
      renderPassInfo.renderArea.extent = m_swapchain.GetExtent();
      VkClearValue clearColor          = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
      renderPassInfo.clearValueCount   = 1;
      renderPassInfo.pClearValues      = &clearColor;

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
                              &m_descriptorSets[i],
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

    // Fill Vertex Buffer
    void* data;
    vkMapMemory(m_device.GetDevice(), stagingBuffer.GetBufferMemory(), 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(m_device.GetDevice(), stagingBuffer.GetBufferMemory());

    m_vertexBuffer.CreateBuffer(m_device,
                                bufferSize,
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                _allocator);

    m_vertexBuffer.CopyBuffer(m_device, m_transferCommandPool, stagingBuffer, bufferSize);

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

    // Fill Vertex Buffer
    void* data;
    vkMapMemory(m_device.GetDevice(), stagingBuffer.GetBufferMemory(), 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(m_device.GetDevice(), stagingBuffer.GetBufferMemory());

    m_indexBuffer.CreateBuffer(m_device,
                               bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               _allocator);

    m_indexBuffer.CopyBuffer(m_device, m_transferCommandPool, stagingBuffer, bufferSize);

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
    VkDescriptorPoolSize poolSize{};
    poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(m_swapchain.GetImages().size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes    = &poolSize;
    poolInfo.maxSets       = static_cast<uint32_t>(m_swapchain.GetImages().size());

    VkCheck(vkCreateDescriptorPool(m_device.GetDevice(), &poolInfo, _allocator, &m_descriptorPool));
  }

  void Renderer::CreateDescriptorSets()
  {
    std::vector<VkDescriptorSetLayout> layouts(m_swapchain.GetImages().size(),
                                               m_pipeline.GetDescriptorSetLayout());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(m_swapchain.GetImages().size());
    allocInfo.pSetLayouts        = layouts.data();

    m_descriptorSets.resize(m_swapchain.GetImages().size());
    VkCheck(vkAllocateDescriptorSets(m_device.GetDevice(), &allocInfo, m_descriptorSets.data()));

    for (size_t i = 0; i < m_swapchain.GetImages().size(); i++)
    {
      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = m_uniformBuffers[i].GetBuffer();
      bufferInfo.offset = 0;
      bufferInfo.range  = sizeof(UniformBufferObject);

      VkWriteDescriptorSet descriptorWrite{};
      descriptorWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrite.dstSet           = m_descriptorSets[i];
      descriptorWrite.dstBinding       = 0;
      descriptorWrite.dstArrayElement  = 0;
      descriptorWrite.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrite.descriptorCount  = 1;
      descriptorWrite.pBufferInfo      = &bufferInfo;
      descriptorWrite.pImageInfo       = nullptr;
      descriptorWrite.pTexelBufferView = nullptr;

      vkUpdateDescriptorSets(m_device.GetDevice(), 1, &descriptorWrite, 0, nullptr);
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
} // namespace MFVE::Vulkan