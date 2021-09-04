#ifndef MY_FIRST_VULKAN_ENGINE_PIPELINE_H
#define MY_FIRST_VULKAN_ENGINE_PIPELINE_H

// Vulkan
#include "vulkan/Vk_Base.h"

// Shaders
#include "shaders/Shader.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class Descriptor;
  class Device;
  class RenderPass;
  class Swapchain;

  class Pipeline
  {
   public:
    Pipeline()  = default;
    ~Pipeline() = default;

    // Graphics Pipeline
    VkResult CreatePipeline(const Device& _device, const Swapchain& _swapchain,
                            const RenderPass& _renderPass,
                            const std::vector<Descriptor>& _descriptors,
                            const VkAllocationCallbacks* _allocator);
    void DestroyPipeline(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
    [[nodiscard]] VkPipeline GetPipeline() const { return m_pipeline; }

   private:
    // Graphics Pipeline
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_pipeline             = VK_NULL_HANDLE;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_PIPELINE_H