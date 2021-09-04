#include "Swapchain.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Device.h"
#include "vulkan/SupportDetails.h"

namespace MFVE::Vulkan
{
  void Swapchain::CreateSwapchain(const Device& _device, Window* _window,
                                  const VkAllocationCallbacks* _allocator)
  {
    const auto supportDetails =
      SupportDetails::QuerySupport(_device.GetPhysicalDevice(), _window->GetSurface());

    ChooseSurfaceFormat(supportDetails);
    ChoosePresentMode(supportDetails);
    ChooseExtent(supportDetails, _window);

    uint32_t imageCount = supportDetails.capabilities.minImageCount + 1;

    if (supportDetails.capabilities.maxImageCount > 0 &&
        imageCount > supportDetails.capabilities.maxImageCount)
    {
      imageCount = supportDetails.capabilities.maxImageCount;
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

    std::set<uint32_t> queueFamilyIndiciesSet = _device.GetUniqueQueueFamilyIndicies();
    std::vector<uint32_t> indicies(queueFamilyIndiciesSet.begin(), queueFamilyIndiciesSet.end());

    if (queueFamilyIndiciesSet.size() > 1)
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = static_cast<uint32_t>(indicies.size());
      createInfo.pQueueFamilyIndices   = indicies.data();
    }
    else
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;
      createInfo.pQueueFamilyIndices   = nullptr;
    }

    createInfo.preTransform   = supportDetails.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = m_presentMode;
    createInfo.clipped        = VK_TRUE;

    VkCheck(vkCreateSwapchainKHR(_device.GetDevice(), &createInfo, _allocator, &m_swapchain));

    imageCount = 0;
    vkGetSwapchainImagesKHR(_device.GetDevice(), m_swapchain, &imageCount, nullptr);

    std::vector<VkImage> images(imageCount);
    vkGetSwapchainImagesKHR(_device.GetDevice(), m_swapchain, &imageCount, images.data());

    m_swapchainImages.resize(imageCount);
    const auto& format = GetSurfaceFormat().format;

    for (size_t i = 0; i < imageCount; i++)
    {
      auto& swapchainImage = m_swapchainImages[i];

      swapchainImage.SetImageHandle(images[i]);
      swapchainImage.CreateImageView(_device, format, VK_IMAGE_ASPECT_COLOR_BIT, _allocator);
    }
  }

  void Swapchain::DestroySwapchain(const Device& _device, const VkAllocationCallbacks* _allocator)
  {
    for (auto& image : m_swapchainImages)
    {
      image.DestroyImageView(_device, _allocator);
      image.ReleaseImageHandle();
    }

    m_swapchainImages.clear();

    vkDestroySwapchainKHR(_device.GetDevice(), m_swapchain, _allocator);
  }

  void Swapchain::ChooseSurfaceFormat(const SupportDetails& _supportDetails)
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

  void Swapchain::ChoosePresentMode(const SupportDetails& _supportDetails)
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

  void Swapchain::ChooseExtent(const SupportDetails& _supportDetails, Window* _window)
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
