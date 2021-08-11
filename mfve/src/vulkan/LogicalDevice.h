#ifndef MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H
#define MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H

// Cpp
#include <set>

// Vulkan
#include "Vk_Base.h"
#include "vulkan/QueueFamily.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class PhysicalDevice;

  class LogicalDevice
  {
   public:
    LogicalDevice()  = default;
    ~LogicalDevice() = default;

    void CreateDevice(const PhysicalDevice& _physicalDevice,
                      const VkAllocationCallbacks* _allocator);
    void Destroy(const VkAllocationCallbacks* _allocator);

    /* Getters */
    [[nodiscard]] VkDevice GetDevice() const { return m_device; }
    [[nodiscard]] const QueueFamily& GetGraphicsQueue() const { return m_graphicsQueue; }
    [[nodiscard]] const QueueFamily& GetPresentQueue() const { return m_presentQueue; }
    [[nodiscard]] const QueueFamily& GetTransferQueue() const { return m_transferQueue; }

    inline std::set<uint32_t> GetUniqueQueueFamilyIndicies() const
    {
      return { m_graphicsQueue.familyIndex,
               m_presentQueue.familyIndex,
               m_transferQueue.familyIndex };
    }

   private:
    // Logical Device
    VkDevice m_device = VK_NULL_HANDLE;

    // Queues
    QueueFamily m_graphicsQueue = {};
    QueueFamily m_presentQueue  = {};
    QueueFamily m_transferQueue = {};
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_LOGICAL_DEVICE_H
