#ifndef MY_FIRST_VULKAN_ENGINE_WINDOW_H
#define MY_FIRST_VULKAN_ENGINE_WINDOW_H

#include <utility>
#include <vector>

#include "Properties.h"

// Vulkan
#include "vulkan/Instance.h"
#include "vulkan/Vk_Base.h"

namespace MFVE
{
  class Window
  {
   public:
    explicit Window(WindowProperties _windowProperties) : m_windowProperties(_windowProperties) {}
    virtual ~Window() = default;

    /* Copy Constructors */
    Window(const Window&) = delete;
    Window(Window&&)      = delete;

    /* Assignment Operator */
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    // Window
    virtual bool CreateWindow(std::string_view _windowTitle) = 0;
    virtual void DestroyWindow()                             = 0;

    // Window Resized
    // TODO : Perhaps create a window event system?
    bool HasWindowResized() const { return m_windowResized; }
    void ResetWindowResized() { m_windowResized = false; }
    virtual void WaitWhileMinimised() = 0;

    // Surface
    virtual VkResult CreateSurface(const Vulkan::Instance& _instance,
                                   const VkAllocationCallbacks* _allocator) = 0;
    void DestroySurface(const Vulkan::Instance& _instance, const VkAllocationCallbacks* _allocator)
    {
      vkDestroySurfaceKHR(_instance.GetInstance(), m_surface, _allocator);
    }

    // Vulkan
    virtual std::vector<const char*> GetRequiredWindowExtensions()   = 0;
    virtual void GetFrameBufferSize(int& _outWidth, int& _outHeight) = 0;

    virtual void SetWindowTitle(std::string_view _windowTitle) = 0;
    virtual bool WindowShouldClose()                           = 0;
    virtual void UpdateEvents()                                = 0;

    // Getters
    [[nodiscard]] const WindowProperties& GetWindowProperties() const { return m_windowProperties; }
    [[nodiscard]] VkSurfaceKHR GetSurface() const { return m_surface; }
    [[nodiscard]] virtual void* GetNativeWindow() const = 0;

    // Setters
    void SetWindowResized(const bool& _windowResized) { m_windowResized = _windowResized; }

   protected:
    // Window
    WindowProperties m_windowProperties;
    bool m_windowResized = false;

    // Vulkan
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
  };

} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_WINDOW_H
