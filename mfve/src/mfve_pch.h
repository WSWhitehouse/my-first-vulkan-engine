#ifndef MFVE_PCH_H
#define MFVE_PCH_H

// Platform Detection
#include "platform/PlatformDetection.h"

// Windows
#ifdef MFVE_PLATFORM_WINDOWS
  #ifndef NOMINMAX
    #define NOMINMAX // github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
  #endif

  #include <Windows.h>
#endif

// C++ Standard Lib
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

// C++ Math
#include <cmath>
#include <numeric>

// C++ String
#include <cstring>
#include <sstream>
#include <string>

// C++ Containers
#include <array>
#include <set>
#include <vector>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// Logger
#include "core/Logger.h"

// Random
#include "core/Random.h"

#endif // MFVE_PCH_H
