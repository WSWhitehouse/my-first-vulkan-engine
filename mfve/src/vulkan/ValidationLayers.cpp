/*
namespace MFVE
{
  void
  ValidationLayers::PopulateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& _createInfo)
  {
    _createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    _createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    _createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    _createInfo.pfnUserCallback = DebugCallback;
  }

  VkResult ValidationLayers::CreateDebugMessenger(
    VkInstance _instance, const VkAllocationCallbacks* _pAllocator, void* _userData)
  {
    if (!Active())
    {
      // If layers are inactive, return success
      return VK_SUCCESS;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);
    createInfo.pUserData = _userData;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      _instance,
      "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr)
    {
      return func(_instance, &createInfo, _pAllocator, &m_debugMessenger);
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
      _instance,
      "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
      func(_instance, m_debugMessenger, _pAllocator);
    }
  }

  VkBool32 ValidationLayers::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT _messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData, void* _pUserData)
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
}
*/