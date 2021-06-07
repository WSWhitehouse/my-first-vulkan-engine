#ifndef MY_FIRST_VULKAN_ENGINE_WINDOW_H
#define MY_FIRST_VULKAN_ENGINE_WINDOW_H

#include "AppProperties.h"

namespace MFVE
{
  class Window
  {
   public:
    explicit Window(const AppProperties& _appProperties) : m_appProperties(_appProperties) {}
    virtual ~Window() = default;

    /* Copy Constructors */
    Window(const Window&) = delete;
    Window(Window&&)      = delete;

    /* Assignment Operator */
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

   public: /* Functions */
    virtual void CreateWindow()  = 0;
    virtual void DestroyWindow() = 0;

    virtual bool WindowShouldClose() = 0;

    virtual void UpdateEvents() = 0;

   public: /* Getters */
    [[nodiscard]] virtual void* GetNativeWindow() const = 0;

   protected:
    const AppProperties& m_appProperties;
  };

} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_WINDOW_H
