#ifndef MY_FIRST_VULKAN_ENGINE_RENDER_PASS_H
#define MY_FIRST_VULKAN_ENGINE_RENDER_PASS_H

// Vulkan
#include "vulkan/Image.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class DepthBuffer;
  class Device;
  class Swapchain;

  class RenderPass
  {
   public:
    RenderPass()  = default;
    ~RenderPass() = default;

    void CreateRenderPass(const Device& _device, const Swapchain& _swapchain,
                          const DepthBuffer& _depthBuffer, const VkAllocationCallbacks* _allocator);
    void DestroyRenderPass(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] VkRenderPass GetRenderPass() const { return m_renderPass; }

   private:
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_RENDER_PASS_H