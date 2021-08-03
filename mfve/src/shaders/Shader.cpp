#include "Shader.h"

#include <mfve_pch.h>

namespace MFVE
{
  Shader::Shader(std::string_view _filePath) : filePath(_filePath)
  {
    //    MFVE_LOG_INFO(std::filesystem::current_path().string());

    auto path = std::filesystem::current_path();
    path += "/assets";

    MFVE_LOG_INFO(path.string());
    MFVE_LOG_INFO(std::filesystem::exists(path) ? "True" : "False");
  }

  Shader::~Shader() {}
}
