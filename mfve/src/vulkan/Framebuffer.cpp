#include "vulkan/Framebuffer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Device.h"
#include "vulkan/Pipeline.h"
#include "vulkan/Swapchain.h"

namespace MFVE::Vulkan
{
  VkResult Framebuffer::CreateFramebuffers(const Device& _device, const Swapchain& _swapchain,
                                           const Pipeline& _pipeline,
                                           const VkAllocationCallbacks* _allocator)
  {
    const auto& images = _swapchain.GetImages();

    m_framebuffers.resize(images.size());

    for (size_t i = 0; i < images.size(); i++)
    {
      VkImageView attachments[] = { images[i].GetImageView() };

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass      = _pipeline.GetRenderPass();
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments    = attachments;
      framebufferInfo.width           = _swapchain.GetExtent().width;
      framebufferInfo.height          = _swapchain.GetExtent().height;
      framebufferInfo.layers          = 1;

      const VkResult result =
        vkCreateFramebuffer(_device.GetDevice(), &framebufferInfo, _allocator, &m_framebuffers[i]);

      if (result != VK_SUCCESS)
      {
        return result;
      }
    }

    return VK_SUCCESS;
  }

  void Framebuffer::DestroyFramebuffers(const Device& _device,
                                        const VkAllocationCallbacks* _allocator)
  {
    for (auto framebuffer : m_framebuffers)
    {
      vkDestroyFramebuffer(_device.GetDevice(), framebuffer, _allocator);
    }

    m_framebuffers.clear();
  }
} // namespace MFVE::Vulkan