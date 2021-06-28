#include "Extensions.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  bool Extensions::AreExtensionsSupported(
    const std::vector<VkExtensionProperties>& _supportedExtensions) const
  {
    // Return false if empty
    if (_supportedExtensions.empty())
    {
      return false;
    }

    return std::all_of(
      m_extensions.cbegin(),
      m_extensions.cend(),
      [&](const std::string& extension)
      {
        for (const auto& ext : _supportedExtensions)
        {
          if (strcmp(extension.c_str(), ext.extensionName) == 0)
          {
            return true;
          }
        }

        return false;
      });
  }

  void Extensions::Add(std::string_view _extension) { m_extensions.emplace_back(_extension.data()); }

  void Extensions::Add(size_t _extensionCount, const char** _extensions)
  {
    m_extensions.reserve(_extensionCount);
    for (int i = 0; i < _extensionCount; ++i)
    {
      Add(_extensions[i]);
    }
  }

}