#ifndef MY_FIRST_VULKAN_ENGINE_DEPTH_BUFFER_H
#define MY_FIRST_VULKAN_ENGINE_DEPTH_BUFFER_H

// Vulkan
#include "vulkan/Image.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class CommandPool;
  class Device;
  class Swapchain;

  class DepthBuffer
  {
   public:
    DepthBuffer()  = default;
    ~DepthBuffer() = default;

    bool CreateDepthBuffer(const Device& _device, const Swapchain& _swapchain,
                           const CommandPool& m_graphicsCommandPool,
                           const VkAllocationCallbacks* _allocator);
    void DestroyDepthBuffer(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] const Image& GetImage() const { return m_image; }
    [[nodiscard]] const VkFormat& GetFormat() const { return m_format; }

   private:
    Image m_image     = {};
    VkFormat m_format = VK_FORMAT_UNDEFINED;

    static inline bool hasStencilComponent(const VkFormat& format)
    {
      return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_DEPTH_BUFFER_H