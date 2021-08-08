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

    VkResult Render(const Pipeline& _pipeline, const Framebuffer& _framebuffer,
                    const CommandBuffer& _commandBuffer, const Swapchain& _swapchain);
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_RENDERER_H