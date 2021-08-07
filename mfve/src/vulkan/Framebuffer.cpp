#include "vulkan/Framebuffer.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  VkResult Framebuffer::CreateFramebuffers(const LogicalDevice& _logicalDevice,
                                           const Swapchain& _swapchain, const Pipeline& _pipeline,
                                           const VkAllocationCallbacks* _allocator)
  {
    const auto& imageViews = _swapchain.GetImageViews();

    m_framebuffers.resize(imageViews.size());

    for (size_t i = 0; i < imageViews.size(); i++)
    {
      VkImageView attachments[] = { imageViews[i] };

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass      = _pipeline.GetRenderPass();
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments    = attachments;
      framebufferInfo.width           = _swapchain.GetExtent().width;
      framebufferInfo.height          = _swapchain.GetExtent().height;
      framebufferInfo.layers          = 1;

      const VkResult result = vkCreateFramebuffer(
        _logicalDevice.GetDevice(), &framebufferInfo, _allocator, &m_framebuffers[i]);

      if (result != VK_SUCCESS)
      {
        return result;
      }
    }

    return VK_SUCCESS;
  }

  void Framebuffer::DestroyFramebuffers(const LogicalDevice& _logicalDevice,
                                        const VkAllocationCallbacks* _allocator)
  {
    for (auto framebuffer : m_framebuffers)
    {
      vkDestroyFramebuffer(_logicalDevice.GetDevice(), framebuffer, _allocator);
    }

    m_framebuffers.clear();
  }
} // namespace MFVE::Vulkan