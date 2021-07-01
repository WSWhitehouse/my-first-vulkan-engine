#ifndef MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H
#define MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H

#include "Vk_Base.h"
#include "vulkan/PhysicalDevice.h"

namespace MFVE::Vulkan
{
  class LogicalDevice
  {
   public:
    explicit LogicalDevice(PhysicalDevice& _physicalDevice);
    ~LogicalDevice();

    /* Getters */
    [[nodiscard]] const VkDevice& GetDevice() const { return m_device; }
    [[nodiscard]] const VkQueue& GetGraphicsQueue() const { return m_graphicsQueue; }

   private:
    // Logical Device
    VkDevice m_device = VK_NULL_HANDLE;

    // Queues
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;

    PhysicalDevice& m_physicalDevice;
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H
