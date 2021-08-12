#ifndef MY_FIRST_VULKAN_ENGINE_INSTANCE_H
#define MY_FIRST_VULKAN_ENGINE_INSTANCE_H

// Cpp
#include <vector>

// Vulkan
#include "vulkan/Vk_Base.h"

// Forward Declarations
namespace MFVE
{
  struct AppProperties;
} // namespace MFVE

namespace MFVE::Vulkan
{
  class Instance
  {
   public:
    Instance()  = default;
    ~Instance() = default;

    void CreateInstance(const AppProperties& _appProperties,
                        const std::vector<const char*>& _extensions,
                        const VkAllocationCallbacks* _allocator);
    void DestroyInstance(const VkAllocationCallbacks* _allocator);

    // Getters
    [[nodiscard]] VkInstance GetInstance() const { return m_instance; }

   private:
    VkInstance m_instance = VK_NULL_HANDLE;
  };
} // namepsace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_INSTANCE_H