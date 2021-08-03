#ifndef MY_FIRST_VULKAN_ENGINE_SHADER_H
#define MY_FIRST_VULKAN_ENGINE_SHADER_H

#include <filesystem>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "vulkan/Vk_Base.h"

namespace MFVE
{
  class Shader
  {
   public:
    explicit Shader(std::string_view _filePath);
    ~Shader();

   private:
    std::string filePath;

    static const char* CacheDir() { return "assets/cache/"; }
  };
}

#endif // MY_FIRST_VULKAN_ENGINE_SHADER_H
