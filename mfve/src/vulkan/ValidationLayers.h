#ifndef MFVE_VALIDATION_LAYERS_H
#define MFVE_VALIDATION_LAYERS_H

#include <array>
#include <vector>

#include "Vk_Base.h"

namespace MFVE
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

   public: /* Getters */
    static bool Active()
    {
#ifdef NDEBUG
      return false;
#else
      return true;
#endif
    }

    static size_t LayerCount() { return m_validationLayers.size(); }
    static const char* const* LayerNames() { return m_validationLayers.data(); }

    static size_t ExtensionCount() { return m_extensions.size(); }
    static const char* const* ExtensionNames() { return m_extensions.data(); }

    /* Layer Support */
    static std::vector<VkLayerProperties> GetSupportedLayers();
    static bool
    CheckLayerSupport(const std::vector<VkLayerProperties>& _layers = GetSupportedLayers());

    /* Debug Messenger */
    static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
    static VkResult InitDebugMessenger(
      VkInstance _instance, const VkAllocationCallbacks* _pAllocator, void* _userData);
    static void
    DestroyDebugMessenger(VkInstance _instance, const VkAllocationCallbacks* _pAllocator);

   private:
    /* Requested validation layers */
    static inline const std::array<const char*, 1> m_validationLayers = {
      "VK_LAYER_KHRONOS_validation"
    };

    /* Requested validation extensions */
    static inline const std::array<const char*, 1> m_extensions = {
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

    /* Debug Messenger */
    static inline VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT _messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData, void* _pUserData);
  };
}

#endif // MFVE_VALIDATION_LAYERS_H
