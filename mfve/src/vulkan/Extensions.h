#ifndef MFVE_EXTENSIONS_H
#define MFVE_EXTENSIONS_H

#include "Vk_Base.h"

namespace MFVE::Vulkan::Extensions
{
  /**
   * \brief Get all supported vulkan extensions
   * \return vector of VkExtensionProperties
   */
  static inline std::vector<VkExtensionProperties> GetSupported()
  {
    // Get number of extensions
    uint32_t count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

    // Populate vector
    std::vector<VkExtensionProperties> extensions(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

    return extensions;
  }

  /**
   * \brief Check if the extensions are supported
   * \param _extensions Vector of extensions to check
   * \param _supportedExtensions Vector of extensions supported by device (uses
   * Extensions::GetSupported() by default)
   * \return True if they are all supported, false otherwise.
   */
  static bool CheckExtensionSupport(
    const std::vector<const char*>& _extensions,
    const std::vector<VkExtensionProperties>& _supportedExtensions = GetSupported())
  {
    return std::all_of(
      _extensions.cbegin(),
      _extensions.cend(),
      [&](const char* extension)
      {
        for (const auto& ext : _supportedExtensions)
        {
          if (strcmp(extension, ext.extensionName) == 0)
          {
            return true;
          }
        }

        return false;
      });
  }
} // namespace MFVE::Vulkan::Extensions

#endif // MFVE_EXTENSIONS_H
