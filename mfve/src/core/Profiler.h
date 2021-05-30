#ifndef MY_FIRST_VULKAN_ENGINE_PROFILER_H
#define MY_FIRST_VULKAN_ENGINE_PROFILER_H

#include <chrono>

#include "Logger.h"

class Profiler
{
 public:
  Profiler() = default;

  explicit Profiler(bool _start)
  {
    if (_start)
    {
      Start();
    }
  }

  ~Profiler() { Stop(); }

  void Start() { m_startTimePoint = std::chrono::high_resolution_clock::now(); }

  void Stop() { m_endTimePoint = std::chrono::high_resolution_clock::now(); }

  double Seconds() { return Duration<std::chrono::seconds>(); }

  double Milliseconds() { return Duration<std::chrono::milliseconds>(); }

  double Microseconds() { return Duration<std::chrono::microseconds>(); }

  double Nanoseconds() { return Duration<std::chrono::nanoseconds>(); }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_endTimePoint;

  template<typename T>
  double Duration()
  {
    auto start = std::chrono::time_point_cast<T>(m_startTimePoint).time_since_epoch().count();
    auto end   = std::chrono::time_point_cast<T>(m_endTimePoint).time_since_epoch().count();

    return static_cast<double>(end - start);
  }
};

#endif // MY_FIRST_VULKAN_ENGINE_PROFILER_H
