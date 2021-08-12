#ifndef MY_FIRST_VULKAN_ENGINE_SUPPORT_DETAILS_H
#define MY_FIRST_VULKAN_ENGINE_SUPPORT_DETAILS_H

// Vulkan
#include "Vk_Base.h"

namespace MFVE::Vulkan
{
  struct SupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    static inline SupportDetails QuerySupport(VkPhysicalDevice _device, VkSurfaceKHR _surface)
    {
      SupportDetails details;

      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, _surface, &details.capabilities);

      uint32_t formatCount;
      vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &formatCount, nullptr);

      if (formatCount != 0)
      {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
          _device, _surface, &formatCount, details.formats.data());
      }

      uint32_t presentModeCount;
      vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface, &presentModeCount, nullptr);

      if (presentModeCount != 0)
      {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
          _device, _surface, &presentModeCount, details.presentModes.data());
      }

      return details;
    }
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_SUPPORT_DETAILS_H
