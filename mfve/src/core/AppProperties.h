#ifndef MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H
#define MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H

#include <string>

namespace MFVE
{
  struct AppProperties
  {
    int WindowWidth   = 800;
    int WindowHeight  = 600;
    std::string Title = "MFVE App";
  };
}

#endif // MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H
