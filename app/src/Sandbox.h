#ifndef MY_FIRST_VULKAN_ENGINE_SANDBOX_H
#define MY_FIRST_VULKAN_ENGINE_SANDBOX_H
#include <mfve.h>

class Sandbox : public MFVE::Application
{
 public:
  Sandbox()           = default;
  ~Sandbox() override = default;

 protected:
  void AppInit() override;
  void AppUpdate(const MFVE::FrameTimer &_frameTimer) override;
  void AppRender() override;
  void AppCleanUp() override;
};

#endif // MY_FIRST_VULKAN_ENGINE_SANDBOX_H
