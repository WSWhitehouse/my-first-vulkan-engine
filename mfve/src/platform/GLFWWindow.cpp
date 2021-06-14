#include "GLFWWindow.h"

#include <mfve_pch.h>

namespace MFVE
{
  void GLFWWindow::CreateWindow()
  {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(
      m_appProperties.WindowWidth,
      m_appProperties.WindowHeight,
      m_appProperties.Title.c_str(),
      nullptr,
      nullptr);
  }

  void GLFWWindow::DestroyWindow()
  {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  bool GLFWWindow::WindowShouldClose() { return glfwWindowShouldClose(m_window); }

  void GLFWWindow::UpdateEvents() { glfwPollEvents(); }

} // namespace MFVE