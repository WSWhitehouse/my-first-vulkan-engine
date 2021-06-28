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
      return;
    }

    /* Vulkan */
    CreateInstance();
  }

  Application::~Application()
  {
    /* Vulkan */
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

  void Application::CreateInstance()
  {
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

    // GLFW Extensions
    {
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;

      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      m_extensions.Add(glfwExtensionCount, glfwExtensions);
    }

    if (!m_extensions.AreExtensionsSupported())
    {
      MFVE_LOG_FATAL("Extensions unsupported!");
      throw std::runtime_error("Extensions unsupported!");
    }

    createInfo.enabledExtensionCount   = m_extensions.GetCount();
    createInfo.ppEnabledExtensionNames = m_extensions.GetNames();

    createInfo.enabledLayerCount = 0;

    VkCheck(vkCreateInstance(&createInfo, nullptr, &m_instance));
  }

} // namespace MFVE