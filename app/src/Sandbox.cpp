#include "Sandbox.h"

#include <pch.h>
#include <platform/GLFWWindow.h>

void Sandbox::AppInit()
{
  MFVE_LOG_INFO("App Init");
}

void Sandbox::AppUpdate(const MFVE::Time& _timer)
{
  // MFVE_LOG_INFO("App Update" + std::to_string(_timer.GetElapsedTime().count()));

  auto window = static_cast<GLFWwindow*>(GetWindow()->GetNativeWindow());

  // Signal exit if escape is pressed
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    SignalExit();
  }
}

void Sandbox::AppRender()
{
  // MFVE_LOG_INFO("App Render");
}

void Sandbox::AppCleanUp()
{
  MFVE_LOG_INFO("App Clean Up");
}
