#ifndef MY_FIRST_VULKAN_ENGINE_COMMAND_BUFFERS_H
#define MY_FIRST_VULKAN_ENGINE_COMMAND_BUFFERS_H

// Vulkan
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class CommandPool;
  class Device;

  class CommandBuffer
  {
   public:
    CommandBuffer()  = default;
    ~CommandBuffer() = default;

    // Command Buffer
    void AllocateCommandBuffers(const Device& _device, const CommandPool& _commandPool,
                                uint32_t _commandBufferCount,
                                VkCommandBufferLevel _level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    void FreeCommandBuffers(const Device& _device, const CommandPool& _commandPool);

    // Getters
    [[nodiscard]] std::vector<VkCommandBuffer>& GetCommandBuffers() { return m_commandBuffers; }
    [[nodiscard]] const std::vector<VkCommandBuffer>& GetCommandBuffers() const
    {
      return m_commandBuffers;
    }

   private:
    std::vector<VkCommandBuffer> m_commandBuffers = {};
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_COMMAND_BUFFERS_H