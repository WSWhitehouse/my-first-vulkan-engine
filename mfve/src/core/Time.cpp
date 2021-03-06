#include "Time.h"

#include <chrono>
#include <mfve_pch.h>

namespace MFVE
{
  bool Time::Tick()
  {
    auto now    = std::chrono::steady_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

    m_elapsedTime = std::chrono::time_point_cast<std::chrono::milliseconds>(m_startTime) - now_ms;
    m_frameCount++;

    return true;
  }
} // namespace MFVE