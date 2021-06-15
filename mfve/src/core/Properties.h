#ifndef MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H
#define MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H

#include <string>

namespace MFVE
{
  struct Properties
  {
    int WindowWidth  = 800;
    int WindowHeight = 600;

    std::string Name = "MFVE App";

    uint32_t VersionMajor = 1;
    uint32_t VersionMinor = 0;
    uint32_t VersionPatch = 0;
  };
}

#endif // MY_FIRST_VULKAN_ENGINE_APPLICATION_PROPERTIES_H
