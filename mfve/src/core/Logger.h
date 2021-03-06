#ifndef MY_FIRST_VULKAN_ENGINE_LOGGER_H
#define MY_FIRST_VULKAN_ENGINE_LOGGER_H

#include <CppLog/cout_logger.h>
#include <CppLog/logger.h>

namespace MFVE
{
  class Logger
  {
   public:
    Logger()  = delete;
    ~Logger() = delete;

    static void CreateLogger() { m_logger = new Log::cout_logger(); }
    static void DestroyLogger() { delete m_logger; }

    static inline Log::logger* GetLogger() { return m_logger; }

   private:
    static inline Log::logger* m_logger = nullptr;
  };
} // namespace MFVE

#ifdef MFVE_ENABLE_LOGGER

  #define MFVE_LOG_INFO(msg)    ::MFVE::Logger::GetLogger()->log(msg, Log::Severity::LOG_INFO)
  #define MFVE_LOG_DEBUG(msg)   ::MFVE::Logger::GetLogger()->log(msg, Log::Severity::LOG_DEBUG)
  #define MFVE_LOG_WARNING(msg) ::MFVE::Logger::GetLogger()->log(msg, Log::Severity::LOG_WARNING)
  #define MFVE_LOG_ERROR(msg)   ::MFVE::Logger::GetLogger()->log(msg, Log::Severity::LOG_ERROR)
  #define MFVE_LOG_FATAL(msg)                                        \
    ::MFVE::Logger::GetLogger()->log(msg, Log::Severity::LOG_FATAL); \
    throw std::runtime_error(msg)
  #define MFVE_LOG_INVALID(msg) ::MFVE::Logger::GetLogger()->log(msg, Log::Severity::LOG_INVALID)

#else

  #define MFVE_LOG_INFO(msg)
  #define MFVE_LOG_DEBUG(msg)
  #define MFVE_LOG_WARNING(msg)
  #define MFVE_LOG_ERROR(msg)
  #define MFVE_LOG_FATAL(msg) throw std::runtime_error(msg)
  #define MFVE_LOG_INVALID(msg)
  
#endif

#endif // MY_FIRST_VULKAN_ENGINE_LOGGER_H
