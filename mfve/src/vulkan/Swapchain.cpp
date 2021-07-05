#include "Swapchain.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  Swapchain::SupportDetails
  Swapchain::SupportDetails::QuerySupport(VkPhysicalDevice _device, VkSurfaceKHR _surface)
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

  VkSurfaceFormatKHR
  Swapchain::ChooseSurfaceFormat(const Swapchain::SupportDetails& _supportDetails)
  {
    const auto& availableFormats = _supportDetails.formats;

    for (const auto& availableFormat : availableFormats)
    {
      if (
        availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        return availableFormat;
      }
    }
    return availableFormats[0];
  }

  VkPresentModeKHR Swapchain::ChoosePresentMode(const Swapchain::SupportDetails& _supportDetails)
  {
    const auto& availablePresentModes = _supportDetails.presentModes;

    for (const auto& availablePresentMode : availablePresentModes)
    {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D
  Swapchain::ChooseExtent(const Swapchain::SupportDetails& _supportDetails, Window* _window)
  {
    const auto& capabilities = _supportDetails.capabilities;

    if (capabilities.currentExtent.width != UINT32_MAX)
    {
      return capabilities.currentExtent;
    }
    else
    {
      int width, height;
      _window->GetFrameBufferSize(width, height);

      VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

      actualExtent.width = std::clamp(
        actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      actualExtent.height = std::clamp(
        actualExtent.height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height);

      return actualExtent;
    }
  }
  VkResult Swapchain::Create(
    const PhysicalDevice& _physicalDevice, const LogicalDevice& _logicalDevice, Window* _window,
    const VkAllocationCallbacks* _allocator)
  {
    const auto swapchainSupport =
      Swapchain::SupportDetails::QuerySupport(_physicalDevice.GetDevice(), _window->GetSurface());

    auto surfaceFormat = Swapchain::ChooseSurfaceFormat(swapchainSupport);
    auto presentMode   = Swapchain::ChoosePresentMode(swapchainSupport);
    auto extent        = Swapchain::ChooseExtent(swapchainSupport, _window);

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

    if (
      swapchainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapchainSupport.capabilities.maxImageCount)
    {
      imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface          = _window->GetSurface();
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    const auto& queueFamilies     = _physicalDevice.GetQueueFamilies();
    uint32_t queueFamilyIndices[] = { queueFamilies.graphicsFamily.value(),
                                      queueFamilies.presentFamily.value() };

    if (queueFamilies.graphicsFamily != queueFamilies.presentFamily)
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices   = queueFamilyIndices;
    }
    else
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;
      createInfo.pQueueFamilyIndices   = nullptr;
    }

    createInfo.preTransform   = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;

    return vkCreateSwapchainKHR(_logicalDevice.GetDevice(), &createInfo, _allocator, &m_swapchain);
  }

  void
  Swapchain::Destroy(const LogicalDevice& _logicalDevice, const VkAllocationCallbacks* _allocator)
  {
    vkDestroySwapchainKHR(_logicalDevice.GetDevice(), m_swapchain, _allocator);
  }

} // namespace MFVE::Vulkan