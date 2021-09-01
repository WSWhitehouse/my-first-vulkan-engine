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

  VkCommandBuffer CommandPool::BeginSingleTimeCommands(const Device& _device) const
  {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool        = GetCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device.GetDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
  }

  void CommandPool::EndSingleTimeCommands(const Device& _device, VkCommandBuffer _commandBuffer) const
  {
    vkEndCommandBuffer(_commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &_commandBuffer;

    vkQueueSubmit(GetQueueFamily().queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(GetQueueFamily().queue);

    vkFreeCommandBuffers(_device.GetDevice(), GetCommandPool(), 1, &_commandBuffer);
  }

} // namepsace MFVE::Vulkan