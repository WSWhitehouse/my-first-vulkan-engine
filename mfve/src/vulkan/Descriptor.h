#ifndef MY_FIRST_VULKAN_ENGINE_DESCRIPTOR_H
#define MY_FIRST_VULKAN_ENGINE_DESCRIPTOR_H

// Cpp
#include <vector>

// Vulkan
#include "vulkan/Vk_Base.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class Device;

  class Descriptor
  {
   public:
    Descriptor()  = default;
    ~Descriptor() = default;

    void CreateDescriptorSetLayout(const Device& _device,
                                   const std::vector<VkDescriptorSetLayoutBinding>& _bindings,
                                   const VkAllocationCallbacks* _allocator);
    void DestroyDescriptorSetLayout(const Device& _device, const VkAllocationCallbacks* _allocator);

    void CreateDescriptorPool(const Device& _device,
                              const std::vector<VkDescriptorPoolSize>& _poolSizes,
                              const uint32_t& _maxSets, const VkAllocationCallbacks* _allocator);
    void DestroyDescriptorPool(const Device& _device, const VkAllocationCallbacks* _allocator);

    void AllocateDescriptorSets(const Device& _device, const uint32_t& _size,
                                const VkAllocationCallbacks* _allocator);
    void UpdateDescriptorSets(const Device& _device, const uint32_t& _size,
                              const std::vector<VkWriteDescriptorSet>& _descriptorWrites);

    // Getters
    [[nodiscard]] const VkDescriptorSetLayout GetDescriptorSetLayout() const
    {
      return m_descriptorSetLayout;
    }
    [[nodiscard]] VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }
    [[nodiscard]] const std::vector<VkDescriptorSet>& GetDescriptorSets() const
    {
      return m_descriptorSets;
    }

   private:
    VkDescriptorSetLayout m_descriptorSetLayout   = VK_NULL_HANDLE;
    VkDescriptorPool m_descriptorPool             = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_descriptorSets = {};
  };
} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_DESCRIPTOR_H