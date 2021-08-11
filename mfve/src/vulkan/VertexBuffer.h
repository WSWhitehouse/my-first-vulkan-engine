#ifndef MY_FIRST_VULKAN_ENGINE_VERTEX_BUFFER_H
#define MY_FIRST_VULKAN_ENGINE_VERTEX_BUFFER_H

// Cpp
#include <array>
#include <vector>

// GLM
#include <glm/glm.hpp>

// Vulkan
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"
#include "vulkan/Vertex.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  class CommandBuffer;

  static inline const std::vector<Vertex> vertices = { { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                                       { { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                                       { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } } };

  class VertexBuffer
  {
   public:
    VertexBuffer()  = default;
    ~VertexBuffer() = default;

    void CreateVertexBuffer(const PhysicalDevice& _physicalDevice,
                            const LogicalDevice& _logicalDevice,
                            const CommandBuffer& _commandBuffer,
                            const VkAllocationCallbacks* _allocator);
    void DestroyVertexBuffer(const LogicalDevice& _logicalDevice,
                             const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] VkBuffer GetVertexBuffer() const { return m_vertexBuffer; }

   private:
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_VERTEX_BUFFER_H