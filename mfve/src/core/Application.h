#ifndef MY_FIRST_VULKAN_ENGINE_APPLICATION_H
#define MY_FIRST_VULKAN_ENGINE_APPLICATION_H

// MFVE
#include "Definitions.h"
#include "Profiler.h"
#include "Properties.h"
#include "Time.h"
#include "Window.h"

// Renderer
#include "core/Renderer.h"

// Shaders
#include "shaders/Shader.h"

namespace MFVE
{
  class Application
  {
   public:
    explicit Application(AppProperties _appProperties);
    virtual ~Application() = default;

    // Copy Constructors
    Application(const Application&) = delete;
    Application(Application&&)      = delete;

    // Assignment Operator
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    // Engine Functions
    void Init(int argc, char** argv);
    void Terminate();
    void Run();

    void SignalExit() { m_signalExit = true; }

    // Getters
    [[nodiscard]] const AppProperties& GetAppProperties() const { return m_appProperties; }
    [[nodiscard]] const Renderer& GetRenderer() const { return m_renderer; }
    [[nodiscard]] Window* GetWindow() const { return m_renderer.GetWindow(); }

   protected:
    // Application Functions
    virtual void AppInit()                    = 0;
    virtual void AppUpdate(const Time& _time) = 0;
    virtual void AppRender()                  = 0;
    virtual void AppCleanUp()                 = 0;

   private:
    bool m_signalExit             = false;
    AppProperties m_appProperties = {};
    Time m_appTimer               = {};

    // Renderer
    Renderer m_renderer = {};
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_APPLICATION_H
