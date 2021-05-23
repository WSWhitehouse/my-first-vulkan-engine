#ifndef MFVE_PCH_H
#define MFVE_PCH_H

#include "platform/PlatformDetection.h"

#ifdef MFVE_PLATFORM_WINDOWS
  #ifndef NOMINMAX
    #define NOMINMAX // github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
  #endif
#endif

// C++ Standard Lib
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

// C++ String
#include <cstring>
#include <sstream>
#include <string>

// C++ Containers
#include <array>
#include <vector>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// Logger
#include "core/Logger.h"

#endif // MFVE_PCH_H
