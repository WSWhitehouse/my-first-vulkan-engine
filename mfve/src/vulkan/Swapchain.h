#ifndef MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H
#define MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H

#include "PhysicalDevice.h"
#include "Vk_Base.h"

namespace MFVE::Vulkan
{
  class Swapchain
  {
   public:
    struct SupportDetails
    {
      VkSurfaceCapabilitiesKHR capabilities{};
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
    };

    static SupportDetails QuerySupport(VkPhysicalDevice _device, VkSurfaceKHR _surface);
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H
