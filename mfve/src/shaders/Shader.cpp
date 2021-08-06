#include "Shader.h"

#include <fstream>
#include <mfve_pch.h>

namespace MFVE
{
  void Shader::Load(const std::filesystem::path& _filePath, const ShaderKind& _shaderKind,
                    const bool& _forceCompile)
  {
    // Set up file paths
    m_filePath         = _filePath;
    m_shaderKind       = _shaderKind;
    auto cacheFilePath = SHADER_CACHE_FOLDER + m_filePath.filename().string() + SPIRV_EXTENSION;

    // Remove any previous data
    m_shaderData.clear();

    // Try opening cached data first
    if (!Cache::Read(cacheFilePath, m_shaderData) || _forceCompile)
    {
      // Remove any previous data
      m_shaderData.clear();

      // If cache doesnt exist compile from source and cache
      std::vector<uint32_t> spriv = CompileFromSource();
      Cache::Write(cacheFilePath, spriv);

      m_shaderData = std::vector<char>(spriv.cbegin(), spriv.cend());

      MFVE_LOG_INFO("Compiled Shader: " + m_filePath.filename().string() +
                    " size: " + std::to_string(m_shaderData.size()));
    }
  }

  VkResult Shader::CreateShaderModule(const Vulkan::LogicalDevice& _logicalDevice,
                                      const VkAllocationCallbacks* _allocator)
  {
    MFVE_LOG_DEBUG("Shader Module " + m_filePath.filename().string());
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = m_shaderData.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(m_shaderData.data());

    return vkCreateShaderModule(
      _logicalDevice.GetDevice(), &createInfo, _allocator, &m_shaderModule);
  }

  void Shader::DestroyShaderModule(const Vulkan::LogicalDevice& _logicalDevice,
                                   const VkAllocationCallbacks* _allocator)
  {
    vkDestroyShaderModule(_logicalDevice.GetDevice(), m_shaderModule, _allocator);
  }

  std::string Shader::ReadSourceFile()
  {
    std::string result;
    std::ifstream in(FileSystem::GetAssetPathFromRelativePath(m_filePath),
                     std::ios::ate | std::ios::in | std::ios::binary);

    if (in)
    {
      size_t size = in.tellg();
      in.seekg(0);

      result.resize(size);
      in.read(&result[0], size);

      in.close();
    }
    else
    {
      MFVE_LOG_ERROR("Could not open file " + m_filePath.string());
    }

    return result;
  }

  std::vector<uint32_t> Shader::CompileFromSource(const bool& _optimize)
  {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    // options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
    // options.SetTargetSpirv(shaderc_spirv_version_1_0);

    if (_optimize)
    {
      options.SetOptimizationLevel(shaderc_optimization_level_size);
    }

    std::string shaderSrc = ReadSourceFile();

    MFVE_LOG_DEBUG(shaderSrc);

    shaderc::SpvCompilationResult module =
      compiler.CompileGlslToSpv(shaderSrc,
                                ShaderUtils::ShaderKindToShaderC(m_shaderKind),
                                FileSystem::GetAssetPathFromRelativePath(m_filePath).c_str(),
                                options);

    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    {
      MFVE_LOG_ERROR(module.GetErrorMessage());
      return std::vector<uint32_t>();
    }

    return std::vector<uint32_t>(module.cbegin(), module.cend());
  }
} // namespace MFVE
