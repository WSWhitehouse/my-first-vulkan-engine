#include "vulkan/Renderer.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  VkResult Renderer::SetUpCmdBuffers(const Pipeline& _pipeline, const Framebuffer& _framebuffer,
                                     const CommandBuffer& _commandBuffer,
                                     const Swapchain& _swapchain)
  {
    for (size_t i = 0; i < _commandBuffer.GetCommandBuffers().size(); i++)
    {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags            = 0;
      beginInfo.pInheritanceInfo = nullptr;

      const auto beginResult =
        vkBeginCommandBuffer(_commandBuffer.GetCommandBuffers()[i], &beginInfo);

      if (beginResult != VK_SUCCESS)
      {
        return beginResult;
      }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass        = _pipeline.GetRenderPass();
      renderPassInfo.framebuffer       = _framebuffer.GetFramebuffers()[i];
      renderPassInfo.renderArea.offset = { 0, 0 };
      renderPassInfo.renderArea.extent = _swapchain.GetExtent();
      VkClearValue clearColor          = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
      renderPassInfo.clearValueCount   = 1;
      renderPassInfo.pClearValues      = &clearColor;

      vkCmdBeginRenderPass(
        _commandBuffer.GetCommandBuffers()[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      vkCmdBindPipeline(_commandBuffer.GetCommandBuffers()[i],
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        _pipeline.GetPipeline());

      vkCmdDraw(_commandBuffer.GetCommandBuffers()[i], 3, 1, 0, 0);

      vkCmdEndRenderPass(_commandBuffer.GetCommandBuffers()[i]);

      const auto endResult = vkEndCommandBuffer(_commandBuffer.GetCommandBuffers()[i]);

      if (endResult != VK_SUCCESS)
      {
        return endResult;
      }
    }

    return VK_SUCCESS;
  }

  void Renderer::DrawFrame(const LogicalDevice& _logicalDevice, const Pipeline& _pipeline,
                           const Framebuffer& _framebuffer, const CommandBuffer& _commandBuffer,
                           const Swapchain& _swapchain)
  {
    vkWaitForFences(
      _logicalDevice.GetDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    // Acquire image from swap chain
    uint32_t imageIndex;
    vkAcquireNextImageKHR(_logicalDevice.GetDevice(),
                          _swapchain.GetSwapchain(),
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
    submitInfo.pCommandBuffers    = &_commandBuffer.GetCommandBuffers()[imageIndex];

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

    VkSwapchainKHR swapChains[] = { _swapchain.GetSwapchain() };

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains    = swapChains;
    presentInfo.pImageIndices  = &imageIndex;
    presentInfo.pResults       = nullptr;

    vkQueuePresentKHR(_logicalDevice.GetPresentQueue(), &presentInfo);

    // Wait for renderering to finish
    vkQueueWaitIdle(_logicalDevice.GetPresentQueue());

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  void Renderer::CreateSyncObjects(const LogicalDevice& _logicalDevice, const Swapchain& _swapchain,
                                   const VkAllocationCallbacks* _allocator)
  {
    m_imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_imagesInFlight.resize(_swapchain.GetImages().size(), VK_NULL_HANDLE);

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