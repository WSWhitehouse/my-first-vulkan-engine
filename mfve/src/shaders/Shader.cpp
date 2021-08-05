#include "Shader.h"

#include <mfve_pch.h>

#include "core/FileSystem.h"

namespace MFVE
{
  Shader::Shader(std::string_view _filePath) : filePath(_filePath)
  {
    auto path = FileSystem::GetAssetPathFromRelativePath(filePath);

    MFVE_LOG_INFO(path.string() + " " + std::to_string(std::filesystem::exists(path)));
  }

  Shader::~Shader() {}
}
