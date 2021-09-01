#include "vulkan/Image.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Buffer.h"
#include "vulkan/CommandPool.h"
#include "vulkan/Device.h"

namespace MFVE::Vulkan
{
  void Image::CreateImage(const Device& _device, const uint32_t& _width, const uint32_t& _height,
                          const VkFormat& _format, const VkImageTiling& _tiling,
                          const VkImageUsageFlags& _usage, const VkMemoryPropertyFlags& _properties,
                          const VkAllocationCallbacks* _allocator)
  {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType     = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width  = _width;
    imageInfo.extent.height = _height;
    imageInfo.extent.depth  = 1;
    imageInfo.mipLevels     = 1;
    imageInfo.arrayLayers   = 1;
    imageInfo.format        = _format;
    imageInfo.tiling        = _tiling;
    imageInfo.initialLayout = m_imageLayout;
    imageInfo.usage         = _usage;
    imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags         = 0;

    std::set<uint32_t> queueFamilyIndiciesSet = _device.GetUniqueQueueFamilyIndicies();
    std::vector<uint32_t> indicies(queueFamilyIndiciesSet.begin(), queueFamilyIndiciesSet.end());

    if (queueFamilyIndiciesSet.size() > 1)
    {
      imageInfo.sharingMode           = VK_SHARING_MODE_CONCURRENT;
      imageInfo.queueFamilyIndexCount = static_cast<uint32_t>(indicies.size());
      imageInfo.pQueueFamilyIndices   = indicies.data();
    }
    else
    {
      imageInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
      imageInfo.queueFamilyIndexCount = 0;
      imageInfo.pQueueFamilyIndices   = nullptr;
    }

    VkCheck(vkCreateImage(_device.GetDevice(), &imageInfo, nullptr, &m_image));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device.GetDevice(), m_image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = _device.FindMemoryType(memRequirements.memoryTypeBits, _properties);

    VkCheck(vkAllocateMemory(_device.GetDevice(), &allocInfo, nullptr, &m_imageMemory));

    vkBindImageMemory(_device.GetDevice(), m_image, m_imageMemory, 0);
  }

  void Image::DestroyImage(const Device& _device, const VkAllocationCallbacks* _allocator)
  {
    vkDestroyImage(_device.GetDevice(), m_image, _allocator);
    vkFreeMemory(_device.GetDevice(), m_imageMemory, _allocator);
  }

  void Image::SetImageHandle(VkImage _image) { m_image = _image; }
  void Image::ReleaseImageHandle() { m_image = VK_NULL_HANDLE; }

  void Image::CreateImageView(const Device& _device, const VkFormat& _format,
                              const VkAllocationCallbacks* _allocator)
  {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image    = m_image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format   = _format;

    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;

    VkCheck(vkCreateImageView(_device.GetDevice(), &viewInfo, _allocator, &m_imageView));
  }

  void Image::DestroyImageView(const Device& _device, const VkAllocationCallbacks* _allocator)
  {
    vkDestroyImageView(_device.GetDevice(), m_imageView, _allocator);
  }

  void Image::CopyFromBuffer(const Device& _device, const CommandPool& _transferCommandPool,
                             const Buffer& _srcBuffer, const uint32_t& _width,
                             const uint32_t& _height)
  {
    auto commandBuffer = _transferCommandPool.BeginSingleTimeCommands(_device);

    VkBufferImageCopy region{};
    region.bufferOffset      = 0;
    region.bufferRowLength   = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel       = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount     = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { _width, _height, 1 };

    vkCmdCopyBufferToImage(commandBuffer,
                           _srcBuffer.GetBuffer(),
                           m_image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &region);

    _transferCommandPool.EndSingleTimeCommands(_device, commandBuffer);
  }

  void Image::SetImageLayout(const Device& _device, const CommandPool& _graphicsCommandPool,
                             const VkFormat& _format, const VkImageLayout& _layout)
  {
    auto commandBuffer = _graphicsCommandPool.BeginSingleTimeCommands(_device);

    const auto oldLayout = m_imageLayout;

    VkImageMemoryBarrier barrier{};
    barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout                       = oldLayout;
    barrier.newLayout                       = _layout;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                           = m_image;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             _layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
      MFVE_LOG_FATAL("Unsupported Layout Transition: " + _layout);
    }

    vkCmdPipelineBarrier(
      commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    _graphicsCommandPool.EndSingleTimeCommands(_device, commandBuffer);

    m_imageLayout = _layout;
  }

} // namespace MFVE::Vulkan
