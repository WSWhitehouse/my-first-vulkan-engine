#ifndef WIPE_VK_BASE_H
#define WIPE_VK_BASE_H

// C++ Standard Lib
#include <string>
#include <vector>

// Vulkan
#include <vulkan/vulkan.h>

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

static constexpr void VkCheck(VkResult _result)
{
  if (_result != VK_SUCCESS)
  {
    throw std::runtime_error("VK ERROR: " + std::to_string(_result));
  }
}

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

static void LogSupportedExtensions(const std::vector<VkExtensionProperties>& _extensions = GetSupportedExtensions())
{
  std::cout << _extensions.size() << " extensions supported.\n";
  std::cout << "Available Extensions:\n";
  for (const auto& extension : _extensions)
  {
    std::cout << '\t' << extension.extensionName << '\n';
  }
}

#endif // WIPE_VK_BASE_H
