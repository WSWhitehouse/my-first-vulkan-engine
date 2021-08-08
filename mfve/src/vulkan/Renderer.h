#ifndef MY_FIRST_VULKAN_ENGINE_RENDERER_H
#define MY_FIRST_VULKAN_ENGINE_RENDERER_H

// Vulkan
#include "vulkan/CommandBuffer.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/Pipeline.h"
#include "vulkan/Swapchain.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  class Renderer
  {
   public:
    Renderer()  = default;
    ~Renderer() = default;

    VkResult SetUpCmdBuffers(const Pipeline& _pipeline, const Framebuffer& _framebuffer,
                             const CommandBuffer& _commandBuffer, const Swapchain& _swapchain);

    // Semaphores
    VkResult CreateSemaphores(const LogicalDevice& _logicalDevice,
                              const VkAllocationCallbacks* _allocator);
    void DestroySemaphores(const LogicalDevice& _logicalDevice,
                           const VkAllocationCallbacks* _allocator);

    VkResult DrawFrame(const LogicalDevice& _logicalDevice, const Pipeline& _pipeline,
                   const Framebuffer& _framebuffer, const CommandBuffer& _commandBuffer,
                   const Swapchain& _swapchain);

   private:
    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_RENDERER_H