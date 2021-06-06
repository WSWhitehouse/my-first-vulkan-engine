#ifndef MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H
#define MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H

#include <string>

namespace MFVE
{
  struct AppProperties
  {
    int width         = 800;
    int height        = 600;
    std::string title = "MFVE App";
  };
}

#endif // MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H
