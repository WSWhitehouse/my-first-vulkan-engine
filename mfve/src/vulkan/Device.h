#ifndef MY_FIRST_VULKAN_ENGINE_DEVICE_H
#define MY_FIRST_VULKAN_ENGINE_DEVICE_H

// Cpp
#include <set>

// Core
#include "core/Window.h"

// Vulkan
#include "Vk_Base.h"
#include "vulkan/QueueFamily.h"
#include "vulkan/SupportDetails.h"

namespace MFVE::Vulkan
{
  // Forward Declarations
  class Instance;
  struct QueueFamilyIndicies;

  class Device
  {
   public:
    Device()  = default;
    ~Device() = default;

    void CreateDevice(const Instance& _instance, Window* _window,
                      const VkAllocationCallbacks* _allocator);
    void DestroyDevice(const VkAllocationCallbacks* _allocator);

    uint32_t FindMemoryType(uint32_t _typeFilter, VkMemoryPropertyFlags _properties) const;

    /* Getters */
    [[nodiscard]] VkDevice GetDevice() const { return m_device; }
    [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
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
    // Physical Device
    VkPhysicalDevice m_physicalDevice                 = VK_NULL_HANDLE;
    const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    bool IsDeviceSuitable(VkPhysicalDevice _device, VkSurfaceKHR _surface,
                          QueueFamilyIndicies& _queueFamilyIndicies);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice _device);

    static inline std::vector<VkPhysicalDevice> FindAllPhysicalDevices(const Instance& _instance)
    {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(_instance.GetInstance(), &deviceCount, nullptr);

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(_instance.GetInstance(), &deviceCount, devices.data());

      return devices;
    }

    // Logical Device
    VkDevice m_device = VK_NULL_HANDLE;

    // Queues
    QueueFamily m_graphicsQueue = {};
    QueueFamily m_presentQueue  = {};
    QueueFamily m_transferQueue = {};
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_DEVICE_H
