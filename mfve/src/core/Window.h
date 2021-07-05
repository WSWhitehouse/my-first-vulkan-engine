#ifndef MY_FIRST_VULKAN_ENGINE_WINDOW_H
#define MY_FIRST_VULKAN_ENGINE_WINDOW_H

#include <utility>
#include <vector>

#include "Properties.h"
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

   public: /* Functions */
    virtual bool CreateWindow(std::string_view _windowTitle) = 0;
    virtual void DestroyWindow()                             = 0;

    /* Surface */
    virtual VkResult CreateSurface(VkInstance _instance,
                                   const VkAllocationCallbacks* _allocator) = 0;
    void DestroySurface(VkInstance _instance, const VkAllocationCallbacks* _allocator)
    {
      vkDestroySurfaceKHR(_instance, m_surface, _allocator);
    }

    /* Vulkan */
    virtual std::vector<const char*> GetRequiredWindowExtensions()   = 0;
    virtual void GetFrameBufferSize(int& _outWidth, int& _outHeight) = 0;

    virtual void SetWindowTitle(std::string_view _windowTitle) = 0;
    virtual bool WindowShouldClose()                           = 0;
    virtual void UpdateEvents()                                = 0;

   public: /* Getters */
    [[nodiscard]] const WindowProperties& GetWindowProperties() const { return m_windowProperties; }
    [[nodiscard]] VkSurfaceKHR GetSurface() const { return m_surface; }
    [[nodiscard]] virtual void* GetNativeWindow() const = 0;

   protected:
    WindowProperties m_windowProperties;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
  };

} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_WINDOW_H
