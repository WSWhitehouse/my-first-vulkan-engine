#ifndef MY_FIRST_VULKAN_ENGINE_VALIDATION_LAYERS_H
#define MY_FIRST_VULKAN_ENGINE_VALIDATION_LAYERS_H

#include <array>
#include <vector>

#include "vulkan/Instance.h"
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan::ValidationLayers
{
  /**
   * \brief Are validation layers enabled
   */
  static inline bool Enabled()
  {
#ifdef MFVE_ENABLE_VK_VALIDATION
    return true;
#else
    return false;
#endif
  }

  static const std::array<const char*, 1> Layers = { "VK_LAYER_KHRONOS_validation" };

  /* Supported Layers*/

  /**
   * \brief Get all supported vulkan validation layers
   * \return vector of VkLayerProperties
   */
  static inline std::vector<VkLayerProperties> GetSupported()
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
   * \param _supportedLayers Vector of layers supported by device (uses
   * ValidationLayers::GetSupported() by default)
   * \return True if they are all supported, false otherwise.
   */
  static inline bool
  CheckLayerSupport(const std::vector<VkLayerProperties>& _supportedLayers = GetSupported())
  {
    return std::all_of(Layers.cbegin(),
                       Layers.cend(),
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

  /* Debug Callback */

  static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
                                VkDebugUtilsMessageTypeFlagsEXT _messageType,
                                const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
                                void* _pUserData)
  {
    switch (_messageSeverity)
    {
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        MFVE_LOG_INFO(_pCallbackData->pMessage);
        break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        MFVE_LOG_WARNING(_pCallbackData->pMessage);
        break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        MFVE_LOG_ERROR(_pCallbackData->pMessage);
        break;
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
      default:
        MFVE_LOG_INVALID(_pCallbackData->pMessage);
        break;
    }

    return VK_FALSE;
  }

  static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
  {
    createInfo                 = {};
    createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
  }

  static VkResult CreateDebugMessenger(const Instance& _instance,
                                       VkDebugUtilsMessengerEXT& _debugMessenger,
                                       const VkAllocationCallbacks* _allocator)
  {
    if (!ValidationLayers::Enabled())
    {
      return VK_SUCCESS;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    ValidationLayers::PopulateDebugMessengerCreateInfo(createInfo);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      _instance.GetInstance(), "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr)
    {
      return func(_instance.GetInstance(), &createInfo, _allocator, &_debugMessenger);
    }
    else
    {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }

  static void DestroyDebugMessenger(const Instance& _instance,
                                    const VkDebugUtilsMessengerEXT& _debugMessenger,
                                    const VkAllocationCallbacks* _allocator)
  {
    if (!ValidationLayers::Enabled())
    {
      return;
    }

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      _instance.GetInstance(), "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
      func(_instance.GetInstance(), _debugMessenger, _allocator);
    }
  }

} // namespace MFVE::Vulkan::ValidationLayers

#endif // MY_FIRST_VULKAN_ENGINE_VALIDATION_LAYERS_H
