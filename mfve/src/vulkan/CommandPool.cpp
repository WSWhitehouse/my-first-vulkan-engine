#include "vulkan/CommandPool.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/LogicalDevice.h"

namespace MFVE::Vulkan
{
  VkResult CommandPool::CreateCommandPool(const LogicalDevice& _logicalDevice,
                                          QueueFamily _queueFamily,
                                          const VkAllocationCallbacks* _allocator)
  {
    m_queueFamily = std::move(_queueFamily);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_queueFamily.familyIndex;
    poolInfo.flags            = 0;

    return vkCreateCommandPool(_logicalDevice.GetDevice(), &poolInfo, _allocator, &m_commandPool);
  }

  void CommandPool::DestroyCommandPool(const LogicalDevice& _logicalDevice,
                                       const VkAllocationCallbacks* _allocator)
  {
    vkDestroyCommandPool(_logicalDevice.GetDevice(), m_commandPool, _allocator);
  }
} // namepsace MFVE::Vulkan