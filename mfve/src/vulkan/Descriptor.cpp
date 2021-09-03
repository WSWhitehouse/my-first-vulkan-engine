#include "vulkan/Descriptor.h"

#include <mfve_pch.h>

// Vulkan
#include "vulkan/Device.h"

namespace MFVE::Vulkan
{
  void Descriptor::CreateDescriptorSetLayout(
    const Device& _device, const std::vector<VkDescriptorSetLayoutBinding>& _bindings,
    const VkAllocationCallbacks* _allocator)
  {
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(_bindings.size());
    layoutInfo.pBindings    = _bindings.data();

    VkCheck(vkCreateDescriptorSetLayout(
      _device.GetDevice(), &layoutInfo, _allocator, &m_descriptorSetLayout));
  }

  void Descriptor::DestroyDescriptorSetLayout(const Device& _device,
                                              const VkAllocationCallbacks* _allocator)
  {
    vkDestroyDescriptorSetLayout(_device.GetDevice(), m_descriptorSetLayout, _allocator);
  }

  void Descriptor::CreateDescriptorPool(const Device& _device,
                                        const std::vector<VkDescriptorPoolSize>& _poolSizes,
                                        const uint32_t& _maxSets,
                                        const VkAllocationCallbacks* _allocator)
  {
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(_poolSizes.size());
    poolInfo.pPoolSizes    = _poolSizes.data();
    poolInfo.maxSets       = _maxSets;

    VkCheck(vkCreateDescriptorPool(_device.GetDevice(), &poolInfo, _allocator, &m_descriptorPool));
  }

  void Descriptor::DestroyDescriptorPool(const Device& _device,
                                         const VkAllocationCallbacks* _allocator)
  {
    vkDestroyDescriptorPool(_device.GetDevice(), m_descriptorPool, _allocator);
  }

  void Descriptor::AllocateDescriptorSets(const Device& _device, const uint32_t& _size,
                                          const VkAllocationCallbacks* _allocator)
  {
    std::vector<VkDescriptorSetLayout> layouts(_size, m_descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = m_descriptorPool;
    allocInfo.descriptorSetCount = _size;
    allocInfo.pSetLayouts        = layouts.data();

    m_descriptorSets.resize(_size);
    VkCheck(vkAllocateDescriptorSets(_device.GetDevice(), &allocInfo, m_descriptorSets.data()));
  }

  void Descriptor::UpdateDescriptorSets(const Device& _device,
                                        const std::vector<VkWriteDescriptorSet>& _descriptorWrites)
  {
    vkUpdateDescriptorSets(_device.GetDevice(),
                           static_cast<uint32_t>(_descriptorWrites.size()),
                           _descriptorWrites.data(),
                           0,
                           nullptr);
  }
} // MFVE::Vulkan