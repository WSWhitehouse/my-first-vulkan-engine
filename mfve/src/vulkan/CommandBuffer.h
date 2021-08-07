#ifndef MY_FIRST_VULKAN_ENGINE_COMMAND_BUFFERS_H
#define MY_FIRST_VULKAN_ENGINE_COMMAND_BUFFERS_H

// Vulkan
#include "vulkan/Framebuffer.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  class CommandBuffer
  {
   public:
    CommandBuffer()  = default;
    ~CommandBuffer() = default;

    // Command Pool
    VkResult CreateCommandPool(const LogicalDevice& _logicalDevice,
                               const PhysicalDevice& _physicalDevice,
                               const VkAllocationCallbacks* _allocator);
    void DestroyCommandPool(const LogicalDevice& _logicalDevice,
                            const VkAllocationCallbacks* _allocator);

    // Command Buffer
    VkResult AllocateCommandBuffers(const LogicalDevice& _logicalDevice,
                                    const Framebuffer& _framebuffer,
                                    const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] const VkCommandPool& GetCommandPool() const { return m_commandPool; }
    [[nodiscard]] const std::vector<VkCommandBuffer>& GetCommandBuffers() const
    {
      return m_commandBuffers;
    }

   private:
    VkCommandPool m_commandPool                   = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_commandBuffers = {};
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_COMMAND_BUFFERS_H