#ifndef MY_FIRST_VULKAN_ENGINE_ASSET_FILESYSTEM_H
#define MY_FIRST_VULKAN_ENGINE_ASSET_FILESYSTEM_H

#include <filesystem>

class AssetFilesystem
{
 public:
  AssetFilesystem()  = default;
  ~AssetFilesystem() = default;

  bool locateDirectories();
  void CompileAssets();

 private:
  std::filesystem::path assetsPath;
  std::filesystem::path outPath;

  bool locateAssetPath();
  bool locateOutPath();


};

#endif // MY_FIRST_VULKAN_ENGINE_ASSET_FILESYSTEM_H
