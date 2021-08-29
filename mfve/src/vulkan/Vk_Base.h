#ifndef MY_FIRST_VULKAN_ENGINE_VK_BASE_H
#define MY_FIRST_VULKAN_ENGINE_VK_BASE_H

#include <mfve_pch.h>
#include <stdexcept>

// Vulkan
#include <vulkan/vulkan.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MFVE::Vulkan
{
  /**
   * \brief Check a VkResult is successful
   */
  static constexpr inline void VkCheck(VkResult _result)
  {
    if (_result != VK_SUCCESS)
    {
      MFVE_LOG_FATAL("VK CHECK FAILED: " + std::to_string(_result));
    }
  }
}

#endif // MY_FIRST_VULKAN_ENGINE_VK_BASE_H
