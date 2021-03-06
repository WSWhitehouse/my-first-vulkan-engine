#include "vulkan/Device.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Extensions.h"
#include "vulkan/Instance.h"
#include "vulkan/QueueFamilyIndicies.h"
#include "vulkan/Swapchain.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
  void Device::CreateDevice(const Instance& _instance, Window* _window,
                            const VkAllocationCallbacks* _allocator)
  {
    // Pick Physical Device
    const std::vector<VkPhysicalDevice> devices = FindAllPhysicalDevices(_instance);

    if (devices.empty())
    {
      MFVE_LOG_FATAL("Failed to find GPUs with Vulkan Support");
    }

    QueueFamilyIndicies queueFamilyIndicies;
    for (const auto& device : devices)
    {
      if (IsDeviceSuitable(device, _window->GetSurface(), queueFamilyIndicies))
      {
        m_physicalDevice = device;
        break;
      }
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
    {
      MFVE_LOG_FATAL("Failed to find a suitable GPU!");
    }

    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);

    // Create Logical Device
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndicies.graphicsFamily.value(),
                                               queueFamilyIndicies.presentFamily.value(),
                                               queueFamilyIndicies.transferFamily.value() };

    float queuePriority = 1.0f;
    for (const auto& queueFamily : uniqueQueueFamilies)
    {
      VkDeviceQueueCreateInfo createInfo{};
      createInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      createInfo.queueFamilyIndex = queueFamily;
      createInfo.queueCount       = 1;
      createInfo.pQueuePriorities = &queuePriority;

      queueCreateInfos.push_back(createInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos    = queueCreateInfos.data();
    createInfo.pEnabledFeatures     = &deviceFeatures;

    createInfo.enabledExtensionCount   = static_cast<uint32_t>(m_deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    if (ValidationLayers::Enabled())
    {
      createInfo.enabledLayerCount   = static_cast<uint32_t>(ValidationLayers::Layers.size());
      createInfo.ppEnabledLayerNames = ValidationLayers::Layers.data();
    }
    else
    {
      createInfo.enabledLayerCount = 0;
    }

    VkCheck(vkCreateDevice(m_physicalDevice, &createInfo, _allocator, &m_device));

    // Create Queue Handles
    m_graphicsQueue.CreateHandle(m_device, queueFamilyIndicies.graphicsFamily.value(), 0);
    m_presentQueue.CreateHandle(m_device, queueFamilyIndicies.presentFamily.value(), 0);
    m_transferQueue.CreateHandle(m_device, queueFamilyIndicies.transferFamily.value(), 0);
  }

  void Device::DestroyDevice(const VkAllocationCallbacks* _allocator)
  {
    vkDestroyDevice(m_device, _allocator);
  }

  uint32_t Device::FindMemoryType(uint32_t _typeFilter, VkMemoryPropertyFlags _properties) const
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
  }

  VkFormat Device::FindSupportedFormat(const std::vector<VkFormat>& candidates,
                                       const VkImageTiling& tiling,
                                       const VkFormatFeatureFlags& features) const
  {
    for (const auto& format : candidates)
    {
      VkFormatProperties props;
      vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

      if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
      {
        return format;
      }
      else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features)
      {
        return format;
      }

      MFVE_LOG_FATAL("Failed to find supported formats");
      return VK_FORMAT_UNDEFINED;
    }
  }

  bool Device::IsDeviceSuitable(VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface,
                                QueueFamilyIndicies& _queueFamilyIndicies)
  {
    _queueFamilyIndicies.FindQueueFamilies(_physicalDevice, _surface);
    bool extensionsSupported = CheckDeviceExtensionSupport(_physicalDevice);

    bool swapChainAdequate;
    if (extensionsSupported)
    {
      const auto supportDetails = SupportDetails::QuerySupport(_physicalDevice, _surface);
      swapChainAdequate = !supportDetails.formats.empty() && !supportDetails.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(_physicalDevice, &supportedFeatures);

    return _queueFamilyIndicies.IsComplete() && extensionsSupported && swapChainAdequate &&
           supportedFeatures.samplerAnisotropy;
  }

  bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice _physicalDevice)
  {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
      _physicalDevice, nullptr, &extensionCount, availableExtensions.data());

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