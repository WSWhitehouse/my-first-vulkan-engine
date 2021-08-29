#ifndef MY_FIRST_VULKAN_ENGINE_PIPELINE_H
#define MY_FIRST_VULKAN_ENGINE_PIPELINE_H

// Vulkan
#include "vulkan/Vk_Base.h"

// Shaders
#include "shaders/Shader.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class Device;
  class Swapchain;

  class Pipeline
  {
   public:
    Pipeline()  = default;
    ~Pipeline() = default;

    // Render Passes
    VkResult CreateRenderPasses(const Device& _device, const Swapchain& _swapchain,
                                const VkAllocationCallbacks* _allocator);
    void DestroyRenderPasses(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Descriptor Set Layout
    VkResult CreateDescriptorSetLayout(const Device& _device, const Swapchain& _swapchain,
                                       const VkAllocationCallbacks* _allocator);
    void DestroyDescriptorSetLayout(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Graphics Pipeline
    VkResult CreatePipeline(const Device& _device, const Swapchain& _swapchain,
                            const VkAllocationCallbacks* _allocator);
    void DestroyPipeline(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] VkRenderPass GetRenderPass() const { return m_renderPass; }
    [[nodiscard]] VkDescriptorSetLayout GetDescriptorSetLayout() const
    {
      return m_descriptorSetLayout;
    }
    [[nodiscard]] VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
    [[nodiscard]] VkPipeline GetPipeline() const { return m_pipeline; }

   private:
    // Render Passes
    VkRenderPass m_renderPass = VK_NULL_HANDLE;

    // Descriptor Set Layout
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

    // Graphics Pipeline
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_pipeline             = VK_NULL_HANDLE;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_PIPELINE_H