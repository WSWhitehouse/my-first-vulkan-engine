#ifndef MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H
#define MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H

#include "Vk_Base.h"

class PhysicalDevice
{
 public:
  PhysicalDevice()  = default;
  ~PhysicalDevice() = default;

  /* Getters */
  [[nodiscard]] const VkPhysicalDevice& GetDevice() const { return m_physicalDevice; }
  [[nodiscard]] const VkPhysicalDeviceFeatures& GetFeatures() const { return m_features; }
  [[nodiscard]] const VkPhysicalDeviceProperties& GetProperties() const { return m_properties; }

 private:
  VkPhysicalDevice m_physicalDevice       = VK_NULL_HANDLE;
  VkPhysicalDeviceFeatures m_features     = {};
  VkPhysicalDeviceProperties m_properties = {};
};

#endif // MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H
