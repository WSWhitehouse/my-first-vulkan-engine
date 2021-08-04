#ifndef MY_FIRST_VULKAN_ENGINE_ASSET_FILESYSTEM_H
#define MY_FIRST_VULKAN_ENGINE_ASSET_FILESYSTEM_H

#include <filesystem>

class AssetFilesystem
{
 public:
  AssetFilesystem()  = default;
  ~AssetFilesystem() = default;

  bool LocateAssetsFolder();
  void CompileAssets();

 private:
  std::filesystem::path assetsPath;
};

#endif // MY_FIRST_VULKAN_ENGINE_ASSET_FILESYSTEM_H
