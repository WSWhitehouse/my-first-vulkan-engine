#ifndef MY_FIRST_VULKAN_ENGINE_SHADER_H
#define MY_FIRST_VULKAN_ENGINE_SHADER_H

// Vulkan
#include "vulkan/LogicalDevice.h"
#include "vulkan/Vk_Base.h"

// Shaders
#include "shaders/Shader_Base.h"

// FileSystem
#include "core/Cache.h"
#include "core/FileSystem.h"

namespace MFVE
{
  class Shader
  {
   public:
    /* Constructors */
    Shader()  = default;
    ~Shader() = default;

    void Load(const std::filesystem::path& _filePath, const ShaderKind& _shaderKind, const bool& _forceCompile = false);

    VkResult CreateShaderModule(const Vulkan::LogicalDevice& _logicalDevice,
                                const VkAllocationCallbacks* _allocator);
    void DestroyShaderModule(const Vulkan::LogicalDevice& _logicalDevice,
                             const VkAllocationCallbacks* _allocator);

    /* Getters */
    [[nodiscard]] const VkShaderModule& GetShaderModule() { return m_shaderModule; }

   private:
    /* File Paths */
    std::filesystem::path m_filePath;

    /* Shader */
    std::vector<char> m_shaderData;
    bool m_shaderLoaded           = false;
    ShaderKind m_shaderKind       = UNKNOWN_SHADER;
    VkShaderModule m_shaderModule = VK_NULL_HANDLE;

    std::string ReadSourceFile();
    std::vector<uint32_t> CompileFromSource(const bool& _optimize = true);

    /* Const */
    static inline const char* SHADER_CACHE_FOLDER = "shaders/";
    static inline const char* SPIRV_EXTENSION     = ".spv";
  };
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_SHADER_H
