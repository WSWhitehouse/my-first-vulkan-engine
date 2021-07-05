#include "LogicalDevice.h"

#include <mfve_pch.h>

#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
  VkResult LogicalDevice::CreateDevice(PhysicalDevice* _physicalDevice,
                                       const VkAllocationCallbacks* _allocator)
  {
    if (_physicalDevice == nullptr)
    {
      MFVE_LOG_FATAL("Physical Device is nullptr! Cannot create Logical Device!");
    }

    m_physicalDevice = _physicalDevice;

    std::set<uint32_t> uniqueQueueFamilies = {
      m_physicalDevice->GetQueueFamilies().graphicsFamily.value(),
      m_physicalDevice->GetQueueFamilies().presentFamily.value()
    };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

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

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos    = queueCreateInfos.data();
    createInfo.pEnabledFeatures     = &deviceFeatures;

    createInfo.enabledExtensionCount   = m_physicalDevice->GetDeviceExtensions().size();
    createInfo.ppEnabledExtensionNames = m_physicalDevice->GetDeviceExtensions().data();

    if (ValidationLayers::Enabled())
    {
      createInfo.enabledLayerCount   = ValidationLayers::Layers.size();
      createInfo.ppEnabledLayerNames = ValidationLayers::Layers.data();
    }
    else
    {
      createInfo.enabledLayerCount = 0;
    }

    return vkCreateDevice(m_physicalDevice->GetDevice(), &createInfo, _allocator, &m_device);
  }

  void LogicalDevice::CreateQueueHandles()
  {
    if (m_physicalDevice == nullptr)
    {
      MFVE_LOG_ERROR(
        "Cannot create queue handles because Physical Device is nullptr, try creating the logical device first!");
      return;
    }

    const auto& queueFamily = m_physicalDevice->GetQueueFamilies();
    vkGetDeviceQueue(m_device, queueFamily.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, queueFamily.presentFamily.value(), 0, &m_presentQueue);
  }

  void LogicalDevice::Destroy(const VkAllocationCallbacks* _allocator)
  {
    vkDestroyDevice(m_device, _allocator);
    m_physicalDevice = nullptr;
    m_graphicsQueue  = VK_NULL_HANDLE;
    m_presentQueue   = VK_NULL_HANDLE;
  }

} // namespace MFVE::Vulkan