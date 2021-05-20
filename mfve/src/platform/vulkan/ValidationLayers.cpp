#include "ValidationLayers.h"

#include <mfve_pch.h>

namespace MFVE
{
  /* Layer Support */

  std::vector<VkLayerProperties> ValidationLayers::GetSupportedLayers()
  {
    // Get number of layers
    uint32_t count;
    vkEnumerateInstanceLayerProperties(&count, nullptr);

    // Populate vector
    std::vector<VkLayerProperties> layers(count);
    vkEnumerateInstanceLayerProperties(&count, layers.data());

    return layers;
  }

  bool ValidationLayers::CheckLayerSupport(const std::vector<VkLayerProperties>& _layers)
  {
    // Check all requested validation layers are supported
    for (const char* layerName : m_validationLayers)
    {
      bool layerFound = false;

      for (const auto& layerProperties : _layers)
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

  /* Debug Messenger */

  void
  ValidationLayers::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo)
  {
    _createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    _createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    _createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    _createInfo.pfnUserCallback = DebugCallback;
  }

  VkResult ValidationLayers::InitDebugMessenger(
    VkInstance _instance, const VkAllocationCallbacks* _pAllocator, void* _userData)
  {
    if (!Active())
    {
      return VK_SUCCESS;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);
    createInfo.pUserData = _userData;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      _instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr)
    {
      return func(_instance, &createInfo, _pAllocator, &debugMessenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }

  void ValidationLayers::DestroyDebugMessenger(
    VkInstance _instance, const VkAllocationCallbacks* _pAllocator)
  {
    if (!Active())
    {
      return;
    }

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      _instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
      func(_instance, debugMessenger, _pAllocator);
    }
  }

  VkBool32 ValidationLayers::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT _messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData, void* _pUserData)
  {
    std::cerr << "Vk Validation Layer: " << _pCallbackData->pMessage << std::endl;

    return VK_FALSE;
  }

}