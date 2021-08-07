#ifndef MY_FIRST_VULKAN_ENGINE_PIPELINE_H
#define MY_FIRST_VULKAN_ENGINE_PIPELINE_H

// Vulkan
#include "vulkan/LogicalDevice.h"
#include "vulkan/Vk_Base.h"

// Shaders
#include "shaders/Shader.h"

namespace MFVE::Vulkan
{
  class Pipeline
  {
   public:
    Pipeline()  = default;
    ~Pipeline() = default;

    void CreatePipeline(const LogicalDevice& _logicalDevice);

   private:
    Shader m_fragShader = {};
    Shader m_vertShader = {};
  };
} // namepsace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_PIPELINE_H