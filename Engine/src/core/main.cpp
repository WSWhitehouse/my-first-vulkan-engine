#include <GLFW/glfw3.h>
#include <wipe_pch.h>

#include "platform/vulkan/Vk_Base.h"

int main(int /*argc*/, char** /*argv*/)
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

  LogSupportedExtensions();

  glm::mat4 matrix;
  glm::vec4 vec;
  auto test = matrix * vec;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  return EXIT_SUCCESS;
}