#include <iostream>

#include "AssetFilesystem.h"

int main(int argc, char** argv)
{
  std::cout << "Running Asset Pipeline" << std::endl;

  AssetFilesystem assetFilesystem;

  if (!assetFilesystem.locateDirectories())
  {
    std::cerr << "Could not find Assets folder!" << std::endl;
  }
}