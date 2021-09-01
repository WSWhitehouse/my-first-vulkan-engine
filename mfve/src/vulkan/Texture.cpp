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

    Buffer stagingBuffer;

    stagingBuffer.CreateBuffer(_device,
                               imageSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               _allocator);

    stagingBuffer.FillBuffer(_device, pixels, imageSize, 0, 0);

    stbi_image_free(pixels);

    m_textureImage.CreateImage(_device,
                               static_cast<uint32_t>(texWidth),
                               static_cast<uint32_t>(texHeight),
                               VK_FORMAT_R8G8B8A8_SRGB,
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               _allocator);

    m_textureImage.SetImageLayout(
      _device, _graphicsCommandPool, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    m_textureImage.CopyFromBuffer(_device,
                                  _transferCommandPool,
                                  stagingBuffer,
                                  static_cast<uint32_t>(texWidth),
                                  static_cast<uint32_t>(texHeight));

    m_textureImage.SetImageLayout(_device,
                                  _graphicsCommandPool,
                                  VK_FORMAT_R8G8B8A8_SRGB,
                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    stagingBuffer.DestroyBuffer(_device, _allocator);

    return true;
  }

  void Texture::DestroyTexture(const Device& _device, const VkAllocationCallbacks* _allocator)
  {
    m_textureImage.DestroyImage(_device, _allocator);
  }
} // namespace MFVE::Vulkan