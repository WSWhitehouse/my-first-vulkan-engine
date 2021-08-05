#include "Shader.h"

#include <fstream>
#include <mfve_pch.h>

namespace MFVE
{
  Shader::Shader(std::filesystem::path _filePath)
  {
    m_filePath      = FileSystem::GetAssetPathFromRelativePath(_filePath);
    m_cacheFilePath = SHADER_CACHE_FOLDER + m_filePath.filename().string() + SPIRV_EXTENSION;
    m_shaderKind    = ShaderUtils::ExtensionToShaderKind(m_filePath.extension().string());
  }

  Shader::~Shader() {}

  void Shader::OpenShader()
  {
    // Remove any previous data
    m_shaderData.clear();

    // Try opening cached data first
    if (Cache::Read(m_cacheFilePath, m_shaderData))
    {
      MFVE_LOG_DEBUG("Loaded shader from cache");
      return;
    }

    // If cache doesnt exist compile from source and cache
    CompileFromSource();
    Cache::Write(m_cacheFilePath, m_shaderData);

    MFVE_LOG_DEBUG("Compiled shader from source & cached");
  }

  std::string Shader::ReadSourceFile()
  {
    std::string result;
    std::ifstream in(m_filePath, std::ios::in | std::ios::binary);

    if (in)
    {
      in.seekg(0, std::ios::end);
      size_t size = in.tellg();
      if (size != -1)
      {
        result.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&result[0], size);
      }
      else
      {
        MFVE_LOG_ERROR("Could not read from file " + m_filePath.string());
      }
    }
    else
    {
      MFVE_LOG_ERROR("Could not open file " + m_filePath.string());
    }

    in.close();
    return result;
  }

  void Shader::CompileFromSource(const bool& _optimize)
  {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    if (_optimize)
      options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::string shaderSrc = ReadSourceFile();

    shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
      shaderSrc, ShaderUtils::ShaderKindToShaderC(m_shaderKind), m_filePath.c_str(), options);

    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    {
      MFVE_LOG_ERROR(module.GetErrorMessage());
      return;
    }

    m_shaderData = std::vector<uint32_t>(module.cbegin(), module.cend());
  }
} // namespace MFVE
