#include "LogicalDevice.h"

#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
  LogicalDevice::LogicalDevice(PhysicalDevice& _physicalDevice) : m_physicalDevice(_physicalDevice)
  {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_physicalDevice.GetQueueFamily().graphicsFamily.value();
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

    VkCheck(vkCreateDevice(_physicalDevice.GetDevice(), &createInfo, nullptr, &m_device));
  }

  LogicalDevice::~LogicalDevice() { vkDestroyDevice(m_device, nullptr); }

} // namespace MFVE::Vulkan