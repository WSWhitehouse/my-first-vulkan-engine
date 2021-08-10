#ifndef MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILY_H
#define MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILY_H

#include "Vk_Base.h"

namespace MFVE::Vulkan
{
  struct QueueFamily
  {
    uint32_t familyIndex = 0;
    VkQueue queue        = VK_NULL_HANDLE;

    void inline CreateHandle(VkDevice _device, uint32_t _familyIndex, uint32_t _index)
    {
      familyIndex = _familyIndex;
      vkGetDeviceQueue(_device, familyIndex, _index, &queue);
    }
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_QUEUE_FAMILY_H