#include "vulkan/CommandBuffer.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/CommandPool.h"
#include "vulkan/Device.h"

namespace MFVE::Vulkan
{
  void CommandBuffer::AllocateCommandBuffers(const Device& _device, const CommandPool& _commandPool,
                                             uint32_t _commandBufferCount,
                                             VkCommandBufferLevel _level)
  {
    m_commandBuffers.resize(_commandBufferCount);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool        = _commandPool.GetCommandPool();
    allocInfo.level              = _level;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    VkCheck(vkAllocateCommandBuffers(_device.GetDevice(), &allocInfo, m_commandBuffers.data()));
  }

  void CommandBuffer::FreeCommandBuffers(const Device& _device, const CommandPool& _commandPool)
  {
    vkFreeCommandBuffers(_device.GetDevice(),
                         _commandPool.GetCommandPool(),
                         static_cast<uint32_t>(m_commandBuffers.size()),
                         m_commandBuffers.data());
  }
} // namepsace MFVE::Vulkan