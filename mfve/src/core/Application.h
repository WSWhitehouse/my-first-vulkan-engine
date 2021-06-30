#ifndef MY_FIRST_VULKAN_ENGINE_APPLICATION_H
#define MY_FIRST_VULKAN_ENGINE_APPLICATION_H

// MFVE
#include "Profiler.h"
#include "Properties.h"
#include "Time.h"
#include "Window.h"

// Vulkan
//#include "vulkan/ValidationLayers.h"
#include "vulkan/Extensions.h"
#include "vulkan/Vk_Base.h"

// Defines
#define MFVE_ENGINE_NAME "my-first-vulkan-engine"
#define MFVE_VER_MAJOR   0
#define MFVE_VER_MINOR   1
#define MFVE_VER_PATCH   0

namespace MFVE
{
  class Application
  {
   public:
    explicit Application(AppProperties _appProperties);
    virtual ~Application();

    /* Copy Constructors */
    Application(const Application&) = delete;
    Application(Application&&)      = delete;

    /* Assignment Operator */
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

   public: /* Engine Functions */
    void Run();
    void SignalExit() { m_signalExit = true; }

   protected: /* Application Functions */
    virtual void AppInit()                    = 0;
    virtual void AppUpdate(const Time& _time) = 0;
    virtual void AppRender()                  = 0;
    virtual void AppCleanUp()                 = 0;

   public: /* Getters */
    [[nodiscard]] inline const AppProperties& GetAppProperties() const { return m_appProperties; }
    [[nodiscard]] inline Window* GetWindow() const { return m_window; }

   private: /* Variables */
    bool m_signalExit             = false;
    AppProperties m_appProperties = {};
    Time m_appTimer               = {};

    Window* m_window = nullptr;

   private: /* Vulkan */
    // Extensions
    std::vector<const char*> GetRequiredExtensions();

    // Validation Layers
    const std::vector<const char*> m_validationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
    const bool m_enableValidationLayers = false;
#else
    const bool m_enableValidationLayers = true;
#endif

    // Instance
    VkInstance m_instance = VK_NULL_HANDLE;
    void CreateInstance();

    // Debug Messenger
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    void CreateDebugMessenger();
    void DestroyDebugMessenger();

    // Physical Device
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    void PickPhysicalDevice();
    bool IsPhysicalDeviceSuitable(const VkPhysicalDevice& _device);
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_APPLICATION_H
