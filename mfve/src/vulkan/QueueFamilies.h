#ifndef MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILIES_H
#define MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILIES_H

#include <optional>

#include "Vk_Base.h"

struct QueueFamilies
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  [[nodiscard]] bool IsComplete() const
  {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }

  void Reset()
  {
    graphicsFamily.reset();
    presentFamily.reset();
  }

  void FindQueueFamilies(VkPhysicalDevice _device, VkSurfaceKHR _surface)
  {
    Reset();

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

    for (int i = 0; i < queueFamilyCount; ++i)
    {
      auto queueFamily = queueFamilies.at(i);

      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        graphicsFamily = i;
      }

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, _surface, &presentSupport);

      if (presentSupport)
      {
        presentFamily = i;
      }

      if (IsComplete())
      {
        break;
      }
    }
  }
};

#endif // MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILIES_H
