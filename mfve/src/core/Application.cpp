#include "Application.h"

#include <mfve_pch.h>

namespace MFVE
{
  void Application::Init(const AppProperties& _appProperties)
  {
    m_appProperties = _appProperties;

    initWindow();
    initVulkan();
  }

  void Application::Run()
  {
    // Initialise Application
    AppInit();

    while (!glfwWindowShouldClose(m_window))
    {
      glfwPollEvents();

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

  void Application::CleanUp()
  {
    vkDestroyDevice(m_device, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

    m_validationLayers.DestroyDebugMessenger(m_instance, nullptr);
    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
  }
} // namespace MFVE