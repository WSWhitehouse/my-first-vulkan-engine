#include "vulkan/Vertex.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  void VertexBuffer::CreateVertexBuffer(const PhysicalDevice& _physicalDevice,
                                        const LogicalDevice& _logicalDevice,
                                        const VkAllocationCallbacks* _allocator)
  {
    // Creating vertex buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size        = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkCheck(vkCreateBuffer(_logicalDevice.GetDevice(), &bufferInfo, _allocator, &m_vertexBuffer));

    // Allocating memory for vertex buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_logicalDevice.GetDevice(), m_vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = _physicalDevice.FindMemoryType(
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkCheck(
      vkAllocateMemory(_logicalDevice.GetDevice(), &allocInfo, _allocator, &m_vertexBufferMemory));

    // Bind memory
    vkBindBufferMemory(_logicalDevice.GetDevice(), m_vertexBuffer, m_vertexBufferMemory, 0);

    // Fill Vertex Buffer
    void* data;
    vkMapMemory(_logicalDevice.GetDevice(), m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);

    memcpy(data, vertices.data(), (size_t)bufferInfo.size);

    vkUnmapMemory(_logicalDevice.GetDevice(), m_vertexBufferMemory);
  }

  void VertexBuffer::DestroyVertexBuffer(const LogicalDevice& _logicalDevice,
                                         const VkAllocationCallbacks* _allocator)
  {
    vkDestroyBuffer(_logicalDevice.GetDevice(), m_vertexBuffer, _allocator);
    vkFreeMemory(_logicalDevice.GetDevice(), m_vertexBufferMemory, _allocator);
  }
} // namespace MFVE::Vulkan