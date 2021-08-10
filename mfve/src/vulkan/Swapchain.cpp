#include "Swapchain.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  Swapchain::SupportDetails Swapchain::SupportDetails::QuerySupport(VkPhysicalDevice _device,
                                                                    VkSurfaceKHR _surface)
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

  VkResult Swapchain::CreateSwapchain(const PhysicalDevice& _physicalDevice,
                                      const LogicalDevice& _logicalDevice, Window* _window,
                                      const VkAllocationCallbacks* _allocator)
  {
    const auto swapchainSupport =
      Swapchain::SupportDetails::QuerySupport(_physicalDevice.GetDevice(), _window->GetSurface());

    ChooseSurfaceFormat(swapchainSupport);
    ChoosePresentMode(swapchainSupport);
    ChooseExtent(swapchainSupport, _window);

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

    if (swapchainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapchainSupport.capabilities.maxImageCount)
    {
      imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface          = _window->GetSurface();
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = m_surfaceFormat.format;
    createInfo.imageColorSpace  = m_surfaceFormat.colorSpace;
    createInfo.imageExtent      = m_extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    std::set<uint32_t> queueFamilyIndiciesSet = _logicalDevice.GetUniqueQueueFamilyIndicies();
    std::vector<uint32_t> indicies(queueFamilyIndiciesSet.begin(), queueFamilyIndiciesSet.end());

    if (queueFamilyIndiciesSet.size() > 1)
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = indicies.size();
      createInfo.pQueueFamilyIndices   = indicies.data();
    }
    else
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;
      createInfo.pQueueFamilyIndices   = nullptr;
    }

    createInfo.preTransform   = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = m_presentMode;
    createInfo.clipped        = VK_TRUE;

    return vkCreateSwapchainKHR(_logicalDevice.GetDevice(), &createInfo, _allocator, &m_swapchain);
  }

  void Swapchain::DestroySwapchain(const LogicalDevice& _logicalDevice,
                                   const VkAllocationCallbacks* _allocator)
  {
    vkDestroySwapchainKHR(_logicalDevice.GetDevice(), m_swapchain, _allocator);
    m_swapchainImages.clear();
  }

  VkResult Swapchain::CreateImageViews(const LogicalDevice& _logicalDevice,
                                       const VkAllocationCallbacks* _allocator)
  {
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(_logicalDevice.GetDevice(), m_swapchain, &imageCount, nullptr);
    m_swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(
      _logicalDevice.GetDevice(), m_swapchain, &imageCount, m_swapchainImages.data());

    m_swapchainImageViews.resize(m_swapchainImages.size());

    for (size_t i = 0; i < m_swapchainImages.size(); i++)
    {
      VkImageViewCreateInfo createInfo{};
      createInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image    = m_swapchainImages[i];
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format   = GetSurfaceFormat().format;

      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

      createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel   = 0;
      createInfo.subresourceRange.levelCount     = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount     = 1;

      auto result = vkCreateImageView(
        _logicalDevice.GetDevice(), &createInfo, _allocator, &m_swapchainImageViews[i]);
      if (result != VK_SUCCESS)
      {
        return result;
      }
    }

    return VK_SUCCESS;
  }

  void Swapchain::DestroyImageViews(const LogicalDevice& _logicalDevice,
                                    const VkAllocationCallbacks* _allocator)
  {
    for (auto imageView : m_swapchainImageViews)
    {
      vkDestroyImageView(_logicalDevice.GetDevice(), imageView, _allocator);
    }
  }

  void Swapchain::ChooseSurfaceFormat(const Swapchain::SupportDetails& _supportDetails)
  {
    const auto& availableFormats = _supportDetails.formats;

    for (const auto& availableFormat : availableFormats)
    {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        m_surfaceFormat = availableFormat;
      }
    }
    m_surfaceFormat = availableFormats[0];
  }

  void Swapchain::ChoosePresentMode(const Swapchain::SupportDetails& _supportDetails)
  {
    const auto& availablePresentModes = _supportDetails.presentModes;

    for (const auto& availablePresentMode : availablePresentModes)
    {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      {
        m_presentMode = availablePresentMode;
      }
    }

    m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
  }

  void Swapchain::ChooseExtent(const Swapchain::SupportDetails& _supportDetails, Window* _window)
  {
    const auto& capabilities = _supportDetails.capabilities;

    if (capabilities.currentExtent.width != UINT32_MAX)
    {
      m_extent = capabilities.currentExtent;
    }
    else
    {
      int width, height;
      _window->GetFrameBufferSize(width, height);

      VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

      actualExtent.width = std::clamp(
        actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      actualExtent.height = std::clamp(actualExtent.height,
                                       capabilities.minImageExtent.height,
                                       capabilities.maxImageExtent.height);

      m_extent = actualExtent;
    }
  }

} // namespace MFVE::Vulkan