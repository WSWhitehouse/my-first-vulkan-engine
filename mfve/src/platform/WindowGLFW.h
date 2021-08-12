#ifndef MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H
#define MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H

#include <GLFW/glfw3.h>

#include "core/Window.h"

namespace MFVE
{
  class WindowGLFW : public Window
  {
   public:
    explicit WindowGLFW(const WindowProperties& _appProperties) : Window(_appProperties) {}
    ~WindowGLFW() override = default;

    // Window
    virtual bool CreateWindow(std::string_view _windowTitle) override;
    virtual void DestroyWindow() override;

     // Window Resized
     virtual void WaitWhileMinimised() override;

    // Surface
    virtual VkResult CreateSurface(const Vulkan::Instance& _instance,
                                   const VkAllocationCallbacks* _allocator) override;

    // Vulkan
    virtual std::vector<const char*> GetRequiredWindowExtensions() override;

    virtual void GetFrameBufferSize(int& _outWidth, int& _outHeight) override;

    virtual void SetWindowTitle(std::string_view _windowTitle) override;
    virtual bool WindowShouldClose() override;
    virtual void UpdateEvents() override;

    // Getters
    [[nodiscard]] virtual void* GetNativeWindow() const override { return m_window; }

    // Window Resize
    static inline void WindowResizeCallback(GLFWwindow* _window, int _width, int _height)
    {
      auto glfwWindow = reinterpret_cast<WindowGLFW*>(glfwGetWindowUserPointer(_window));
      glfwWindow->SetWindowResized(true);
    }

   private:
    GLFWwindow* m_window = nullptr;
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H
