#ifndef WIPE_VALIDATION_LAYERS_H
#define WIPE_VALIDATION_LAYERS_H

#include "Vk_Base.h"

namespace WIPE
{
  class ValidationLayers
  {
   public:
    /* Constructor & Destructor */
    ValidationLayers()  = delete;
    ~ValidationLayers() = delete;

    /* Copy Constructors */
    ValidationLayers(const ValidationLayers&) = delete;
    ValidationLayers(ValidationLayers&&)      = delete;

    /* Assignment Operator */
    ValidationLayers& operator=(const ValidationLayers&) = delete;
    ValidationLayers& operator=(ValidationLayers&&) = delete;

    /* Getters */
    static const bool& Active() { return ValidationLayersActive; }
    static uint32_t LayerCount() { return static_cast<uint32_t>(m_validationLayers.size()); }
    static const char* const* LayerNames() { return m_validationLayers.data(); }

    static inline bool CheckLayerSupport()
    {
      // Get number of layers
      uint32_t count;
      vkEnumerateInstanceLayerProperties(&count, nullptr);

      // Populate vector
      std::vector<VkLayerProperties> layers(count);
      vkEnumerateInstanceLayerProperties(&count, layers.data());

      // Check all requested validation layers are supported
      for (const char* layerName : m_validationLayers)
      {
        bool layerFound = false;

        for (const auto& layerProperties : layers)
        {
          if (strcmp(layerName, layerProperties.layerName) == 0)
          {
            layerFound = true;
            break;
          }
        }

        if (!layerFound)
        {
          return false;
        }
      }

      return true;
    }

   private:
#ifdef NDEBUG
    static inline const bool ValidationLayersActive = false;
#else
    static inline const bool ValidationLayersActive = true;
#endif

    static inline const std::vector<const char*> m_validationLayers = {
      "VK_LAYER_KHRONOS_validation"
    };
  };
}

#endif // WIPE_VALIDATION_LAYERS_H
