#ifndef MFVE_VK_BASE_H
#define MFVE_VK_BASE_H

#include <mfve_pch.h>
#include <stdexcept>

// Vulkan
#include <vulkan/vulkan.h>

namespace MFVE
{
  /**
   * \brief Check a VkResult is successful
   */
  static constexpr inline void VkCheck(VkResult _result)
  {
    if (_result != VK_SUCCESS)
    {
      MFVE_LOG_FATAL("VK RUNTIME ERROR: " + std::to_string(_result));
      throw std::runtime_error("VK ERROR: " + std::to_string(_result));
    }
  }
}

#endif // MFVE_VK_BASE_H
