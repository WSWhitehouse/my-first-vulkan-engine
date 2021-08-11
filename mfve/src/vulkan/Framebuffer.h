#ifndef MY_FIRST_VULKAN_ENGINE_FRAMEBUFFER_H
#define MY_FIRST_VULKAN_ENGINE_FRAMEBUFFER_H

// Vulkan
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class LogicalDevice;
  class Pipeline;
  class Swapchain;

  class Framebuffer
  {
   public:
    Framebuffer()  = default;
    ~Framebuffer() = default;

    // Framebuffers
    VkResult CreateFramebuffers(const LogicalDevice& _logicalDevice, const Swapchain& _swapchain,
                                const Pipeline& _pipeline, const VkAllocationCallbacks* _allocator);
    void DestroyFramebuffers(const LogicalDevice& _logicalDevice,
                             const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] const std::vector<VkFramebuffer>& GetFramebuffers() const
    {
      return m_framebuffers;
    }

   private:
    std::vector<VkFramebuffer> m_framebuffers = {};
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_FRAMEBUFFER_H