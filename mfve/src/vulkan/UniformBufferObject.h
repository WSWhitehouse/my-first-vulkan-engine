#ifndef MY_FIRST_VULKAN_ENGINE_UNIFORM_BUFFER_OBJECT_H
#define MY_FIRST_VULKAN_ENGINE_UNIFORM_BUFFER_OBJECT_H

// Vulkan
#include "vulkan/Vk_Base.h"

struct UniformBufferObject
{
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

#endif // MY_FIRST_VULKAN_ENGINE_UNIFORM_BUFFER_OBJECT_H