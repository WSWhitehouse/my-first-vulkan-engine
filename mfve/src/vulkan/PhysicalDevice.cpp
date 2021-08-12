#include "vulkan/PhysicalDevice.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Swapchain.h"

namespace MFVE::Vulkan
{
  void PhysicalDevice::PickSuitableDevice(const Instance& _instance, VkSurfaceKHR _surface)
  {
    const std::vector<VkPhysicalDevice> devices = FindAllPhysicalDevices(_instance);

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

  uint32_t PhysicalDevice::FindMemoryType(uint32_t _typeFilter,
                                          VkMemoryPropertyFlags _properties) const
  {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
      if ((_typeFilter & (1 << i)) &&
          (memProperties.memoryTypes[i].propertyFlags & _properties) == _properties)
      {
        return i;
      }
    }

    MFVE_LOG_FATAL("Failed to find suitable memory type on Physical Device!");
    return 0;
  }

  bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice _device, VkSurfaceKHR _surface)
  {
    m_queueFamilies.FindQueueFamilies(_device, _surface);
    bool extensionsSupported = CheckDeviceExtensionSupport(_device);

    bool swapChainAdequate;
    if (extensionsSupported)
    {
      const auto swapchainSupport = Swapchain::SupportDetails::QuerySupport(_device, _surface);
      swapChainAdequate           = !swapchainSupport.formats.empty() &&
                          !swapchainSupport.presentModes.empty();
    }

    return m_queueFamilies.IsComplete() && extensionsSupported && swapChainAdequate;
  }

  bool PhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice _device)
  {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
      _device, nullptr, &extensionCount, availableExtensions.data());

    return std::all_of(m_deviceExtensions.cbegin(),
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