#include "LogicalDevice.h"

#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
  VkResult LogicalDevice::CreateLogicalDevice(
    PhysicalDevice* _physicalDevice, const VkAllocationCallbacks* _allocator)
  {
    if (_physicalDevice == nullptr)
    {
      MFVE_LOG_FATAL("Physical Device is nullptr! Cannot create Logical Device!");
    }

    m_physicalDevice = _physicalDevice;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_physicalDevice->GetQueueFamily().graphicsFamily.value();
    queueCreateInfo.queueCount       = 1;

    float queuePriority              = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos    = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
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

  void LogicalDevice::DestroyLogicalDevice(const VkAllocationCallbacks* _allocator)
  {
    vkDestroyDevice(m_device, _allocator);
    m_physicalDevice = nullptr;
  }

} // namespace MFVE::Vulkan