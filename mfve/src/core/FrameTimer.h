#ifndef MY_FIRST_VULKAN_ENGINE_FRAME_TIMER_H
#define MY_FIRST_VULKAN_ENGINE_FRAME_TIMER_H

#include <chrono>

namespace MFVE
{
  class FrameTimer
  {
   public:
    FrameTimer()  = default;
    ~FrameTimer() = default;

    bool Tick();

   public: /* Getters */
    [[nodiscard]] const std::chrono::milliseconds& GetElapsedTime() const { return m_elapsedTime; }
    [[nodiscard]] const uint32_t& GetFrameCount() const { return m_frameCount; }

   private:
    const std::chrono::time_point<std::chrono::steady_clock> m_startTime =
      std::chrono::steady_clock::now();

    std::chrono::milliseconds m_elapsedTime{};
    uint32_t m_frameCount = 0;

    std::chrono::duration<double, std::milli> m_frameDelta;
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_FRAME_TIMER_H
