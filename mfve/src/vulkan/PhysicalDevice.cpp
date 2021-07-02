#include "PhysicalDevice.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  void PhysicalDevice::PickSuitableDevice(VkInstance _instance, VkSurfaceKHR _surface)
  {
    const auto devices = FindAllPhysicalDevices(_instance);

    if (devices.empty())
    {
      MFVE_LOG_FATAL("Failed to find GPUs with Vulkan Support");
    }

    for (const auto& device : devices)
    {
      if (IsDeviceSuitable(device, _surface))
      {
        m_physicalDevice = device;
        vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
        vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_features);
        return;
      }
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
    {
      MFVE_LOG_FATAL("Failed to find a suitable GPU!");
    }
  }

  bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice _device, VkSurfaceKHR _surface)
  {
    m_queueFamilies.FindQueueFamilies(_device, _surface);
    bool extensionsSupported = CheckDeviceExtensionSupport(_device);
    return m_queueFamilies.IsComplete() && extensionsSupported;
  }

  bool PhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice _device)
  {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
      _device, nullptr, &extensionCount, availableExtensions.data());

    return std::all_of(
      m_deviceExtensions.cbegin(),
      m_deviceExtensions.cend(),
      [&](const char* extension)
      {
        for (const auto& ext : availableExtensions)
        {
          if (strcmp(extension, ext.extensionName) == 0)
          {
            return true;
          }
        }

        return false;
      });
  }

} // namespace MFVE::Vulkan