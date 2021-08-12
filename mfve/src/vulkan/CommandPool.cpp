#include "vulkan/CommandPool.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Device.h"

namespace MFVE::Vulkan
{
  VkResult CommandPool::CreateCommandPool(const Device& _device, QueueFamily _queueFamily,
                                          const VkAllocationCallbacks* _allocator)
  {
    m_queueFamily = std::move(_queueFamily);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_queueFamily.familyIndex;
    poolInfo.flags            = 0;

    return vkCreateCommandPool(_device.GetDevice(), &poolInfo, _allocator, &m_commandPool);
  }

  void CommandPool::DestroyCommandPool(const Device& _device,
                                       const VkAllocationCallbacks* _allocator)
  {
    vkDestroyCommandPool(_device.GetDevice(), m_commandPool, _allocator);
  }
} // namepsace MFVE::Vulkan