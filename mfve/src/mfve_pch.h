#ifndef MFVE_PCH_H
#define MFVE_PCH_H

// Platform Detection
#include "platform/PlatformDetection.h"

// Windows
#ifdef MFVE_PLATFORM_WINDOWS
  // github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
#endif

// C++ Standard Lib
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <chrono>


// C++ Math
#include <cmath>
#include <numeric>

// C++ String
#include <cstring>
#include <sstream>
#include <string>
#include <string_view>

// C++ Containers
#include <array>
#include <set>
#include <vector>

// Logger
#include "core/Logger.h"

// Random
#include "core/Random.h"

#endif // MFVE_PCH_H
