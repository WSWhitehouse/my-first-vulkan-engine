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
      pickPhysicalDevice();
      createLogicalDevice();
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

    void pickPhysicalDevice()
    {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

      if (deviceCount == 0)
      {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

      for (const auto& device : devices)
      {
        if (isDeviceSuitable(device))
        {
          m_physicalDevice = device;
          break;
        }
      }

      if (m_physicalDevice == VK_NULL_HANDLE)
      {
        throw std::runtime_error("Failed to find suitable GPU!");
      }
    }

    bool isDeviceSuitable(VkPhysicalDevice _device)
    {
      QueueFamilyIndices indices = findQueueFamilies(_device);

      return indices.isComplete();
    }

    struct QueueFamilyIndices
    {
      std::optional<uint32_t> graphicsFamily;

      [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value(); }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice _device)
    {
      QueueFamilyIndices indices;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

      for (uint32_t i = 0; i < queueFamilyCount; ++i)
      {
        const auto& queueFamily = queueFamilies.at(static_cast<size_t>(i));

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
          indices.graphicsFamily = i;
          break;
        }
      }

      return indices;
    }

    void createLogicalDevice()
    {
      QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
      float queuePriority        = 1.0F;

      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
      queueCreateInfo.queueCount       = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;

      VkPhysicalDeviceFeatures deviceFeatures{};

      VkDeviceCreateInfo createInfo{};
      createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      createInfo.pQueueCreateInfos    = &queueCreateInfo;
      createInfo.queueCreateInfoCount = 1;
      createInfo.pEnabledFeatures     = &deviceFeatures;

      createInfo.enabledExtensionCount = 0;

      if (ValidationLayers::Active())
      {
        createInfo.enabledLayerCount   = ValidationLayers::LayerCount();
        createInfo.ppEnabledLayerNames = ValidationLayers::LayerNames();
      }
      else
      {
        createInfo.enabledLayerCount = 0;
      }

      VkCheck(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));
      vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
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
      vkDestroyDevice(m_device, nullptr);

      ValidationLayers::DestroyDebugMessenger(m_instance, nullptr);
      vkDestroyInstance(m_instance, nullptr);

      glfwDestroyWindow(m_window);
      glfwTerminate();
    }

   private:
    WindowProperties m_properties = {};
    GLFWwindow* m_window          = nullptr;

    VkInstance m_instance             = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device                 = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue           = VK_NULL_HANDLE;
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