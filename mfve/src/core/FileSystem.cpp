#include "FileSystem.h"

#include <fstream>
#include <mfve_pch.h>
#include <string>

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
      // Using the 'whereami' C API to find the path of the executable
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
      m_cachePath = m_basePath / CACHE_FOLDER;
      MFVE_LOG_INFO("    Cache Path:  " + GetCachePath().string());
    }

    MFVE_LOG_INFO("Finished Initialising File System");
    MFVE_LOG_INFO("------------------------------");
  }

  void FileSystem::Terminate()
  {
    MFVE_LOG_INFO("------------------------------");
    MFVE_LOG_INFO("Terminating File System");

    MFVE_LOG_INFO("Finished Terminating File System");
    MFVE_LOG_INFO("------------------------------");
  }

  std::filesystem::path
  FileSystem::GetAssetPathFromRelativePath(const std::filesystem::path& _relPath)
  {
    if (_relPath.empty())
    {
      return GetAssetPath();
    }

    return GetAssetPath() / CleanUpRelativePath(_relPath);
  }

  std::filesystem::path FileSystem::CleanUpRelativePath(const std::filesystem::path& _relPath)
  {
    auto path = _relPath;

    // Remove first '/' from relative path
    if (path.string()[0] == '/')
    {
      MFVE_LOG_WARNING("'/' is first char in relative path, consider removing it. Relative Path: " +
                       _relPath.string());

      std::string pathStr = path.string();
      pathStr.erase(pathStr.cbegin());

      path = pathStr;
    }

    /*
      // Remove folder from relative path if it exitsts
      if (*_relPath.begin() == ASSETS_FOLDER)
      {
        MFVE_LOG_WARNING(
          "Folder name is included in relative path, consider removing it. Relative Path: " +
          _relPath.string());

        path.clear();

        // Putting the path back together without 'assets/'
        for (auto itr = ++_relPath.begin(); itr != _relPath.end(); ++itr)
        {
          path /= *itr;
        }
      }
     */

    return path;
  }
} // namespace MFVE
