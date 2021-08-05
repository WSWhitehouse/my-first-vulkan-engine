#ifndef MY_FIRST_VULKAN_ENGINE_CACHE_H
#define MY_FIRST_VULKAN_ENGINE_CACHE_H

#include <fstream>
#include <vector>

#include "core/FileSystem.h"

namespace MFVE
{
  class Cache
  {
   public:
    Cache()  = delete;
    ~Cache() = delete;

    /* Copy Constructors */
    Cache(const Cache& _cache) = delete;
    Cache(Cache&& _cache)      = delete;

    /* Assignment Operator */
    Cache& operator=(const Cache& _cache) = delete;
    Cache& operator=(Cache&& _cache) = delete;

    /// Write data to file in cache
    /// @param _relPath relative path to file in cache
    /// @param _data data to write to cache
    template<typename T>
    static void Write(const std::filesystem::path& _relPath, const T& _data)
    {
      static_assert(!std::is_pointer_v<T>, "No Pointers");

      const auto path = FileSystem::GetCachePath() / FileSystem::CleanUpRelativePath(_relPath);

      if (!std::filesystem::exists(path))
      {
        std::filesystem::create_directories(path.parent_path());
      }

      std::ofstream stream(path, std::ios::out | std::ios::binary);
      if (stream.is_open())
      {
        stream.write(reinterpret_cast<const char*>(&_data), sizeof(T));
        stream.flush();
        stream.close();
      }
    }

    /// Write data to file in cache
    /// @param _relPath relative path to file in cache
    /// @param _data vector of data to write to cache
    template<typename T>
    static void Write(const std::filesystem::path& _relPath, const std::vector<T>& _data)
    {
      static_assert(!std::is_pointer_v<T>, "No Pointers");

      const auto path = FileSystem::GetCachePath() / FileSystem::CleanUpRelativePath(_relPath);

      if (!std::filesystem::exists(path))
      {
        std::filesystem::create_directories(path.parent_path());
      }

      std::ofstream stream(path, std::ios::out | std::ios::binary);
      if (stream.is_open())
      {
        stream.write((char*)_data.data(), _data.size() * sizeof(T));
        stream.flush();
        stream.close();
      }
    }

    /// Read data from file in cache
    /// @return boolean true if read correctly, false otherwise
    /// @param _relPath relative path to file in cache
    /// @param _outData data read from file
    template<typename T>
    static bool Read(const std::filesystem::path& _relPath, T& _outData)
    {
      static_assert(!std::is_pointer_v<T>, "No Pointers");

      const auto path = FileSystem::GetCachePath() / FileSystem::CleanUpRelativePath(_relPath);

      std::ifstream stream(path, std::ios::in | std::ios::binary);
      if (stream.is_open())
      {
        stream.read((char*)_outData.data(), sizeof(T));
        return true;
      }

      return false;
    }

    /// Read data from file in cache
    /// @return boolean true if read correctly, false otherwise
    /// @param _relPath relative path to file in cache
    /// @param _outData vector of data read from file
    template<typename T>
    static bool Read(const std::filesystem::path& _relPath, std::vector<T>& _outData)
    {
      static_assert(!std::is_pointer_v<T>, "No Pointers");

      const auto path = FileSystem::GetCachePath() / FileSystem::CleanUpRelativePath(_relPath);

      std::ifstream stream(path, std::ios::in | std::ios::binary);
      if (stream.is_open())
      {
        // Find length of file
        stream.seekg(0, std::ios::end);
        auto size = stream.tellg();
        stream.seekg(0, std::ios::beg);

        _outData.resize(size / sizeof(T));
        stream.read((char*)_outData.data(), size);

        return true;
      }

      return false;
    }
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_CACHE_H