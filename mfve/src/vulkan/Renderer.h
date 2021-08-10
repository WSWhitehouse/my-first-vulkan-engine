#ifndef MY_FIRST_VULKAN_ENGINE_RENDERER_H
#define MY_FIRST_VULKAN_ENGINE_RENDERER_H

// Cpp
#include <vector>

// Vulkan
#include "vulkan/CommandBuffer.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/Pipeline.h"
#include "vulkan/Swapchain.h"
#include "vulkan/Vertex.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  class Renderer
  {
   public:
    Renderer()  = default;
    ~Renderer() = default;

    // Renderer
    void CreateRenderer(const PhysicalDevice& _physicalDevice, const LogicalDevice& _logicalDevice,
                        Window* _window, const VkAllocationCallbacks* _allocator);
    void DestroyRenderer(const LogicalDevice& _logicalDevice,
                         const VkAllocationCallbacks* _allocator);

    void RecreateRenderer(const PhysicalDevice& _physicalDevice,
                          const LogicalDevice& _logicalDevice, Window* _window,
                          const VkAllocationCallbacks* _allocator);

    // Drawing
    void DrawFrame(const PhysicalDevice& _physicalDevice, const LogicalDevice& _logicalDevice,
                   Window* _window, const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] const Swapchain& GetSwapchain() const { return m_swapchain; }
    [[nodiscard]] const Pipeline& GetPipeline() const { return m_pipeline; }
    [[nodiscard]] const Framebuffer& GetFramebuffer() const { return m_framebuffer; }
    [[nodiscard]] const CommandBuffer& GetCommandBuffer() const { return m_commandBuffer; }

   private:
    void CleanUpRenderer(const LogicalDevice& _logicalDevice,
                         const VkAllocationCallbacks* _allocator);

    // Vulkan
    Swapchain m_swapchain         = {};
    Pipeline m_pipeline           = {};
    Framebuffer m_framebuffer     = {};
    CommandBuffer m_commandBuffer = {};

    VertexBuffer m_vertexBuffer = {};

    // Sync Objects
    std::vector<VkSemaphore> m_imageAvailableSemaphore;
    std::vector<VkSemaphore> m_renderFinishedSemaphore;

    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;

    void CreateSyncObjects(const LogicalDevice& _logicalDevice,
                           const VkAllocationCallbacks* _allocator);
    void DestroySyncObjects(const LogicalDevice& _logicalDevice,
                            const VkAllocationCallbacks* _allocator);

    // Frames
    const int MAX_FRAMES_IN_FLIGHT = 2;
    size_t m_currentFrame          = 0;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_RENDERER_H