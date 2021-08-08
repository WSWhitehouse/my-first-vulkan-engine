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

  VkResult Renderer::DrawFrame(const LogicalDevice& _logicalDevice, const Pipeline& _pipeline,
                               const Framebuffer& _framebuffer, const CommandBuffer& _commandBuffer,
                               const Swapchain& _swapchain)
  {
    // Acquire image from swap chain
    uint32_t imageIndex;
    vkAcquireNextImageKHR(_logicalDevice.GetDevice(),
                          _swapchain.GetSwapchain(),
                          UINT64_MAX,
                          m_imageAvailableSemaphore,
                          VK_NULL_HANDLE,
                          &imageIndex);

    // Submitting the command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[]      = { m_imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores    = waitSemaphores;
    submitInfo.pWaitDstStageMask  = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &_commandBuffer.GetCommandBuffers()[imageIndex];

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    const auto queueSubmitResult =
      vkQueueSubmit(_logicalDevice.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);

    if (queueSubmitResult != VK_SUCCESS)
    {
      return VK_SUCCESS;
    }

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

    return VK_SUCCESS;
  }

  VkResult Renderer::CreateSemaphores(const LogicalDevice& _logicalDevice,
                                      const VkAllocationCallbacks* _allocator)
  {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    const auto imageResult = vkCreateSemaphore(
      _logicalDevice.GetDevice(), &semaphoreInfo, _allocator, &m_imageAvailableSemaphore);

    if (imageResult != VK_SUCCESS)
    {
      return imageResult;
    }

    return vkCreateSemaphore(
      _logicalDevice.GetDevice(), &semaphoreInfo, _allocator, &m_renderFinishedSemaphore);
  }

  void Renderer::DestroySemaphores(const LogicalDevice& _logicalDevice,
                                   const VkAllocationCallbacks* _allocator)
  {
    vkDestroySemaphore(_logicalDevice.GetDevice(), m_renderFinishedSemaphore, _allocator);
    vkDestroySemaphore(_logicalDevice.GetDevice(), m_imageAvailableSemaphore, _allocator);
  }
} // namespace MFVE::Vulkan