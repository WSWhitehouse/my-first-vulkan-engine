#ifndef MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILY_INDICIES_H
#define MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILY_INDICIES_H

#include <optional>
#include <set>

#include "Vk_Base.h"

namespace MFVE::Vulkan
{
  struct QueueFamilyIndicies
  {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> transferFamily;

    [[nodiscard]] bool IsComplete() const
    {
      return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
    }
    
    void Reset()
    {
      graphicsFamily.reset();
      presentFamily.reset();
      transferFamily.reset();
    }

    void FindQueueFamilies(VkPhysicalDevice _device, VkSurfaceKHR _surface)
    {
      Reset();

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

      for (unsigned int i = 0; i < queueFamilyCount; ++i)
      {
       const auto& queueFamily = queueFamilies.at(i);

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
          graphicsFamily = i;
        }
        else if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
          transferFamily = i;
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
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILY_INDICIES_H
