#ifndef MY_FIRST_VULKAN_ENGINE_PROPERTIES_H
#define MY_FIRST_VULKAN_ENGINE_PROPERTIES_H

#include <string>

namespace MFVE
{
  struct AppProperties
  {
    std::string name = "MFVE App";

    uint32_t versionMajor = 1;
    uint32_t versionMinor = 0;
    uint32_t versionPatch = 0;
  };

  struct WindowProperties
  {
    int width  = 800;
    int height = 600;
  };
}

#endif // MY_FIRST_VULKAN_ENGINE_PROPERTIES_H
