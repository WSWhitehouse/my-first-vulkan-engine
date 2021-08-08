#include "vulkan/CommandBuffer.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  VkResult CommandBuffer::CreateCommandPool(const LogicalDevice& _logicalDevice,
                                            const PhysicalDevice& _physicalDevice,
                                            const VkAllocationCallbacks* _allocator)
  {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = _physicalDevice.GetQueueFamilies().graphicsFamily.value();
    poolInfo.flags            = 0;

    return vkCreateCommandPool(_logicalDevice.GetDevice(), &poolInfo, nullptr, &m_commandPool);
  }

  void CommandBuffer::DestroyCommandPool(const LogicalDevice& _logicalDevice,
                                         const VkAllocationCallbacks* _allocator)
  {
    vkDestroyCommandPool(_logicalDevice.GetDevice(), m_commandPool, nullptr);
  }

  VkResult CommandBuffer::AllocateCommandBuffers(const LogicalDevice& _logicalDevice,
                                                 const Framebuffer& _framebuffer,
                                                 const VkAllocationCallbacks* _allocator)
  {
    m_commandBuffers.resize(_framebuffer.GetFramebuffers().size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool        = m_commandPool;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    return vkAllocateCommandBuffers(
      _logicalDevice.GetDevice(), &allocInfo, m_commandBuffers.data());
  }
} // namepsace MFVE::Vulkan