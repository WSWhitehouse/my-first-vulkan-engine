#include "Application.h"

#include <mfve_pch.h>

// Platform
#include "platform/GLFWWindow.h"

// Vulkan
#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"

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
      MFVE_LOG_FATAL("Failed to create Window!");
    }

    /* Vulkan */
    InitExtensions();
    CreateInstance();
    CreateDebugMessenger();
    VkCheck(m_window->CreateWindowSurface(m_instance, nullptr, &m_surface));
    m_physicalDevice.PickSuitableDevice(m_instance, m_surface);
    VkCheck(m_logicalDevice.CreateDevice(&m_physicalDevice, nullptr));
    m_logicalDevice.CreateQueueHandles();
  }

  Application::~Application()
  {
    /* Vulkan */
    m_logicalDevice.Destroy(nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
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

  void Application::InitExtensions()
  {
    // Window Extensions
    Extensions::Extensions = m_window->GetRequiredWindowExtensions();

    // Validation Layers Extensions
    if (ValidationLayers::Enabled())
    {
      Extensions::Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (!Extensions::CheckExtensionSupport())
    {
      MFVE_LOG_FATAL("Extensions unsupported!");
    }
  }

  void Application::CreateInstance()
  {
    if (ValidationLayers::Enabled() && !ValidationLayers::CheckLayerSupport())
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

    createInfo.enabledExtensionCount   = Extensions::Extensions.size();
    createInfo.ppEnabledExtensionNames = Extensions::Extensions.data();

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

    VkCheck(vkCreateInstance(&createInfo, nullptr, &m_instance));
  }

  void Application::CreateDebugMessenger()
  {
    if (!ValidationLayers::Enabled())
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
    if (!ValidationLayers::Enabled())
    {
      return;
    }

    ValidationLayers::DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
  }

} // namespace MFVE