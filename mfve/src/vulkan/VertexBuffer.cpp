#include "vulkan/VertexBuffer.h"

#include <mfve_pch.h>

#include "vulkan/Buffer.h"

namespace MFVE::Vulkan
{
  void VertexBuffer::CreateVertexBuffer(const PhysicalDevice& _physicalDevice,
                                        const LogicalDevice& _logicalDevice,
                                        const VkAllocationCallbacks* _allocator)
  {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    Buffer::Create(_physicalDevice,
                   _logicalDevice,
                   bufferSize,
                   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                   m_vertexBuffer,
                   m_vertexBufferMemory,
                   _allocator);

    // Fill Vertex Buffer
    void* data;
    vkMapMemory(_logicalDevice.GetDevice(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(_logicalDevice.GetDevice(), m_vertexBufferMemory);
  }

  void VertexBuffer::DestroyVertexBuffer(const LogicalDevice& _logicalDevice,
                                         const VkAllocationCallbacks* _allocator)
  {
    vkDestroyBuffer(_logicalDevice.GetDevice(), m_vertexBuffer, _allocator);
    vkFreeMemory(_logicalDevice.GetDevice(), m_vertexBufferMemory, _allocator);
  }
} // namespace MFVE::Vulkan