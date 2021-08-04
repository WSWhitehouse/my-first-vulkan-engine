#include "AssetFilesystem.h"

#include <iostream>

bool AssetFilesystem::locateDirectories()
{
  if (!locateAssetPath())
  {
    return false;
  }

  if (!locateOutPath())
  {
    return false;
  }

  return true;
}

void AssetFilesystem::CompileAssets() {}

bool AssetFilesystem::locateAssetPath()
{
  namespace fs = std::filesystem;

  std::cout << "Locating Assets Folder:" << std::endl;

  const fs::path& currentPath = std::filesystem::current_path();
  std::cout << "  Current Path: " << currentPath.string() << std::endl;

  // The while loop goes through each parent directory and checks for an assets folder. The
  // asset folder should always be in the root of the project so the loop shouldn't make many
  // iterations - and should always be able to locate it. Replace the loop if a better method
  // is developed as this is probably not the most efficient way to do it, but it's only an
  // editor program after all!

  fs::path path = currentPath;

  while (true)
  {
    // Create a tmp path to check for assets folder
    fs::path tmpPath = path;
    tmpPath /= "assets";

    if (!fs::exists(tmpPath))
    {
      if (!path.has_parent_path())
      {
        // If we've hit the root directory then we haven't found the assets folder
        return false;
      }

      // Get the next parent path
      path = path.parent_path();
      continue;
    }

    assetsPath = tmpPath;
    std::cout << "  Asset Path:   " << assetsPath.string() << std::endl;
    return true;
  }
}

bool AssetFilesystem::locateOutPath()
{
  return true;
}
