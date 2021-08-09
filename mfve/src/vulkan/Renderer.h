#ifndef MY_FIRST_VULKAN_ENGINE_RENDERER_H
#define MY_FIRST_VULKAN_ENGINE_RENDERER_H

// Cpp
#include <vector>

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

    // Sync Objects
    void CreateSyncObjects(const LogicalDevice& _logicalDevice, const Swapchain& _swapchain,
                           const VkAllocationCallbacks* _allocator);
    void DestroySyncObjects(const LogicalDevice& _logicalDevice,
                            const VkAllocationCallbacks* _allocator);

    void DrawFrame(const LogicalDevice& _logicalDevice, const Pipeline& _pipeline,
                   const Framebuffer& _framebuffer, const CommandBuffer& _commandBuffer,
                   const Swapchain& _swapchain);

   private:
    // Sync Objects
    std::vector<VkSemaphore> m_imageAvailableSemaphore;
    std::vector<VkSemaphore> m_renderFinishedSemaphore;

    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;

    // Frames
    const int MAX_FRAMES_IN_FLIGHT = 2;
    size_t m_currentFrame          = 0;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_RENDERER_H