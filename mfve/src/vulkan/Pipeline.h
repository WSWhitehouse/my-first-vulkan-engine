#ifndef MY_FIRST_VULKAN_ENGINE_PIPELINE_H
#define MY_FIRST_VULKAN_ENGINE_PIPELINE_H

// Cpp
#include <vector>

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

    // Render Passes
    VkResult CreateRenderPasses(const LogicalDevice& _logicalDevice, const Swapchain& _swapchain,
                                const VkAllocationCallbacks* _allocator);
    void DestroyRenderPasses(const LogicalDevice& _logicalDevice,
                             const VkAllocationCallbacks* _allocator);

    // Graphics Pipeline
    VkResult CreatePipeline(const LogicalDevice& _logicalDevice, const Swapchain& _swapchain,
                            const VkAllocationCallbacks* _allocator);
    void DestroyPipeline(const LogicalDevice& _logicalDevice,
                         const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] VkRenderPass GetRenderPass() const { return m_renderPass; }
    [[nodiscard]] VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
    [[nodiscard]] VkPipeline GetPipeline() const { return m_pipeline; }

   private:
    Shader m_fragShader = {};
    Shader m_vertShader = {};

    // Render Passes
    VkRenderPass m_renderPass = VK_NULL_HANDLE;

    // Graphics Pipeline
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_pipeline             = VK_NULL_HANDLE;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_PIPELINE_H