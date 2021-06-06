#include "Sandbox.h"

#include <pch.h>

void Sandbox::AppInit()
{
  MFVE_LOG_INFO("App Init")
}

void Sandbox::AppUpdate(const MFVE::FrameTimer& _frameTimer)
{
  // MFVE_LOG_INFO("App Update" + std::to_string(_frameTimer.GetElapsedTime().count()))
}

void Sandbox::AppRender()
{
  // MFVE_LOG_INFO("App Render")
}

void Sandbox::AppCleanUp()
{
  MFVE_LOG_INFO("App Clean Up")
}
