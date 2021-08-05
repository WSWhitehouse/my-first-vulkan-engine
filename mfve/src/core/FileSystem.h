#ifndef MY_FIRST_VULKAN_ENGINE_FILE_SYSTEM_H
#define MY_FIRST_VULKAN_ENGINE_FILE_SYSTEM_H

#include <filesystem>
#include <string>

/* --- MFVE FILE SYSTEM ---
 *  Base Path  = Path where executable is
 *  Asset Path = Path where assets folder is
 */

namespace MFVE
{
  class FileSystem
  {
   public:
    FileSystem()  = delete;
    ~FileSystem() = delete;

    /* Copy Constructors */
    FileSystem(const FileSystem& _fileSystem) = delete;
    FileSystem(FileSystem&& _fileSystem)      = delete;

    /* Assignment Operator */
    FileSystem& operator=(const FileSystem& _fileSystem) = delete;
    FileSystem& operator=(FileSystem&& _fileSystem) = delete;

    static void Init();
    static void Terminate();

    /* Getters */
    [[nodiscard]] static inline const std::filesystem::path& GetBasePath() { return m_basePath; }
    [[nodiscard]] static inline const std::filesystem::path& GetAssetPath() { return m_assetPath; }
    [[nodiscard]] static inline const std::filesystem::path& GetCachePath() { return m_cachePath; }

   private:
    static inline std::filesystem::path m_basePath;
    static inline std::filesystem::path m_assetPath;
    static inline std::filesystem::path m_cachePath;
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_FILE_SYSTEM_H