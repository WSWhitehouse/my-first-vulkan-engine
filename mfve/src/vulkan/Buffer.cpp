#include "vulkan/Buffer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/CommandBuffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/Device.h"

namespace MFVE::Vulkan
{
  void Buffer::CreateBuffer(const Device& _device, VkDeviceSize _bufferSize,
                            VkBufferUsageFlags _usage, VkMemoryPropertyFlags _properties,
                            const VkAllocationCallbacks* _allocator)
  {
    m_bufferSize = _bufferSize;

    // Creating buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size  = m_bufferSize;
    bufferInfo.usage = _usage;

    std::set<uint32_t> queueFamilyIndiciesSet = _device.GetUniqueQueueFamilyIndicies();
    std::vector<uint32_t> indicies(queueFamilyIndiciesSet.begin(), queueFamilyIndiciesSet.end());

    if (queueFamilyIndiciesSet.size() > 1)
    {
      bufferInfo.sharingMode           = VK_SHARING_MODE_CONCURRENT;
      bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(indicies.size());
      bufferInfo.pQueueFamilyIndices   = indicies.data();
    }
    else
    {
      bufferInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
      bufferInfo.queueFamilyIndexCount = 0;
      bufferInfo.pQueueFamilyIndices   = nullptr;
    }

    VkCheck(vkCreateBuffer(_device.GetDevice(), &bufferInfo, _allocator, &m_buffer));

    // Allocating memory for buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device.GetDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = _device.FindMemoryType(memRequirements.memoryTypeBits,
                                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkCheck(vkAllocateMemory(_device.GetDevice(), &allocInfo, _allocator, &m_bufferMemory));

    // Bind memory
    vkBindBufferMemory(_device.GetDevice(), m_buffer, m_bufferMemory, 0);
  }

  void Buffer::DestroyBuffer(const Device& _device, const VkAllocationCallbacks* _allocator)
  {
    vkDestroyBuffer(_device.GetDevice(), m_buffer, _allocator);
    vkFreeMemory(_device.GetDevice(), m_bufferMemory, _allocator);
  }

  void Buffer::FillBuffer(const Device& _device, const void* _data, const VkDeviceSize& _size,
                          const VkDeviceSize& _offset, const VkMemoryMapFlags& _flags)
  {
    void* memory;
    vkMapMemory(_device.GetDevice(), m_bufferMemory, _offset, _size, _flags, &memory);
    memcpy(memory, _data, (size_t)_size);
    vkUnmapMemory(_device.GetDevice(), m_bufferMemory);
  }

  void Buffer::CopyFromBuffer(const Device& _device, const CommandPool& _transferCommandPool,
                              const Buffer& _srcBuffer, VkDeviceSize _bufferSize)
  {
    VkCommandBuffer commandBuffer = _transferCommandPool.BeginSingleTimeCommands(_device);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size      = _bufferSize;
    vkCmdCopyBuffer(commandBuffer, _srcBuffer.GetBuffer(), m_buffer, 1, &copyRegion);

    _transferCommandPool.EndSingleTimeCommands(_device, commandBuffer);
  }

} // namespace MFVE::Vulkan