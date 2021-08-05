#ifndef MY_FIRST_VULKAN_ENGINE_SHADER_H
#define MY_FIRST_VULKAN_ENGINE_SHADER_H

// Vulkan
#include "vulkan/Vk_Base.h"

// Shaders
#include "shaders/Shader_Base.h"

// FileSystem
#include "core/FileSystem.h"
#include "core/Cache.h"

namespace MFVE
{
  class Shader
  {
   public:
    explicit Shader(std::string_view _filePath);
    ~Shader();

    void OpenShader();

   private:
    // File Paths
    std::filesystem::path m_filePath;
    std::filesystem::path m_cacheFilePath;

    ShaderKind m_shaderKind;
    std::vector<uint32_t> m_shaderData;

    std::string ReadSourceFile();
    void CompileFromSource(const bool& _optimize = true);

static inline const char* SHADER_CACHE_FOLDER = "shaders/";
    static inline const char* SPIRV_EXTENSION = ".spv";
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_SHADER_H
