#include "Application.h"

#include <mfve_pch.h>

// Platform
#include "platform/GLFWWindow.h"

namespace MFVE
{
  Application::Application(AppProperties _appProperties) :
    m_appProperties(std::move(_appProperties))
  {
    InitWindow();
    initVulkan();
  }

  Application::~Application()
  {
    vkDestroyDevice(m_device, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

    m_validationLayers.DestroyDebugMessenger(m_instance, nullptr);
    vkDestroyInstance(m_instance, nullptr);

    m_window->DestroyWindow();
    delete m_window;
  }

  void Application::Run()
  {
    // Initialise Application
    AppInit();

    while (!m_window->WindowShouldClose())
    {
      m_window->UpdateEvents();

      if (m_frameTimer.Tick())
      {
        // Update Application
        AppUpdate(m_frameTimer);
      }

      // Render Application
      AppRender();
    }

    // Clean Up Application
    AppCleanUp();
  }

  void Application::InitWindow()
  {
    // Creating a GLFW Window for now
    m_window = new GLFWWindow(m_appProperties);
    m_window->CreateWindow();
  }

} // namespace MFVE