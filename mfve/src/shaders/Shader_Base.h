#ifndef MY_FIRST_VULKAN_ENGINE_SHADER_BASE_H
#define MY_FIRST_VULKAN_ENGINE_SHADER_BASE_H

// Shaders
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace MFVE
{
  enum ShaderKind : int
  {
    UNKNOWN_SHADER = -1,
    VERTEX_SHADER,
    FRAGMENT_SHADER
  };

  namespace ShaderUtils
  {
    static inline shaderc_shader_kind ShaderKindToShaderC(const ShaderKind& _kind)
    {
      switch (_kind)
      {
        case VERTEX_SHADER:
          return shaderc_glsl_vertex_shader;
        case FRAGMENT_SHADER:
          return shaderc_glsl_fragment_shader;
      }

      MFVE_LOG_FATAL("Cannot convert Shader Kind to 'shaderc_shader_kind'");
      return (shaderc_shader_kind)0;
    }

    static inline ShaderKind ExtensionToShaderKind(std::string_view _extension)
    {
      if (_extension == ".vert")
      {
        return VERTEX_SHADER;
      }
      else if (_extension == ".frag")
      {
        return FRAGMENT_SHADER;
      }

      MFVE_LOG_FATAL("Cannot convert extension to Shader Kind");
      return (ShaderKind)0;
    }
  } // namespace ShaderUtils
} // namespace MFVE

#endif // MY_FIRST_VULKAN_ENGINE_SHADER_BASE_H