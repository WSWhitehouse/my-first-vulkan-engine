#ifndef MY_FIRST_VULKAN_ENGINE_VERTEX_H
#define MY_FIRST_VULKAN_ENGINE_VERTEX_H

// Cpp
#include <array>
#include <vector>

// GLM
#include <glm/glm.hpp>

// Vulkan
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  struct Vertex
  {
    glm::vec3 pos;
    glm::vec3 colour;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription GetBindingDescription()
    {
      VkVertexInputBindingDescription bindingDescription{};
      bindingDescription.binding   = 0;
      bindingDescription.stride    = sizeof(Vertex);
      bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
    {
      std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

      attributeDescriptions[0].binding  = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[0].offset   = offsetof(Vertex, pos);

      attributeDescriptions[1].binding  = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset   = offsetof(Vertex, colour);

      attributeDescriptions[2].binding  = 0;
      attributeDescriptions[2].location = 2;
      attributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[2].offset   = offsetof(Vertex, texCoord);

      return attributeDescriptions;
    }
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_VERTEX_H