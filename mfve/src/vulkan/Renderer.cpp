#include "vulkan/Renderer.h"

#include <mfve_pch.h>

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
    VkCheck(m_commandBuffer.CreateCommandPool(_logicalDevice, _physicalDevice, nullptr));
    m_commandBuffer.AllocateCommandBuffers(_logicalDevice, m_swapchain, m_pipeline, m_framebuffer);
    CreateSyncObjects(_logicalDevice, _allocator);
  }

  void Renderer::DestroyRenderer(const LogicalDevice& _logicalDevice,
                                 const VkAllocationCallbacks* _allocator)
  {
    CleanUpRenderer(_logicalDevice, _allocator);
    DestroySyncObjects(_logicalDevice, _allocator);
    m_commandBuffer.DestroyCommandPool(_logicalDevice, _allocator);
  }

  void Renderer::RecreateRenderer(const PhysicalDevice& _physicalDevice,
                                  const LogicalDevice& _logicalDevice, Window* _window,
                                  const VkAllocationCallbacks* _allocator)
  {
    vkDeviceWaitIdle(_logicalDevice.GetDevice());

    CleanUpRenderer(_logicalDevice, _allocator);

    VkCheck(m_swapchain.CreateSwapchain(_physicalDevice, _logicalDevice, _window, _allocator));
    VkCheck(m_swapchain.CreateImageViews(_logicalDevice, _allocator));
    VkCheck(m_pipeline.CreateRenderPasses(_logicalDevice, m_swapchain, _allocator));
    VkCheck(m_pipeline.CreatePipeline(_logicalDevice, m_swapchain, _allocator));
    VkCheck(m_framebuffer.CreateFramebuffers(_logicalDevice, m_swapchain, m_pipeline, _allocator));
    m_commandBuffer.AllocateCommandBuffers(_logicalDevice, m_swapchain, m_pipeline, m_framebuffer);
  }

  void Renderer::CleanUpRenderer(const LogicalDevice& _logicalDevice,
                                 const VkAllocationCallbacks* _allocator)
  {
    m_commandBuffer.FreeCommandBuffers(_logicalDevice);
    m_framebuffer.DestroyFramebuffers(_logicalDevice, _allocator);
    m_pipeline.DestroyPipeline(_logicalDevice, _allocator);
    m_pipeline.DestroyRenderPasses(_logicalDevice, _allocator);
    m_swapchain.DestroyImageViews(_logicalDevice, _allocator);
    m_swapchain.DestroySwapchain(_logicalDevice, _allocator);
  }

  void Renderer::DrawFrame(const LogicalDevice& _logicalDevice)
  {
    vkWaitForFences(
      _logicalDevice.GetDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    // Acquire image from swap chain
    uint32_t imageIndex;
    vkAcquireNextImageKHR(_logicalDevice.GetDevice(),
                          m_swapchain.GetSwapchain(),
                          UINT64_MAX,
                          m_imageAvailableSemaphore[m_currentFrame],
                          VK_NULL_HANDLE,
                          &imageIndex);

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
    submitInfo.pCommandBuffers    = &m_commandBuffer.GetCommandBuffers()[imageIndex];

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore[m_currentFrame] };

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    vkResetFences(_logicalDevice.GetDevice(), 1, &m_inFlightFences[m_currentFrame]);

    VkCheck(vkQueueSubmit(
      _logicalDevice.GetGraphicsQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapchain.GetSwapchain() };

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains    = swapChains;
    presentInfo.pImageIndices  = &imageIndex;
    presentInfo.pResults       = nullptr;

    vkQueuePresentKHR(_logicalDevice.GetPresentQueue(), &presentInfo);

    // Wait for renderering to finish
    vkQueueWaitIdle(_logicalDevice.GetPresentQueue());

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