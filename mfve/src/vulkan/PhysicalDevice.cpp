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
      QueueFamily queueFamily = FindQueueFamilies(device, _surface);

      if (queueFamily.IsComplete())
      {
        m_physicalDevice = device;
        m_queueFamily    = queueFamily;
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

  PhysicalDevice::QueueFamily
  PhysicalDevice::FindQueueFamilies(VkPhysicalDevice _device, VkSurfaceKHR _surface)
  {
    QueueFamily family;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

    for (int i = 0; i < queueFamilyCount; ++i)
    {
      auto queueFamily = queueFamilies.at(i);

      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        family.graphicsFamily = i;
      }

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, _surface, &presentSupport);

      if (presentSupport)
      {
        family.presentFamily = i;
      }

      if (family.IsComplete())
      {
        break;
      }
    }

    return family;
  }
}