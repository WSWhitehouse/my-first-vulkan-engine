#include "PhysicalDevice.h"

namespace MFVE::Vulkan
{
  void PhysicalDevice::PickSuitableDevice(VkInstance _instance)
  {
    const auto devices = FindAllPhysicalDevices(_instance);

    if (devices.empty())
    {
      MFVE_LOG_FATAL("Failed to find GPUs with Vulkan Support");
    }

    for (const auto& device : devices)
    {
      if (IsDeviceSuitable(device))
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

  bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice _device)
  {
    QueueFamily queueFamily = FindQueueFamilies(_device);

    return queueFamily.IsComplete();
  }

  PhysicalDevice::QueueFamily PhysicalDevice::FindQueueFamilies(VkPhysicalDevice _device)
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

      if (family.IsComplete())
      {
        break;
      }
    }

    return family;
  }
}