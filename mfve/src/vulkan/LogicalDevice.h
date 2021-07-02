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

    VkResult CreateDevice(PhysicalDevice* _physicalDevice, const VkAllocationCallbacks* _allocator);
    void CreateQueueHandles();
    void Destroy(const VkAllocationCallbacks* _allocator);

    /* Getters */
    [[nodiscard]] const VkDevice& GetDevice() const { return m_device; }
    [[nodiscard]] const VkQueue& GetGraphicsQueue() const { return m_graphicsQueue; }
    [[nodiscard]] const VkQueue& GetPresentQueue() const { return m_presentQueue; }

   private:
    // Logical Device
    VkDevice m_device                = VK_NULL_HANDLE;
    PhysicalDevice* m_physicalDevice = nullptr;

    // Queues
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue  = VK_NULL_HANDLE;
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H
