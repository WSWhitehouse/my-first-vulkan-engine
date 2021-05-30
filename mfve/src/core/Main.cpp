#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <mfve_pch.h>

#include "Profiler.h"
#include "mfve.h"
#include "vulkan/Extensions.h"
#include "vulkan/ValidationLayers.h"
#include "vulkan/Vk_Base.h"

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
      constexpr const int count = 10;
      std::array<double, count> profilers{};
      for (int i = 0; i < count; ++i)
      {
        profilers.at(i) = profile();
      }

      double sum = std::accumulate(profilers.cbegin(), profilers.cend(), 0.0);
      double avg = sum / count;

      MFVE_LOG_INFO(std::to_string(avg));

      /* initWindow();
       initVulkan();
       mainLoop();
       cleanup();*/
    }

    double profile()
    {
      constexpr const int count = 1000;
      std::array<double, count> profilers{};
      for (int i = 0; i < count; ++i)
      {
        profilers.at(i) = profileLogger();
      }

      double sum = std::accumulate(profilers.cbegin(), profilers.cend(), 0.0);
      return sum / count;
    }

    double profileLogger()
    {
      constexpr const int count = 100;

      Profiler completeProfiler{};
      completeProfiler.Start();
      for (int i = 0; i < count; ++i)
      {
        // auto profiler = profilers.at(i);
        // profiler.Start();
        Logger::GetLogger()->log("TEST", static_cast<Log::Severity>(Random::Range(0, 4)));
        // MFVE_LOG_INFO("INFO");
        // profiler.Stop();
      }
      completeProfiler.Stop();

      /*
            MFVE_LOG_INFO("PROFILERS:");
            for (int i = 0; i < count; ++i)
            {
              auto profiler = profilers.at(i);
              MFVE_LOG_INFO("\t" + std::to_string(i) + " " +
         std::to_string(profiler.Nanoseconds()));
            }
      */

      // MFVE_LOG_INFO("TOTAL: " + std::to_string(completeProfiler.Microseconds()));

      return completeProfiler.Microseconds();
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
      VkCheck(m_validationLayers.CreateDebugMessenger(m_instance, nullptr, nullptr));
      createSurface();
      pickPhysicalDevice();
      createLogicalDevice();
    }

    void createInstance()
    {
      if (m_validationLayers.Active())
      {
        if (!m_validationLayers.CheckLayerSupport())
        {
          throw std::runtime_error("Validation layers requested, but not available!");
        }

        // Add validation layer extensions
        m_extensions.Add(m_validationLayers.Extensions());
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

      // Add GLFW Extensions
      {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        m_extensions.Add(glfwExtensions, glfwExtensionCount);
      }

      createInfo.enabledExtensionCount   = static_cast<uint32_t>(m_extensions.ExtensionCount());
      createInfo.ppEnabledExtensionNames = m_extensions.ExtensionNames();

      // Outside scope to ensure that it is not destroyed before the vkCreateInstance call
      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
      if (m_validationLayers.Active())
      {
        createInfo.enabledLayerCount   = static_cast<uint32_t>(m_validationLayers.LayerCount());
        createInfo.ppEnabledLayerNames = m_validationLayers.LayerNames();

        m_validationLayers.PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
      }
      else
      {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext             = nullptr;
      }

      VkCheck(vkCreateInstance(&createInfo, nullptr, &m_instance));
    }

    void createSurface()
    {
      VkCheck(glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface));
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
      bool extensionsSupported   = checkDeviceExtensionSupport(_device);

      return indices.isComplete() && extensionsSupported;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for (const auto& extension : availableExtensions)
      {
        requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
    }

    struct QueueFamilyIndices
    {
      std::optional<uint32_t> graphicsFamily;
      std::optional<uint32_t> presentFamily;

      [[nodiscard]] bool isComplete() const
      {
        return graphicsFamily.has_value() && presentFamily.has_value();
      }
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
        }

        VkBool32 presentSupport = true;
        VkCheck(vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, m_surface, &presentSupport));

        if (presentSupport)
        {
          indices.presentFamily = i;
        }
      }

      return indices;
    }

    void createLogicalDevice()
    {
      QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
      float queuePriority        = 1.0F;

      std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(),
                                                 indices.presentFamily.value() };
      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
      queueCreateInfos.reserve(uniqueQueueFamilies.size());

      for (uint32_t queueFamily : uniqueQueueFamilies)
      {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
      }

      VkPhysicalDeviceFeatures deviceFeatures{};

      VkDeviceCreateInfo createInfo{};
      createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      createInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());
      createInfo.pQueueCreateInfos       = queueCreateInfos.data();
      createInfo.pEnabledFeatures        = &deviceFeatures;
      createInfo.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
      createInfo.ppEnabledExtensionNames = deviceExtensions.data();

      if (m_validationLayers.Active())
      {
        createInfo.enabledLayerCount   = m_validationLayers.LayerCount();
        createInfo.ppEnabledLayerNames = m_validationLayers.LayerNames();
      }
      else
      {
        createInfo.enabledLayerCount = 0;
      }

      VkCheck(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));
      vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
      vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
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
      vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

      m_validationLayers.DestroyDebugMessenger(m_instance, nullptr);
      vkDestroyInstance(m_instance, nullptr);

      glfwDestroyWindow(m_window);
      glfwTerminate();
    }

   private:
    WindowProperties m_properties = {};
    GLFWwindow* m_window          = nullptr;

    Extensions m_extensions = {};
    ValidationLayers m_validationLayers{
#ifdef NDEBUG
      false
#else
      true
#endif
    };

    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkInstance m_instance             = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface            = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device                 = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue           = VK_NULL_HANDLE;
    VkQueue m_presentQueue            = VK_NULL_HANDLE;
  };
}

int main(int /*argc*/, char** /*argv*/)
{
  MFVE::Logger::Init();

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

  MFVE::Logger::CleanUp();
  return EXIT_SUCCESS;
}