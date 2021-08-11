#ifndef MY_FIRST_VULKAN_ENGINE_RENDERER_H
#define MY_FIRST_VULKAN_ENGINE_RENDERER_H

// Cpp
#include <vector>

// Vulkan
#include "vulkan/Buffer.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/Pipeline.h"
#include "vulkan/Swapchain.h"
#include "vulkan/Vertex.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class LogicalDevice;
  class PhysicalDevice;

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
    [[nodiscard]] const CommandBuffer& GetCommandBuffer() const { return m_graphicsCommandBuffer; }

   private:
    void CleanUpRenderer(const LogicalDevice& _logicalDevice,
                         const VkAllocationCallbacks* _allocator);

    // Vulkan
    Swapchain m_swapchain     = {};
    Pipeline m_pipeline       = {};
    Framebuffer m_framebuffer = {};

    CommandPool m_graphicsCommandPool     = {};
    CommandBuffer m_graphicsCommandBuffer = {};

    void SetUpGraphicsCommandBuffer(const LogicalDevice& _logicalDevice);

    CommandPool m_transferCommandPool = {};

    Buffer m_vertexBuffer = {};
    void CreateVertexBuffer(const PhysicalDevice& _physicalDevice,
                            const LogicalDevice& _logicalDevice,
                            const VkAllocationCallbacks* _allocator);

    Buffer m_indexBuffer = {};
    void CreateIndexBuffer(const PhysicalDevice& _physicalDevice,
                           const LogicalDevice& _logicalDevice,
                           const VkAllocationCallbacks* _allocator);

    const std::vector<Vertex> vertices = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                           { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
                                           { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                           { { -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f } } };

    const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

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