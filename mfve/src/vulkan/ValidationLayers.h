#ifndef MFVE_VALIDATION_LAYERS_H
#define MFVE_VALIDATION_LAYERS_H

#include <array>
#include <vector>

#include "Vk_Base.h"

namespace MFVE::Vulkan::ValidationLayers
{
  /**
   * \brief Get all supported vulkan validation layers
   * \return vector of VkLayerProperties
   */
  static std::vector<VkLayerProperties> GetSupported()
  {
    // Get number of layers
    uint32_t count;
    vkEnumerateInstanceLayerProperties(&count, nullptr);

    // Populate vector
    std::vector<VkLayerProperties> layers(count);
    vkEnumerateInstanceLayerProperties(&count, layers.data());

    return layers;
  }

  /**
   * \brief Check if the validation layers are supported
   * \param _layers Vector of layers to check
   * \param _supportedLayers Vector of layers supported by device (uses
   * ValidationLayers::GetSupported() by default)
   * \return True if they are all supported, false otherwise.
   */
  static bool CheckLayerSupport(
    const std::vector<const char*>& _layers,
    const std::vector<VkLayerProperties>& _supportedLayers = GetSupported())
  {
    return std::all_of(
      _layers.cbegin(),
      _layers.cend(),
      [&](const char* layer)
      {
        for (const auto& supportedLayer : _supportedLayers)
        {
          if (strcmp(layer, supportedLayer.layerName) == 0)
          {
            return true;
          }
        }

        return false;
      });
  }

}

#endif // MFVE_VALIDATION_LAYERS_H
