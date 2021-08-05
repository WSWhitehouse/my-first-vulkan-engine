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
    {
      int length = wai_getExecutablePath(nullptr, 0, nullptr);
      std::string path;
      path.resize(length);

      wai_getExecutablePath(path.data(), path.capacity(), nullptr);

      if (path.empty())
      {
        MFVE_LOG_FATAL("ABORTING! Unable to locate Base Path. Base path is empty!");
        return;
      }

      m_basePath = fs::path(path).remove_filename();

      MFVE_LOG_INFO("    Base Path:   " + m_basePath.string());
    }

    // Asset Path
    {
      m_assetPath = m_basePath / "assets";

      if (!fs::exists(m_assetPath))
      {
        MFVE_LOG_FATAL(
          "ABORTING! Unable to locate Assets. Have you tried running 'Generate-Assets'?");
        return;
      }

      MFVE_LOG_INFO("    Assets Path: " + m_assetPath.string());
    }

    // Cache Path
    {
      m_cachePath = fs::temp_directory_path();
    }

    MFVE_LOG_INFO("Finished Initialising File System");
    MFVE_LOG_INFO("------------------------------");
  }

  void FileSystem::Terminate() {}
} // namespace MFVE
