#include "LogicalDevice.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Extensions.h"
#include "vulkan/PhysicalDevice.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
  void LogicalDevice::CreateDevice(const PhysicalDevice& _physicalDevice,
                                   const VkAllocationCallbacks* _allocator)
  {
    const auto& queueFamily = _physicalDevice.GetQueueFamilies();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::set<uint32_t> uniqueQueueFamilies = { queueFamily.graphicsFamily.value(),
                                               queueFamily.presentFamily.value(),
                                               queueFamily.transferFamily.value() };

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

    createInfo.enabledExtensionCount   = _physicalDevice.GetDeviceExtensions().size();
    createInfo.ppEnabledExtensionNames = _physicalDevice.GetDeviceExtensions().data();

    if (ValidationLayers::Enabled())
    {
      createInfo.enabledLayerCount   = ValidationLayers::Layers.size();
      createInfo.ppEnabledLayerNames = ValidationLayers::Layers.data();
    }
    else
    {
      createInfo.enabledLayerCount = 0;
    }

    VkCheck(vkCreateDevice(_physicalDevice.GetDevice(), &createInfo, _allocator, &m_device));

    m_graphicsQueue.CreateHandle(m_device, queueFamily.graphicsFamily.value(), 0);
    m_presentQueue.CreateHandle(m_device, queueFamily.presentFamily.value(), 0);
    m_transferQueue.CreateHandle(m_device, queueFamily.transferFamily.value(), 0);
  }

  void LogicalDevice::Destroy(const VkAllocationCallbacks* _allocator)
  {
    vkDestroyDevice(m_device, _allocator);
  }

} // namespace MFVE::Vulkan