#ifndef MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H
#define MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H

#include "Vk_Base.h"
#include "vulkan/PhysicalDevice.h"

namespace MFVE::Vulkan
{
  class LogicalDevice
  {
   public:
    LogicalDevice()  = default;
    ~LogicalDevice() = default;

    VkResult CreateDevice(const PhysicalDevice& _physicalDevice,
                          const VkAllocationCallbacks* _allocator);
    void CreateQueueHandles(const PhysicalDevice& _physicalDevice);
    void Destroy(const VkAllocationCallbacks* _allocator);

    /* Getters */
    [[nodiscard]] VkDevice GetDevice() const { return m_device; }
    [[nodiscard]] VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
    [[nodiscard]] VkQueue GetPresentQueue() const { return m_presentQueue; }

   private:
    // Logical Device
    VkDevice m_device = VK_NULL_HANDLE;

    // Queues
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue  = VK_NULL_HANDLE;
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H
