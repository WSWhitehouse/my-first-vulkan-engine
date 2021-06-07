#ifndef MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H
#define MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H

#include <GLFW/glfw3.h>

#include "core/Window.h"

namespace MFVE
{
  class GLFWWindow : public Window
  {
   public:
    explicit GLFWWindow(const AppProperties& _appProperties) : Window(_appProperties) {}
    ~GLFWWindow() override = default;

   public: /* Functions */
    void CreateWindow() override;
    void DestroyWindow() override;

    bool WindowShouldClose() override;

    void UpdateEvents() override;

   public: /* Getters */
    [[nodiscard]] void* GetNativeWindow() const override { return m_window; }

   private:
    GLFWwindow* m_window = nullptr;
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_GLFW_WINDOW_H
