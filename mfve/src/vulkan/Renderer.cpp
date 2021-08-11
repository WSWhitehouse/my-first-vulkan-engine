#include "vulkan/Renderer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"

namespace MFVE::Vulkan
{
  void Renderer::CreateRenderer(const PhysicalDevice& _physicalDevice,
                                const LogicalDevice& _logicalDevice, Window* _window,
                                const VkAllocationCallbacks* _allocator)
  {
    VkCheck(m_swapchain.CreateSwapchain(_physicalDevice, _logicalDevice, _window, _allocator));
    VkCheck(m_swapchain.CreateImageViews(_logicalDevice, _allocator));
    VkCheck(m_pipeline.CreateRenderPasses(_logicalDevice, m_swapchain, _allocator));
    VkCheck(m_pipeline.CreatePipeline(_logicalDevice, m_swapchain, _allocator));
    VkCheck(m_framebuffer.CreateFramebuffers(_logicalDevice, m_swapchain, m_pipeline, _allocator));

    VkCheck(m_graphicsCommandPool.CreateCommandPool(
      _logicalDevice, _logicalDevice.GetGraphicsQueue(), _allocator));
    VkCheck(m_transferCommandPool.CreateCommandPool(
      _logicalDevice, _logicalDevice.GetTransferQueue(), _allocator));

    CreateVertexBuffer(_physicalDevice, _logicalDevice, _allocator);
    CreateIndexBuffer(_physicalDevice, _logicalDevice, _allocator);

    SetUpGraphicsCommandBuffer(_logicalDevice);
    CreateSyncObjects(_logicalDevice, _allocator);
  }

  void Renderer::DestroyRenderer(const LogicalDevice& _logicalDevice,
                                 const VkAllocationCallbacks* _allocator)
  {
    CleanUpRenderer(_logicalDevice, _allocator);

    DestroySyncObjects(_logicalDevice, _allocator);
    m_vertexBuffer.DestroyBuffer(_logicalDevice, _allocator);
    m_indexBuffer.DestroyBuffer(_logicalDevice, _allocator);
    m_graphicsCommandPool.DestroyCommandPool(_logicalDevice, _allocator);
    m_transferCommandPool.DestroyCommandPool(_logicalDevice, _allocator);
  }

  void Renderer::RecreateRenderer(const PhysicalDevice& _physicalDevice,
                                  const LogicalDevice& _logicalDevice, Window* _window,
                                  const VkAllocationCallbacks* _allocator)
  {
    _window->WaitWhileMinimised();

    vkDeviceWaitIdle(_logicalDevice.GetDevice());

    CleanUpRenderer(_logicalDevice, _allocator);

    VkCheck(m_swapchain.CreateSwapchain(_physicalDevice, _logicalDevice, _window, _allocator));
    VkCheck(m_swapchain.CreateImageViews(_logicalDevice, _allocator));
    VkCheck(m_pipeline.CreateRenderPasses(_logicalDevice, m_swapchain, _allocator));
    VkCheck(m_pipeline.CreatePipeline(_logicalDevice, m_swapchain, _allocator));
    VkCheck(m_framebuffer.CreateFramebuffers(_logicalDevice, m_swapchain, m_pipeline, _allocator));
    SetUpGraphicsCommandBuffer(_logicalDevice);
  }

  void Renderer::CleanUpRenderer(const LogicalDevice& _logicalDevice,
                                 const VkAllocationCallbacks* _allocator)
  {
    m_graphicsCommandBuffer.FreeCommandBuffers(_logicalDevice, m_graphicsCommandPool);
    m_framebuffer.DestroyFramebuffers(_logicalDevice, _allocator);
    m_pipeline.DestroyPipeline(_logicalDevice, _allocator);
    m_pipeline.DestroyRenderPasses(_logicalDevice, _allocator);
    m_swapchain.DestroyImageViews(_logicalDevice, _allocator);
    m_swapchain.DestroySwapchain(_logicalDevice, _allocator);
  }

  void Renderer::SetUpGraphicsCommandBuffer(const LogicalDevice& _logicalDevice)
  {
    m_graphicsCommandBuffer.AllocateCommandBuffers(
      _logicalDevice, m_graphicsCommandPool, m_framebuffer.GetFramebuffers().size());

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

      // Draw
      vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

      vkCmdEndRenderPass(commandBuffers[i]);

      VkCheck(vkEndCommandBuffer(commandBuffers[i]));
    }
  }

  void Renderer::CreateVertexBuffer(const PhysicalDevice& _physicalDevice,
                                    const LogicalDevice& _logicalDevice,
                                    const VkAllocationCallbacks* _allocator)
  {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    Buffer stagingBuffer;
    stagingBuffer.CreateBuffer(_physicalDevice,
                               _logicalDevice,
                               bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               _allocator);

    // Fill Vertex Buffer
    void* data;
    vkMapMemory(
      _logicalDevice.GetDevice(), stagingBuffer.GetBufferMemory(), 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(_logicalDevice.GetDevice(), stagingBuffer.GetBufferMemory());

    m_vertexBuffer.CreateBuffer(_physicalDevice,
                                _logicalDevice,
                                bufferSize,
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                _allocator);

    m_vertexBuffer.CopyBuffer(_logicalDevice, m_transferCommandPool, stagingBuffer, bufferSize);

    stagingBuffer.DestroyBuffer(_logicalDevice, _allocator);
  }

  void Renderer::CreateIndexBuffer(const PhysicalDevice& _physicalDevice,
                                   const LogicalDevice& _logicalDevice,
                                   const VkAllocationCallbacks* _allocator)
  {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    Buffer stagingBuffer;
    stagingBuffer.CreateBuffer(_physicalDevice,
                               _logicalDevice,
                               bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               _allocator);

    // Fill Vertex Buffer
    void* data;
    vkMapMemory(
      _logicalDevice.GetDevice(), stagingBuffer.GetBufferMemory(), 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(_logicalDevice.GetDevice(), stagingBuffer.GetBufferMemory());

    m_indexBuffer.CreateBuffer(_physicalDevice,
                               _logicalDevice,
                               bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               _allocator);

    m_indexBuffer.CopyBuffer(_logicalDevice, m_transferCommandPool, stagingBuffer, bufferSize);

    stagingBuffer.DestroyBuffer(_logicalDevice, _allocator);
  }

  void Renderer::DrawFrame(const PhysicalDevice& _physicalDevice,
                           const LogicalDevice& _logicalDevice, Window* _window,
                           const VkAllocationCallbacks* _allocator)
  {
    vkWaitForFences(
      _logicalDevice.GetDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    // Acquire image from swap chain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_logicalDevice.GetDevice(),
                                            m_swapchain.GetSwapchain(),
                                            UINT64_MAX,
                                            m_imageAvailableSemaphore[m_currentFrame],
                                            VK_NULL_HANDLE,
                                            &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
      RecreateRenderer(_physicalDevice, _logicalDevice, _window, _allocator);
      return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
      VkCheck(result);
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
      vkWaitForFences(
        _logicalDevice.GetDevice(), 1, &m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
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

    vkResetFences(_logicalDevice.GetDevice(), 1, &m_inFlightFences[m_currentFrame]);

    VkCheck(vkQueueSubmit(
      _logicalDevice.GetGraphicsQueue().queue, 1, &submitInfo, m_inFlightFences[m_currentFrame]));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapchain.GetSwapchain() };

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains    = swapChains;
    presentInfo.pImageIndices  = &imageIndex;
    presentInfo.pResults       = nullptr;

    result = vkQueuePresentKHR(_logicalDevice.GetPresentQueue().queue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        _window->HasWindowResized())
    {
      _window->ResetWindowResized();
      RecreateRenderer(_physicalDevice, _logicalDevice, _window, _allocator);
    }
    else
    {
      VkCheck(result);
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  void Renderer::CreateSyncObjects(const LogicalDevice& _logicalDevice,
                                   const VkAllocationCallbacks* _allocator)
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
        _logicalDevice.GetDevice(), &semaphoreInfo, _allocator, &m_imageAvailableSemaphore[i]));

      VkCheck(vkCreateSemaphore(
        _logicalDevice.GetDevice(), &semaphoreInfo, _allocator, &m_renderFinishedSemaphore[i]));

      VkCheck(
        vkCreateFence(_logicalDevice.GetDevice(), &fenceInfo, _allocator, &m_inFlightFences[i]));
    }
  }

  void Renderer::DestroySyncObjects(const LogicalDevice& _logicalDevice,
                                    const VkAllocationCallbacks* _allocator)
  {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
      vkDestroySemaphore(_logicalDevice.GetDevice(), m_renderFinishedSemaphore[i], _allocator);
      vkDestroySemaphore(_logicalDevice.GetDevice(), m_imageAvailableSemaphore[i], _allocator);
      vkDestroyFence(_logicalDevice.GetDevice(), m_inFlightFences[i], _allocator);
    }
  }
} // namespace MFVE::Vulkan