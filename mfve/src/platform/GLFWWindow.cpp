#include "GLFWWindow.h"

#include <mfve_pch.h>

namespace MFVE
{
  bool GLFWWindow::CreateWindow(std::string_view _windowTitle)
  {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(
      GetWindowProperties().width,
      GetWindowProperties().height,
      _windowTitle.data(),
      nullptr,
      nullptr);

    return m_window != nullptr;
  }

  void GLFWWindow::DestroyWindow()
  {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  std::vector<const char*> GLFWWindow::GetRequiredWindowExtensions()
  {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
  }

  void GLFWWindow::SetWindowTitle(std::string_view _windowTitle)
  {
    glfwSetWindowTitle(m_window, _windowTitle.data());
  }

  bool GLFWWindow::WindowShouldClose() { return glfwWindowShouldClose(m_window); }
  void GLFWWindow::UpdateEvents() { glfwPollEvents(); }

} // namespace MFVE