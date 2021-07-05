#ifndef MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H
#define MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H

#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Vk_Base.h"
#include "core/Window.h"

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

      static SupportDetails QuerySupport(VkPhysicalDevice _device, VkSurfaceKHR _surface);
    };

    static VkSurfaceFormatKHR ChooseSurfaceFormat(const SupportDetails& _supportDetails);
    static VkPresentModeKHR ChoosePresentMode(const SupportDetails& _supportDetails);
    static VkExtent2D ChooseExtent(const SupportDetails& _supportDetails, Window* _window);

    VkResult Create(
      const PhysicalDevice& _physicalDevice, const LogicalDevice& _logicalDevice, Window* _window,
      const VkAllocationCallbacks* _allocator);

    void Destroy(const LogicalDevice& _logicalDevice, const VkAllocationCallbacks* _allocator);

    /* Getters */
    [[nodiscard]] VkSwapchainKHR GetSwapchain() const { return m_swapchain; }

   private:
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H
