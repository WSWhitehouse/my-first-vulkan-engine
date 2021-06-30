#ifndef MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H
#define MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H

#include <GLFW/glfw3.h>

#include "core/Window.h"

namespace MFVE
{
  class GLFWWindow : public Window
  {
   public:
    explicit GLFWWindow(const WindowProperties& _appProperties) : Window(_appProperties) {}
    ~GLFWWindow() override = default;

   public: /* Functions */
    virtual bool CreateWindow(std::string_view _windowTitle) override;
    virtual void DestroyWindow() override;

    virtual std::vector<const char*> GetRequiredWindowExtensions() override;

    virtual void SetWindowTitle(std::string_view _windowTitle) override;
    virtual bool WindowShouldClose() override;
    virtual void UpdateEvents() override;

   public: /* Getters */
    [[nodiscard]] virtual void* GetNativeWindow() const override { return m_window; }

   private:
    GLFWwindow* m_window = nullptr;
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H
