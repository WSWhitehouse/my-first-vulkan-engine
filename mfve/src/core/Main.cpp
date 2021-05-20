#include <GLFW/glfw3.h>
#include <mfve_pch.h>

#include "mfve.h"
#include "platform/vulkan/Extensions.h"
#include "platform/vulkan/ValidationLayers.h"
#include "platform/vulkan/Vk_Base.h"

namespace MFVE
{
  struct WindowProperties
  {
    int width         = 800;
    int height        = 600;
    std::string title = "MFVE";
  };

  class Application
  {
   public:
    void run()
    {
      initWindow();
      initVulkan();
      mainLoop();
      cleanup();
    }

   private:
    void initWindow()
    {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      m_window = glfwCreateWindow(
        m_properties.width, m_properties.height, m_properties.title.c_str(), nullptr, nullptr);
    }

    void initVulkan()
    {
      createInstance();
      VkCheck(ValidationLayers::InitDebugMessenger(m_instance, nullptr, nullptr));
    }

    void createInstance()
    {
      if (ValidationLayers::Active() && !ValidationLayers::CheckLayerSupport())
      {
        throw std::runtime_error("Validation layers requested, but not available!");
      }

      VkApplicationInfo appInfo{};
      appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName   = m_properties.title.c_str();
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName        = MFVE_ENGINE_NAME;
      appInfo.engineVersion      = VK_MAKE_VERSION(MFVE_VER_MAJOR, MFVE_VER_MINOR, MFVE_VER_PATCH);
      appInfo.apiVersion         = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo{};
      createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      auto extensions                    = Extensions::GetRequired();
      createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();

      // Outside scope to ensure that it is not destroyed before the vkCreateInstance call
      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
      if (ValidationLayers::Active())
      {
        createInfo.enabledLayerCount   = static_cast<uint32_t>(ValidationLayers::LayerCount());
        createInfo.ppEnabledLayerNames = ValidationLayers::LayerNames();

        ValidationLayers::PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
      }
      else
      {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext             = nullptr;
      }

      VkCheck(vkCreateInstance(&createInfo, nullptr, &m_instance));
    }

    void mainLoop()
    {
      while (!glfwWindowShouldClose(m_window))
      {
        glfwPollEvents();
      }
    }

    void cleanup()
    {
      ValidationLayers::DestroyDebugMessenger(m_instance, nullptr);

      vkDestroyInstance(m_instance, nullptr);

      glfwDestroyWindow(m_window);
      glfwTerminate();
    }

   private:
    WindowProperties m_properties = {};
    GLFWwindow* m_window          = nullptr;
    VkInstance m_instance         = nullptr;
  };
}

int main(int /*argc*/, char** /*argv*/)
{
  MFVE::Application app;

  try
  {
    app.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}