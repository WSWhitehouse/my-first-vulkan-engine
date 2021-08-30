#include "Application.h"

#include <mfve_pch.h>

// Platform
#include "platform/WindowGLFW.h"

// FileSystem
#include "core/FileSystem.h"

namespace MFVE
{
  Application::Application(AppProperties _appProperties) :
    m_appProperties(std::move(_appProperties))
  {
  }

  void Application::Init(int argc, char** argv)
  {
    /* File System */
    FileSystem::Init();

    /* Window */
    WindowProperties windowProps{};

    // Creating a GLFW Window for now - in future automatically
    // decide which window is best on each platform
    auto _window = new WindowGLFW(windowProps);

    /* Vulkan */
    m_renderer.CreateRenderer(GetAppProperties(), _window, nullptr);
  }

  void Application::Terminate()
  {
    /* Vulkan */
    m_renderer.DestroyRenderer(nullptr);

    /* File System */
    FileSystem::Terminate();
  }

  void Application::Run()
  {
    // Initialise Application
    AppInit();

    while (!m_renderer.SignalExit() && !m_signalExit)
    {
      m_renderer.PreRender();

      if (m_appTimer.Tick())
      {
        // Update Application
        AppUpdate(m_appTimer);
      }

      // Render Application
      AppRender();
      m_renderer.DrawFrame(nullptr);
      m_renderer.PostRender();
    }

    // Wait for logical device to finish up
    m_renderer.DeviceWaitIdle();

    // Clean Up Application
    AppCleanUp();
  }

} // namespace MFVE