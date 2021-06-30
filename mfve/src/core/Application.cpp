#include "Application.h"

#include <mfve_pch.h>

// Platform
#include "platform/GLFWWindow.h"

// GLFW
#include <GLFW/glfw3.h>

// Vulkan
using namespace MFVE::Vulkan;

namespace MFVE
{
  Application::Application(AppProperties _appProperties) :
    m_appProperties(std::move(_appProperties))
  {
    /* Window */
    WindowProperties windowProps{};
    m_window = new GLFWWindow(windowProps); // Creating a GLFW Window for now
    if (!m_window->CreateWindow(GetAppProperties().name))
    {
      MFVE_LOG_FATAL("Window failed to create!");
    }

    /* Vulkan */
    CreateInstance();
    CreateDebugMessenger();
    PickPhysicalDevice();
  }

  Application::~Application()
  {
    /* Vulkan */
    DestroyDebugMessenger();
    vkDestroyInstance(m_instance, nullptr);

    /* Window */
    m_window->DestroyWindow();
    delete m_window;
  }

  void Application::Run()
  {
    // Initialise Application
    AppInit();

    while (!m_window->WindowShouldClose() && !m_signalExit)
    {
      m_window->UpdateEvents();

      if (m_appTimer.Tick())
      {
        // Update Application
        AppUpdate(m_appTimer);
      }

      // Render Application
      AppRender();
    }

    // Clean Up Application
    AppCleanUp();
  }

  std::vector<const char*> Application::GetRequiredExtensions()
  {
    std::vector<const char*> extensions = m_window->GetRequiredWindowExtensions();

    if (m_enableValidationLayers)
    {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (!Extensions::CheckExtensionSupport(extensions))
    {
      MFVE_LOG_FATAL("Extensions unsupported!");
    }

    return extensions;
  }

  void Application::CreateInstance()
  {
    if (m_enableValidationLayers && !ValidationLayers::CheckLayerSupport(m_validationLayers))
    {
      MFVE_LOG_FATAL("Validation Layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = m_appProperties.name.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(
      GetAppProperties().versionMajor,
      GetAppProperties().versionMinor,
      GetAppProperties().versionPatch);
    appInfo.pEngineName   = MFVE_ENGINE_NAME;
    appInfo.engineVersion = VK_MAKE_VERSION(MFVE_VER_MAJOR, MFVE_VER_MINOR, MFVE_VER_PATCH);
    appInfo.apiVersion    = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto extensions              = GetRequiredExtensions();
    createInfo.enabledExtensionCount   = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (m_enableValidationLayers)
    {
      createInfo.enabledLayerCount   = m_validationLayers.size();
      createInfo.ppEnabledLayerNames = m_validationLayers.data();

      ValidationLayers::PopulateDebugMessengerCreateInfo(debugCreateInfo);
      createInfo.pNext = &debugCreateInfo;
    }
    else
    {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext             = nullptr;
    }

    VkCheck(vkCreateInstance(&createInfo, nullptr, &m_instance));
  }

  void Application::CreateDebugMessenger()
  {
    if (!m_enableValidationLayers)
    {
      return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    ValidationLayers::PopulateDebugMessengerCreateInfo(createInfo);

    VkCheck(ValidationLayers::CreateDebugUtilsMessengerEXT(
      m_instance, &createInfo, nullptr, &m_debugMessenger));
  }

  void Application::DestroyDebugMessenger()
  {
    if (!m_enableValidationLayers)
    {
      return;
    }

    ValidationLayers::DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
  }

  void Application::PickPhysicalDevice()
  {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
      MFVE_LOG_FATAL("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
      if (IsPhysicalDeviceSuitable(device))
      {
        m_physicalDevice = device;
        break;
      }
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
    {
      MFVE_LOG_FATAL("Failed to find a suitable GPU!");
    }
  }

  bool Application::IsPhysicalDeviceSuitable(const VkPhysicalDevice& _device) { return true; }

} // namespace MFVE