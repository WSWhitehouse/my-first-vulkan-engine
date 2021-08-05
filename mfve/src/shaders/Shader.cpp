#include "Shader.h"

#include <mfve_pch.h>

namespace MFVE
{
  Shader::Shader(std::string_view _filePath) : filePath(_filePath) {}

  Shader::~Shader() {}
}
