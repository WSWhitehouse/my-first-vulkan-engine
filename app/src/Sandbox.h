#ifndef MY_FIRST_VULKAN_ENGINE_SANDBOX_H
#define MY_FIRST_VULKAN_ENGINE_SANDBOX_H
#include <mfve.h>

class Sandbox : public MFVE::Application
{
 public:
  explicit Sandbox(const MFVE::AppProperties& _appProperties) : MFVE::Application(_appProperties) {}
  ~Sandbox() override = default;

 protected:
  void AppInit() override;
  void AppUpdate(const MFVE::Time& _timer) override;
  void AppRender() override;
  void AppCleanUp() override;
};

#endif // MY_FIRST_VULKAN_ENGINE_SANDBOX_H
