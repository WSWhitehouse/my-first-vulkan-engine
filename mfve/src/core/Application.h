#ifndef MY_FIRST_VULKAN_ENGINE_APPLICATION_H
#define MY_FIRST_VULKAN_ENGINE_APPLICATION_H

// MFVE
#include "Profiler.h"
#include "Properties.h"
#include "Time.h"
#include "Window.h"

// Vulkan
#include "vulkan/CommandBuffer.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/LogicalDevice.h"
#include "vulkan/PhysicalDevice.h"
#include "vulkan/Pipeline.h"
#include "vulkan/Swapchain.h"
#include "vulkan/Vk_Base.h"

// Shaders
#include "shaders/Shader.h"

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
    virtual ~Application() = default;

    /* Copy Constructors */
    Application(const Application&) = delete;
    Application(Application&&)      = delete;

    /* Assignment Operator */
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

   public: /* Engine Functions */
    void Init(int argc, char** argv);
    void Terminate();

    void Run();
    void SignalExit() { m_signalExit = true; }

    /* Getters */
    [[nodiscard]] inline const AppProperties& GetAppProperties() const { return m_appProperties; }
    [[nodiscard]] inline Window* GetWindow() const { return m_window; }

   protected: /* Application Functions */
    virtual void AppInit()                    = 0;
    virtual void AppUpdate(const Time& _time) = 0;
    virtual void AppRender()                  = 0;
    virtual void AppCleanUp()                 = 0;

   private: /* Variables */
    bool m_signalExit             = false;
    AppProperties m_appProperties = {};
    Time m_appTimer               = {};

    // Window
    Window* m_window = nullptr;

    // Shaders
    Shader m_fragShader;
    Shader m_vertShader;

   private: /* Vulkan */
    // Extensions
    void InitExtensions();

    // Instance
    VkInstance m_instance = VK_NULL_HANDLE;
    void CreateInstance();

    // Debug Messenger
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    void CreateDebugMessenger();
    void DestroyDebugMessenger();

    // Devices
    Vulkan::PhysicalDevice m_physicalDevice = {};
    Vulkan::LogicalDevice m_logicalDevice   = {};

    // Renderer
    Vulkan::Swapchain m_swapchain         = {};
    Vulkan::Pipeline m_pipeline           = {};
    Vulkan::Framebuffer m_framebuffer     = {};
    Vulkan::CommandBuffer m_commandBuffer = {};
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_APPLICATION_H
