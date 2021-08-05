#include "FileSystem.h"

#include <mfve_pch.h>

// Where Am I
#include "whereami.h"

namespace MFVE
{
  void FileSystem::Init()
  {
    namespace fs = std::filesystem;

    MFVE_LOG_INFO("------------------------------");
    MFVE_LOG_INFO("Initialising File System");

    // Base Path
    int dirname_length = 0;
    int length         = wai_getExecutablePath(nullptr, 0, nullptr);
    char* path         = (char*)malloc(length + 1);
    wai_getExecutablePath(path, length, nullptr);
    path[length] = '\0';

    m_basePath = fs::path(path).remove_filename();
    free(path);
    MFVE_LOG_INFO("    Base Path:   " + m_basePath.string());

    // Asset Path
    m_assetPath = m_basePath / "assets";

    if (!std::filesystem::exists(m_assetPath))
    {
      MFVE_LOG_FATAL(
        "    ABORTING! Unable to locate Assets. Have you tried running 'Generate-Assets'?");
      return;
    }

    MFVE_LOG_INFO("    Assets Path: " + m_assetPath.string());

    MFVE_LOG_INFO("Finished Initialising File System");
    MFVE_LOG_INFO("------------------------------");

    return;
  }
} // namespace MFVE
