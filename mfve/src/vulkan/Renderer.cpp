#include "vulkan/Renderer.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  VkResult Renderer::Render(const Pipeline& _pipeline, const Framebuffer& _framebuffer,
                            const CommandBuffer& _commandBuffer, const Swapchain& _swapchain)
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
} // namespace MFVE::Vulkan