#include "LogicalDevice.h"

#include <mfve_pch.h>

#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
  VkResult LogicalDevice::CreateDevice(
    PhysicalDevice* _physicalDevice, const VkAllocationCallbacks* _allocator)
  {
    if (_physicalDevice == nullptr)
    {
      MFVE_LOG_FATAL("Physical Device is nullptr! Cannot create Logical Device!");
    }

    m_physicalDevice = _physicalDevice;

    std::set<uint32_t> uniqueQueueFamilies = {
      m_physicalDevice->GetQueueFamily().graphicsFamily.value(),
      m_physicalDevice->GetQueueFamily().presentFamily.value()
    };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(uniqueQueueFamilies.size());

    float queuePriority = 1.0f;
    for (const auto& queueFamily : uniqueQueueFamilies)
    {
      queueCreateInfos.emplace_back();
      auto& createInfo = queueCreateInfos.back();

      createInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      createInfo.queueFamilyIndex = queueFamily;
      createInfo.queueCount       = 1;
      createInfo.pQueuePriorities = &queuePriority;
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos    = queueCreateInfos.data();
    createInfo.pEnabledFeatures     = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

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
      MFVE_LOG_ERROR("Physical Device is nullptr, try creating logical device first!");
      return;
    }

    const auto& queueFamily = m_physicalDevice->GetQueueFamily();

    vkGetDeviceQueue(m_device, queueFamily.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, queueFamily.presentFamily.value(), 0, &m_presentQueue);
  }

  void LogicalDevice::Destroy(const VkAllocationCallbacks* _allocator)
  {
    vkDestroyDevice(m_device, _allocator);
    m_physicalDevice = nullptr;
  }

} // namespace MFVE::Vulkan