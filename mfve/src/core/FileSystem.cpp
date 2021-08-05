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

      MFVE_LOG_INFO("    Base Path:   " + GetBasePath().string());
    }

    // Asset Path
    {
      m_assetPath = m_basePath / ASSETS_FOLDER;

      if (!fs::exists(GetAssetPath()))
      {
        MFVE_LOG_FATAL(
          "ABORTING! Unable to locate Assets. Have you tried running 'Generate-Assets'?");
        return;
      }

      MFVE_LOG_INFO("    Assets Path: " + GetAssetPath().string());
    }

    // Cache Path
    {
      m_cachePath = fs::temp_directory_path();
      MFVE_LOG_INFO("    Cache Path:  " + GetCachePath().string());
    }

    MFVE_LOG_INFO("Finished Initialising File System");
    MFVE_LOG_INFO("------------------------------");
  }

  void FileSystem::Terminate() {}

  std::filesystem::path
  FileSystem::GetAssetPathFromRelativePath(const std::filesystem::path& _relPath)
  {
    if (_relPath.empty())
    {
      return GetAssetPath();
    }

    auto path = _relPath;

    // Finding relative root
    {
      const auto& relRoot = *_relPath.begin();

      // Remove 'assets/' from relative path if it exitsts
      if (relRoot == ASSETS_FOLDER)
      {
        MFVE_LOG_WARNING(
          "'assets/' is included in relative path, consider removing it. Relative Path: " +
          _relPath.string());

        path.clear();

        // Putting the path back together without 'assets/'
        for (auto itr = ++_relPath.begin(); itr != _relPath.end(); ++itr)
        {
          path /= *itr;
        }
      }
    }

    return GetAssetPath() / path;
  }
} // namespace MFVE
