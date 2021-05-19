#ifndef WIPE_VK_BASE_H
#define WIPE_VK_BASE_H

#include <array>
#include <cstring>
#include <vector>

// Vulkan
#include <vulkan/vulkan.h>

namespace WIPE
{
  /**
   * \brief Check a VkResult is successful
   */
  static constexpr void VkCheck(VkResult _result)
  {
    if (_result != VK_SUCCESS)
    {
      throw std::runtime_error("VK ERROR: " + std::to_string(_result));
    }
  }

  /* Extensions */

  /**
   * \brief Get all supported vulkan extensions
   * \return vector of VkExtensionProperties
   */
  static std::vector<VkExtensionProperties> GetSupportedExtensions()
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
   * \brief Log supported vulkan extensions
   * \param _extensions - extensions to log (equals GetSupportedExtensions() by default)
   */
  static void LogSupportedExtensions(
    const std::vector<VkExtensionProperties>& _extensions = GetSupportedExtensions())
  {
    std::cout << _extensions.size() << " extensions supported.\n";
    std::cout << "Available Extensions:\n";
    for (const auto& extension : _extensions)
    {
      std::cout << '\t' << extension.extensionName << '\n';
    }
  }
}

#endif // WIPE_VK_BASE_H
