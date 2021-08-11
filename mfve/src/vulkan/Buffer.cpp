#include "vulkan/Buffer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Buffer.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"

namespace MFVE::Vulkan
{
  void Buffer::CreateBuffer(const PhysicalDevice& _physicalDevice,
                            const LogicalDevice& _logicalDevice, VkDeviceSize _bufferSize,
                            VkBufferUsageFlags _usage, VkMemoryPropertyFlags _properties,
                            const VkAllocationCallbacks* _allocator)
  {
    m_bufferSize = _bufferSize;

    // Creating buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size  = m_bufferSize;
    bufferInfo.usage = _usage;

    std::set<uint32_t> queueFamilyIndiciesSet = _logicalDevice.GetUniqueQueueFamilyIndicies();
    std::vector<uint32_t> indicies(queueFamilyIndiciesSet.begin(), queueFamilyIndiciesSet.end());

    if (queueFamilyIndiciesSet.size() > 1)
    {
      bufferInfo.sharingMode           = VK_SHARING_MODE_CONCURRENT;
      bufferInfo.queueFamilyIndexCount = indicies.size();
      bufferInfo.pQueueFamilyIndices   = indicies.data();
    }
    else
    {
      bufferInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
      bufferInfo.queueFamilyIndexCount = 0;
      bufferInfo.pQueueFamilyIndices   = nullptr;
    }

    VkCheck(vkCreateBuffer(_logicalDevice.GetDevice(), &bufferInfo, _allocator, &m_buffer));

    // Allocating memory for buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_logicalDevice.GetDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = _physicalDevice.FindMemoryType(
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkCheck(vkAllocateMemory(_logicalDevice.GetDevice(), &allocInfo, _allocator, &m_bufferMemory));

    // Bind memory
    vkBindBufferMemory(_logicalDevice.GetDevice(), m_buffer, m_bufferMemory, 0);
  }

  void Buffer::DestroyBuffer(const LogicalDevice& _logicalDevice,
                             const VkAllocationCallbacks* _allocator)
  {
    vkDestroyBuffer(_logicalDevice.GetDevice(), m_buffer, _allocator);
    vkFreeMemory(_logicalDevice.GetDevice(), m_bufferMemory, _allocator);
  }

  void Buffer::CopyBuffer(const LogicalDevice& _logicalDevice, const CommandPool& _commandPool,
                          const Buffer& _srcBuffer, VkDeviceSize _bufferSize)
  {
    CommandBuffer commandBuffer;
    commandBuffer.AllocateCommandBuffers(_logicalDevice, _commandPool, 1);

    auto cmdbuffer = commandBuffer.GetCommandBuffers().at(0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cmdbuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size      = _bufferSize;
    vkCmdCopyBuffer(cmdbuffer, _srcBuffer.GetBuffer(), m_buffer, 1, &copyRegion);

    vkEndCommandBuffer(cmdbuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &cmdbuffer;

    vkQueueSubmit(_commandPool.GetQueueFamily().queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_commandPool.GetQueueFamily().queue);

    commandBuffer.FreeCommandBuffers(_logicalDevice, _commandPool);
  }

} // namespace MFVE::Vulkan