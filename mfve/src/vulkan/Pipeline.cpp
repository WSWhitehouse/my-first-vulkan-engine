#include "Pipeline.h"

#include <mfve_pch.h>

namespace MFVE::Vulkan
{
  void Pipeline::CreatePipeline(const LogicalDevice& _logicalDevice)
  {
    // Shaders
    m_fragShader.Load("shaders/fragshader.glsl", FRAGMENT_SHADER);
    m_vertShader.Load("shaders/vertshader.glsl", VERTEX_SHADER);

    VkCheck(m_fragShader.CreateShaderModule(_logicalDevice, nullptr));
    VkCheck(m_vertShader.CreateShaderModule(_logicalDevice, nullptr));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage               = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module              = m_vertShader.GetShaderModule();
    vertShaderStageInfo.pName               = "main";
    vertShaderStageInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module              = m_fragShader.GetShaderModule();
    fragShaderStageInfo.pName               = "main";
    fragShaderStageInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    // Shaders
    m_fragShader.DestroyShaderModule(_logicalDevice, nullptr);
    m_vertShader.DestroyShaderModule(_logicalDevice, nullptr);
  }
} // namespace MFVE