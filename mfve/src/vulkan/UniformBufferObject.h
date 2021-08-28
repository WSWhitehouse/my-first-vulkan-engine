#ifndef MY_FIRST_VULKAN_ENGINE_UNIFORM_BUFFER_OBJECT_H
#define MY_FIRST_VULKAN_ENGINE_UNIFORM_BUFFER_OBJECT_H

// Vulkan
#include "vulkan/Vk_Base.h"

struct UniformBufferObject
{
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

#endif // MY_FIRST_VULKAN_ENGINE_UNIFORM_BUFFER_OBJECT_H