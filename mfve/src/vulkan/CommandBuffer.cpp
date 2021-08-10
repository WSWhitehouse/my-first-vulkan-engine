#include "vulkan/CommandBuffer.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  VkResult CommandBuffer::CreateCommandPool(const LogicalDevice& _logicalDevice,
                                            const PhysicalDevice& _physicalDevice,
                                            const VkAllocationCallbacks* _allocator)
  {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = _physicalDevice.GetQueueFamilies().graphicsFamily.value();
    poolInfo.flags            = 0;

    return vkCreateCommandPool(_logicalDevice.GetDevice(), &poolInfo, _allocator, &m_commandPool);
  }

  void CommandBuffer::DestroyCommandPool(const LogicalDevice& _logicalDevice,
                                         const VkAllocationCallbacks* _allocator)
  {
    vkDestroyCommandPool(_logicalDevice.GetDevice(), m_commandPool, _allocator);
  }

  void CommandBuffer::AllocateCommandBuffers(const LogicalDevice& _logicalDevice,
                                             const Swapchain& _swapchain,
                                             const VertexBuffer& _vertexBuffer,
                                             const Pipeline& _pipeline,
                                             const Framebuffer& _framebuffer)
  {
    m_commandBuffers.resize(_framebuffer.GetFramebuffers().size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool        = m_commandPool;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    VkCheck(
      vkAllocateCommandBuffers(_logicalDevice.GetDevice(), &allocInfo, m_commandBuffers.data()));

    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags            = 0;
      beginInfo.pInheritanceInfo = nullptr;

      VkCheck(vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo));

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass        = _pipeline.GetRenderPass();
      renderPassInfo.framebuffer       = _framebuffer.GetFramebuffers()[i];
      renderPassInfo.renderArea.offset = { 0, 0 };
      renderPassInfo.renderArea.extent = _swapchain.GetExtent();
      VkClearValue clearColor          = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
      renderPassInfo.clearValueCount   = 1;
      renderPassInfo.pClearValues      = &clearColor;

      vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      VkBuffer vertexBuffers[] = { _vertexBuffer.GetVertexBuffer() };
      VkDeviceSize offsets[]   = { 0 };
      vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

      vkCmdBindPipeline(
        m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline.GetPipeline());

      vkCmdDraw(m_commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);

      vkCmdEndRenderPass(m_commandBuffers[i]);

      VkCheck(vkEndCommandBuffer(m_commandBuffers[i]));
    }
  }

  void CommandBuffer::FreeCommandBuffers(const LogicalDevice& _logicalDevice)
  {
    vkFreeCommandBuffers(_logicalDevice.GetDevice(),
                         m_commandPool,
                         static_cast<uint32_t>(m_commandBuffers.size()),
                         m_commandBuffers.data());
  }
} // namepsace MFVE::Vulkan