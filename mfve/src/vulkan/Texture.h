#ifndef MY_FIRST_VULKAN_ENGINE_TEXTURE_H
#define MY_FIRST_VULKAN_ENGINE_TEXTURE_H

// Vulkan
#include "vulkan/Image.h"
#include "vulkan/Vk_Base.h"

// Filesystem
#include "core/FileSystem.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class CommandPool;
  class Device;

  class Texture
  {
   public:
    Texture()  = default;
    ~Texture() = default;

    bool LoadTexture(const std::filesystem::path& _filePath, const Device& _device,
                     const CommandPool& _graphicsCommandPool,
                     const CommandPool& _transferCommandPool,
                     const VkAllocationCallbacks* _allocator);
    void DestroyTexture(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] VkDescriptorImageInfo GetDescriptorImageInfo() const;
    [[nodiscard]] const Image& GetImage() const { return m_image; }
    [[nodiscard]] VkSampler GetSampler() const { return m_sampler; }

   private:
    Image m_image       = {};
    VkSampler m_sampler = VK_NULL_HANDLE;
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_TEXTURE_H