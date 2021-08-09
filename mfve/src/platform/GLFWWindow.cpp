#include "GLFWWindow.h"

#include <mfve_pch.h>

namespace MFVE
{
  bool GLFWWindow::CreateWindow(std::string_view _windowTitle)
  {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(GetWindowProperties().width,
                                GetWindowProperties().height,
                                _windowTitle.data(),
                                nullptr,
                                nullptr);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, WindowResizeCallback);

    return m_window != nullptr;
  }

  void GLFWWindow::DestroyWindow()
  {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  void GLFWWindow::WaitWhileMinimised()
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    
    while (width == 0 || height == 0)
    {
      glfwGetFramebufferSize(m_window, &width, &height);
      glfwWaitEvents();
    }
  }

  std::vector<const char*> GLFWWindow::GetRequiredWindowExtensions()
  {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
  }

  VkResult GLFWWindow::CreateSurface(VkInstance _instance, const VkAllocationCallbacks* _allocator)
  {
    return glfwCreateWindowSurface(_instance, m_window, _allocator, &m_surface);
  }

  void GLFWWindow::GetFrameBufferSize(int& _outWidth, int& _outHeight)
  {
    glfwGetFramebufferSize(m_window, &_outWidth, &_outHeight);
  }

  void GLFWWindow::SetWindowTitle(std::string_view _windowTitle)
  {
    glfwSetWindowTitle(m_window, _windowTitle.data());
  }

  bool GLFWWindow::WindowShouldClose() { return glfwWindowShouldClose(m_window); }
  void GLFWWindow::UpdateEvents() { glfwPollEvents(); }

} // namespace MFVE