#ifndef MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H
#define MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H

// Cpp
#include <vector>

// Vulkan
#include "vulkan/Image.h"
#include "vulkan/Vk_Base.h"

// Core
#include "core/Window.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class Device;
  struct SupportDetails;

  class Swapchain
  {
   public:
    Swapchain()  = default;
    ~Swapchain() = default;

    void CreateSwapchain(const Device& _device, Window* _window,
                             const VkAllocationCallbacks* _allocator);
    void DestroySwapchain(const Device& _device, const VkAllocationCallbacks* _allocator);

    /* Getters */
    [[nodiscard]] VkSwapchainKHR GetSwapchain() const { return m_swapchain; }
    [[nodiscard]] const VkSurfaceFormatKHR& GetSurfaceFormat() const { return m_surfaceFormat; }
    [[nodiscard]] const VkPresentModeKHR& GetPresentMode() const { return m_presentMode; }
    [[nodiscard]] const VkExtent2D& GetExtent() const { return m_extent; }
    [[nodiscard]] const std::vector<Image>& GetImages() const { return m_swapchainImages; }

   private:
    // Swapchain
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

    VkSurfaceFormatKHR m_surfaceFormat;
    VkPresentModeKHR m_presentMode;
    VkExtent2D m_extent;

    void ChooseSurfaceFormat(const SupportDetails& _supportDetails);
    void ChoosePresentMode(const SupportDetails& _supportDetails);
    void ChooseExtent(const SupportDetails& _supportDetails, Window* _window);

    // Swapchain Images
    std::vector<Image> m_swapchainImages;
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_SWAPCHAIN_H
