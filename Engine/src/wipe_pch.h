#ifndef WIPE_WIPE_PCH_H
#define WIPE_WIPE_PCH_H

#include "platform/PlatformDetection.h"

#ifdef WIPE_PLATFORM_WINDOWS
#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

// C++ Standard Lib
#include <iostream>
#include <cstdlib>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

// C++ String
#include <string>
#include <sstream>

// C++ Containers
#include <array>
#include <vector>



#endif //WIPE_WIPE_PCH_H
