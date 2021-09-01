#include "vulkan/Texture.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Buffer.h"
#include "vulkan/Device.h"
#include "vulkan/Image.h"

// stb
#include "stb_image.h"

namespace MFVE::Vulkan
{
  bool Texture::LoadTexture(const std::filesystem::path& _filePath, const Device& _device,
                            const CommandPool& _graphicsCommandPool,
                            const CommandPool& _transferCommandPool,
                            const VkAllocationCallbacks* _allocator)
  {
    // Load texture from path
    const auto filePath = FileSystem::GetAssetPathFromRelativePath(_filePath);

    int texWidth, texHeight, texChannels;

    stbi_uc* pixels =
      stbi_load(filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
      MFVE_LOG_ERROR("Failed to load texture: " + _filePath.string());
      return false;
    }

    // Fill a staging buffer with texture data
    Buffer stagingBuffer;

    stagingBuffer.CreateBuffer(_device,
                               imageSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               _allocator);

    stagingBuffer.FillBuffer(_device, pixels, imageSize, 0, 0);

    stbi_image_free(pixels);

    // Create VkImage and copy texture data from staging buffer
    m_image.CreateImage(_device,
                        static_cast<uint32_t>(texWidth),
                        static_cast<uint32_t>(texHeight),
                        VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        _allocator);

    m_image.SetImageLayout(
      _device, _graphicsCommandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    m_image.CopyFromBuffer(_device,
                           _transferCommandPool,
                           stagingBuffer,
                           static_cast<uint32_t>(texWidth),
                           static_cast<uint32_t>(texHeight));

    m_image.SetImageLayout(_device,
                           _graphicsCommandPool,
                           VK_FORMAT_R8G8B8A8_SRGB,
                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    stagingBuffer.DestroyBuffer(_device, _allocator);

    // Create Image view
    m_image.CreateImageView(_device, VK_FORMAT_R8G8B8A8_SRGB, _allocator);

    // Create Sampler
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType     = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = _device.GetPhysicalDeviceProperties().limits.maxSamplerAnisotropy;

    samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp     = VK_COMPARE_OP_ALWAYS;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod     = 0.0f;
    samplerInfo.maxLod     = 0.0f;

    VkCheck(vkCreateSampler(_device.GetDevice(), &samplerInfo, _allocator, &m_sampler));

    return true;
  }

  void Texture::DestroyTexture(const Device& _device, const VkAllocationCallbacks* _allocator)
  {
    vkDestroySampler(_device.GetDevice(), m_sampler, _allocator);
    m_image.DestroyImageView(_device, _allocator);
    m_image.DestroyImage(_device, _allocator);
  }
} // namespace MFVE::Vulkan