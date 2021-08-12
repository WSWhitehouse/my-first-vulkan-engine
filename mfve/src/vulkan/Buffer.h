#ifndef MY_FIRST_VULKAN_ENGINE_BUFFER_H
#define MY_FIRST_VULKAN_ENGINE_BUFFER_H

// Vulkan
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class CommandPool;
  class Device;

  class Buffer
  {
   public:
    Buffer()  = default;
    ~Buffer() = default;

    void CreateBuffer(const Device& _device, VkDeviceSize _bufferSize, VkBufferUsageFlags _usage,
                      VkMemoryPropertyFlags _properties, const VkAllocationCallbacks* _allocator);

    void DestroyBuffer(const Device& _device, const VkAllocationCallbacks* _allocator);

    void CopyBuffer(const Device& _device, const CommandPool& _commandPool,
                    const Buffer& _srcBuffer, VkDeviceSize _bufferSize);

    // Getters
    [[nodiscard]] const VkDeviceSize& GetBufferSize() const { return m_bufferSize; }
    [[nodiscard]] VkBuffer GetBuffer() const { return m_buffer; }
    [[nodiscard]] VkDeviceMemory GetBufferMemory() const { return m_bufferMemory; }

   private:
    VkDeviceSize m_bufferSize     = 0;
    VkBuffer m_buffer             = VK_NULL_HANDLE;
    VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_BUFFER_H