#include "vulkan/VertexBuffer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Buffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"

namespace MFVE::Vulkan
{
  void VertexBuffer::CreateVertexBuffer(const PhysicalDevice& _physicalDevice,
                                        const LogicalDevice& _logicalDevice,
                                        const CommandPool& _commandPool,
                                        const VkAllocationCallbacks* _allocator)
  {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Buffer::Create(_physicalDevice,
                   _logicalDevice,
                   bufferSize,
                   VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                   stagingBuffer,
                   stagingBufferMemory,
                   _allocator);

    // Fill Vertex Buffer
    void* data;
    vkMapMemory(_logicalDevice.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(_logicalDevice.GetDevice(), stagingBufferMemory);

    Buffer::Create(_physicalDevice,
                   _logicalDevice,
                   bufferSize,
                   VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                   m_vertexBuffer,
                   m_vertexBufferMemory,
                   _allocator);

    Buffer::Copy(_logicalDevice, _commandPool, stagingBuffer, m_vertexBuffer, bufferSize);

    Buffer::Destroy(_logicalDevice, stagingBuffer, stagingBufferMemory, _allocator);
  }

  void VertexBuffer::DestroyVertexBuffer(const LogicalDevice& _logicalDevice,
                                         const VkAllocationCallbacks* _allocator)
  {
    Buffer::Destroy(_logicalDevice, m_vertexBuffer, m_vertexBufferMemory, _allocator);
  }
} // namespace MFVE::Vulkan