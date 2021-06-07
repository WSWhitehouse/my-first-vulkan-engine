#include "GLFWWindow.h"

#include <mfve_pch.h>

void MFVE::GLFWWindow::CreateWindow()
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

void MFVE::GLFWWindow::DestroyWindow()
{
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

bool MFVE::GLFWWindow::WindowShouldClose()
{
  return glfwWindowShouldClose(m_window);
}

void MFVE::GLFWWindow::UpdateEvents() {
  glfwPollEvents();
}
