#include "vulkan/DepthBuffer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/CommandPool.h"
#include "vulkan/Device.h"
#include "vulkan/Swapchain.h"

namespace MFVE::Vulkan
{
  bool DepthBuffer::CreateDepthBuffer(const Device& _device, const Swapchain& _swapchain,
                                      const CommandPool& m_graphicsCommandPool,
                                      const VkAllocationCallbacks* _allocator)
  {
    m_format = _device.FindSupportedFormat(
      { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    const auto& extent = _swapchain.GetExtent();

    m_image.CreateImage(_device,
                        extent.width,
                        extent.height,
                        m_format,
                        VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        _allocator);

    m_image.CreateImageView(_device, m_format, VK_IMAGE_ASPECT_DEPTH_BIT, _allocator);

    m_image.SetImageLayout(_device,
                           m_graphicsCommandPool,
                           m_format,
                           VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  }

  void DepthBuffer::DestroyDepthBuffer(const Device& _device,
                                       const VkAllocationCallbacks* _allocator)
  {
    m_image.DestroyImageView(_device, _allocator);
    m_image.DestroyImage(_device, _allocator);
  }
} // namespace MFVE::Vulkan