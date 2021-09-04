#ifndef MY_FIRST_VULKAN_ENGINE_IMAGE_H
#define MY_FIRST_VULKAN_ENGINE_IMAGE_H

// Vulkan
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class Buffer;
  class CommandPool;
  class Device;

  class Image
  {
   public:
    Image()  = default;
    ~Image() = default;

    // Image
    void CreateImage(const Device& _device, const uint32_t& _width, const uint32_t& _height,
                     const VkFormat& _format, const VkImageTiling& _tiling,
                     const VkImageUsageFlags& _usage, const VkMemoryPropertyFlags& _properties,
                     const VkAllocationCallbacks* _allocator);

    void DestroyImage(const Device& _device, const VkAllocationCallbacks* _allocator);

    /**
     * \brief Sets image handle. Does NOT create image.
     */
    void SetImageHandle(VkImage _image);

    /**
     * \brief Releases image handle. Does NOT destroy image.
     */
    void ReleaseImageHandle();

    // Image View
    void CreateImageView(const Device& _device, const VkFormat& _format,
                         const VkImageAspectFlags& _aspectFlags,
                         const VkAllocationCallbacks* _allocator);
    void DestroyImageView(const Device& _device, const VkAllocationCallbacks* _allocator);

    // Util
    void CopyFromBuffer(const Device& _device, const CommandPool& _transferCommandPool,
                        const Buffer& _srcBuffer, const uint32_t& _width, const uint32_t& _height);

    // Getters
    [[nodiscard]] VkImage GetImage() const { return m_image; }
    [[nodiscard]] VkImageView GetImageView() const { return m_imageView; }
    [[nodiscard]] VkDeviceMemory GetImageMemory() const { return m_imageMemory; }
    [[nodiscard]] const VkImageLayout& GetImageLayout() const { return m_imageLayout; }

    // Setters
    void SetImageLayout(const Device& _device, const CommandPool& _graphicsCommandPool,
                        const VkFormat& _format, const VkImageLayout& _layout);

   private:
    VkImage m_image              = VK_NULL_HANDLE;
    VkImageView m_imageView      = VK_NULL_HANDLE;
    VkDeviceMemory m_imageMemory = VK_NULL_HANDLE;
    VkImageLayout m_imageLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_IMAGE_H