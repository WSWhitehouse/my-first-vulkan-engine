#ifndef MY_FIRST_VULKAN_ENGINE_BUFFER_H
#define MY_FIRST_VULKAN_ENGINE_BUFFER_H

// Vulkan
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan::Buffer
{
  static inline void Create(const PhysicalDevice& _physicalDevice,
                            const LogicalDevice& _logicalDevice, VkDeviceSize _size,
                            VkBufferUsageFlags _usage, VkMemoryPropertyFlags _properties,
                            VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory,
                            const VkAllocationCallbacks* _allocator)
  {
    // Creating vertex buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size  = _size;
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

    VkCheck(vkCreateBuffer(_logicalDevice.GetDevice(), &bufferInfo, _allocator, &_Buffer));

    // Allocating memory for vertex buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_logicalDevice.GetDevice(), _Buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = _physicalDevice.FindMemoryType(
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkCheck(vkAllocateMemory(_logicalDevice.GetDevice(), &allocInfo, _allocator, &_BufferMemory));

    // Bind memory
    vkBindBufferMemory(_logicalDevice.GetDevice(), _Buffer, _BufferMemory, 0);
  }
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_BUFFER_H