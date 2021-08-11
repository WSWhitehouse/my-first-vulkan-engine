#ifndef MY_FIRST_VULKAN_ENGINE_COMMAND_POOL_H
#define MY_FIRST_VULKAN_ENGINE_COMMAND_POOL_H

// Vulkan
#include "vulkan/QueueFamily.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class LogicalDevice;

  class CommandPool
  {
   public:
    CommandPool()  = default;
    ~CommandPool() = default;

    // Command Pool
    VkResult CreateCommandPool(const LogicalDevice& _logicalDevice, QueueFamily _queueFamily,
                               const VkAllocationCallbacks* _allocator);
    void DestroyCommandPool(const LogicalDevice& _logicalDevice,
                            const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] const QueueFamily& GetQueueFamily() const { return m_queueFamily; }
    [[nodiscard]] VkCommandPool GetCommandPool() const { return m_commandPool; }

   private:
    QueueFamily m_queueFamily   = {};
    VkCommandPool m_commandPool = VK_NULL_HANDLE;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_COMMAND_POOL_H