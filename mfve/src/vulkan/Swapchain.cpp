#include "Swapchain.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  Swapchain::SupportDetails Swapchain::QuerySupport(VkPhysicalDevice _device, VkSurfaceKHR _surface)
  {
    SupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, _surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &formatCount, details.formats.data());
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
} // namespace MFVE::Vulkan