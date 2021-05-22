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
    explicit ValidationLayers(bool _active) : m_active(_active) {}
    ~ValidationLayers() = default;

    /* Copy Constructors */
    ValidationLayers(const ValidationLayers&) = default;
    ValidationLayers(ValidationLayers&&)      = default;

    /* Assignment Operator */
    ValidationLayers& operator=(const ValidationLayers&) = delete;
    ValidationLayers& operator=(ValidationLayers&&) = delete;

    /* Getters - Active */
    [[nodiscard]] const bool& Active() const { return m_active; }

    /* Getters - Validation Layers */
    [[nodiscard]] size_t LayerCount() const { return LayerVector().size(); }
    [[nodiscard]] const char* const* LayerNames() const { return LayerVector().data(); }
    [[nodiscard]] const std::vector<const char*>& LayerVector() const { return m_validationLayers; }
    [[nodiscard]] std::vector<const char*>& LayerVector() { return m_validationLayers; }

    /* Getters - Extensions */
    [[nodiscard]] const std::vector<const char*>& Extensions() const { return m_extensions; }
    [[nodiscard]] std::vector<const char*>& Extensions() { return m_extensions; }

    /* Layer Support */
    static std::vector<VkLayerProperties> GetSupportedLayers();
    bool
    CheckLayerSupport(const std::vector<VkLayerProperties>& _supportedLayers = GetSupportedLayers());

    /* Debug Messenger */
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
    VkResult CreateDebugMessenger(
      VkInstance _instance, const VkAllocationCallbacks* _pAllocator, void* _userData);
    void DestroyDebugMessenger(VkInstance _instance, const VkAllocationCallbacks* _pAllocator);

   private:
    const bool m_active;

    /* Validation Layers & Extensions*/
    std::vector<const char*> m_validationLayers = { "VK_LAYER_KHRONOS_validation" };
    std::vector<const char*> m_extensions       = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };

    /* Debug Messenger */
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT _messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData, void* _pUserData);
  };
}

#endif // MFVE_VALIDATION_LAYERS_H
