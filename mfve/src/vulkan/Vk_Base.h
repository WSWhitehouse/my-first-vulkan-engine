#ifndef MFVE_VK_BASE_H
#define MFVE_VK_BASE_H

#include <stdexcept>

// Vulkan
#include <vulkan/vulkan.h>

namespace MFVE
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
}

#endif // MFVE_VK_BASE_H
