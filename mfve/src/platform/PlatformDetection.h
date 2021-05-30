#ifndef MFVE_PLATFORM_DETECTION_H
#define MFVE_PLATFORM_DETECTION_H

/* --------------------------------------------------------------------------------------- */
/* Platform detection using predefined macros. File sourced from TheCherno/Hazel:          */
/* https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/PlatformDetection.h */
/* --------------------------------------------------------------------------------------- */

#ifdef _WIN32 /* Windows */
  #define MFVE_PLATFORM_WINDOWS

  #ifdef _WIN64 /* Windows x64  */
    #define MFVE_PLATFORM_WINDOWS_64
    //#error "Windows x64 Builds are not supported!"

  #else /* Windows x86 */
    #define MFVE_PLATFORM_WINDOWS_86
    #error "Windows x86 Builds are not supported!"

  #endif

#elif defined(__APPLE__) || defined(__MACH__) /* Apple */
  #include <TargetConditionals.h>

  /* TARGET_OS_MAC exists on all the platforms so we must check all of them (in this
   * order) to ensure that we're running on MAC and not some other Apple platform
   */

  #if TARGET_IPHONE_SIMULATOR == 1 /* IPhone Sim */
    #error "IOS simulator is not supported!"

  #elif TARGET_OS_IPHONE == 1 /* IOS */
    #define MFVE_PLATFORM_IOS
    #error "IOS is not supported!"

  #elif TARGET_OS_MAC == 1 /* MAC OS */
    #define MFVE_PLATFORM_MACOS
    #error "MacOS is not supported!"

  #else /* Unknown Apple */
    #error "Unknown Apple platform!"
  #endif

/* We also have to check __ANDROID__ before __linux__ since android
 * is based on the linux kernel it has  __linux__ defined
 */
#elif defined(__ANDROID__) /* Android */
  #define MFVE_PLATFORM_ANDROID
  #error "Android is not supported!"

#elif defined(__linux__) /* Linux */
  #define MFVE_PLATFORM_LINUX

#else /* Unknown compiler/platform */
  #error "Unknown platform!"

#endif /* End of platform detection */

#endif // MFVE_PLATFORM_DETECTION_H
