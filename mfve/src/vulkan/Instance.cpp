#include "vulkan/Instance.h"

// Core
#include "core/Definitions.h"
#include "core/Properties.h"

// Vulkan
#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

namespace MFVE::Vulkan
{
  void Instance::CreateInstance(const AppProperties& _appProperties,
                                const std::vector<const char*>& _extensions,
                                const VkAllocationCallbacks* _allocator)
  {
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = _appProperties.name.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(
      _appProperties.versionMajor, _appProperties.versionMinor, _appProperties.versionPatch);
    appInfo.pEngineName   = MFVE_ENGINE_NAME;
    appInfo.engineVersion = VK_MAKE_VERSION(MFVE_VER_MAJOR, MFVE_VER_MINOR, MFVE_VER_PATCH);
    appInfo.apiVersion    = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    createInfo.enabledExtensionCount   = _extensions.size();
    createInfo.ppEnabledExtensionNames = _extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (ValidationLayers::Enabled())
    {
      createInfo.enabledLayerCount   = ValidationLayers::Layers.size();
      createInfo.ppEnabledLayerNames = ValidationLayers::Layers.data();

      ValidationLayers::PopulateDebugMessengerCreateInfo(debugCreateInfo);
      createInfo.pNext = &debugCreateInfo;
    }
    else
    {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext             = nullptr;
    }

    VkCheck(vkCreateInstance(&createInfo, _allocator, &m_instance));
  }

  void Instance::DestroyInstance(const VkAllocationCallbacks* _allocator)
  {
    vkDestroyInstance(m_instance, _allocator);
  }
} // namespace MFVE::Vulkan