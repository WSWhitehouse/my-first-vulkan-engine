#ifndef MY_FIRST_VULKAN_ENGINE_PIPELINE_H
#define MY_FIRST_VULKAN_ENGINE_PIPELINE_H

// Vulkan
#include "vulkan/LogicalDevice.h"
#include "vulkan/Swapchain.h"
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

    VkResult CreateRenderPasses(const LogicalDevice& _logicalDevice, const Swapchain& _swapchain,
                                const VkAllocationCallbacks* _allocator);
    void DestroyRenderPasses(const LogicalDevice& _logicalDevice,
                             const VkAllocationCallbacks* _allocator);

    VkResult CreatePipelineLayout(const LogicalDevice& _logicalDevice, const Swapchain& _swapchain,
                                  const VkAllocationCallbacks* _allocator);
    void DestroyPipelineLayout(const LogicalDevice& _logicalDevice,
                               const VkAllocationCallbacks* _allocator);

   private:
    Shader m_fragShader = {};
    Shader m_vertShader = {};

    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
  };
} // namepsace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_PIPELINE_H