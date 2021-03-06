#ifndef MY_FIRST_VULKAN_ENGINE_RENDERER_H
#define MY_FIRST_VULKAN_ENGINE_RENDERER_H

// Cpp
#include <vector>

// Core
#include "core/Properties.h"
#include "core/Window.h"

// Vulkan
#include "vulkan/Buffer.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/DepthBuffer.h"
#include "vulkan/Descriptor.h"
#include "vulkan/Device.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/Image.h"
#include "vulkan/Instance.h"
#include "vulkan/Pipeline.h"
#include "vulkan/RenderPass.h"
#include "vulkan/Swapchain.h"
#include "vulkan/Texture.h"
#include "vulkan/UniformBufferObject.h"
#include "vulkan/Vertex.h"
#include "vulkan/Vk_Base.h"

namespace MFVE
{
  class Renderer
  {
   public:
    Renderer()  = default;
    ~Renderer() = default;

    // Renderer
    void CreateRenderer(const AppProperties& _appProperties, Window* _window,
                        const VkAllocationCallbacks* _allocator);
    void DestroyRenderer(const VkAllocationCallbacks* _allocator);

    void DeviceWaitIdle() { vkDeviceWaitIdle(m_device.GetDevice()); }
    bool SignalExit() { return m_window->WindowShouldClose(); }

    // Drawing
    void PreRender() { m_window->UpdateEvents(); }
    void DrawFrame(const VkAllocationCallbacks* _allocator);
    void PostRender() {}

    // Getters
    [[nodiscard]] Window* GetWindow() const { return m_window; }

   private:
    void RecreateSwapchain(const VkAllocationCallbacks* _allocator);
    void CleanUpSwapchain(const VkAllocationCallbacks* _allocator);

    // Window
    Window* m_window = nullptr;

    /* Vulkan */
    // Extensions
    std::vector<const char*> m_extensions = {};

    // Instance
    Vulkan::Instance m_instance = {};

    // Debug Messenger
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

    // Devices
    Vulkan::Device m_device = {};

    Vulkan::Swapchain m_swapchain     = {};
    Vulkan::Pipeline m_pipeline       = {};
    Vulkan::DepthBuffer m_depthBuffer = {};
    Vulkan::RenderPass m_renderPass   = {};
    Vulkan::Framebuffer m_framebuffer = {};

    // Command Buffers
    Vulkan::CommandPool m_graphicsCommandPool     = {};
    Vulkan::CommandBuffer m_graphicsCommandBuffer = {};
    void SetUpGraphicsCommandBuffer();

    Vulkan::CommandPool m_transferCommandPool = {};

    // Buffers
    Vulkan::Buffer m_vertexBuffer = {};
    void CreateVertexBuffer(const VkAllocationCallbacks* _allocator);

    Vulkan::Buffer m_indexBuffer = {};
    void CreateIndexBuffer(const VkAllocationCallbacks* _allocator);

    // Uniform Buffers
    std::vector<Vulkan::Buffer> m_uniformBuffers = {};
    void CreateUniformBuffers(const VkAllocationCallbacks* _allocator);
    void UpdateUniformBuffer(uint32_t _currentImage);

    // Descriptor
    Vulkan::Descriptor m_descriptor = {};
    void CreateDescriptorPool(const VkAllocationCallbacks* _allocator);
    void CreateDescriptorSets(const VkAllocationCallbacks* _allocator);

    const std::vector<Vulkan::Vertex> vertices = {
      { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
      { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
      { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
      { { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

      { { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
      { { 0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
      { { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
      { { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
    };

    const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };

    // Sync Objects
    std::vector<VkSemaphore> m_imageAvailableSemaphore;
    std::vector<VkSemaphore> m_renderFinishedSemaphore;

    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;

    void CreateSyncObjects(const VkAllocationCallbacks* _allocator);
    void DestroySyncObjects(const VkAllocationCallbacks* _allocator);

    // Frames
    const int MAX_FRAMES_IN_FLIGHT = 2;
    size_t m_currentFrame          = 0;

    // Test
    Vulkan::Texture m_testTexture = {};
    void CreateTestTexture();
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_RENDERER_H