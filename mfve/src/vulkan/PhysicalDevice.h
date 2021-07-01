#ifndef MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H
#define MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H

#include <optional>

#include "Vk_Base.h"

namespace MFVE::Vulkan
{
  class PhysicalDevice
  {
   public:
    PhysicalDevice()  = default;
    ~PhysicalDevice() = default;

    struct QueueFamily
    {
      std::optional<uint32_t> graphicsFamily;

      [[nodiscard]] bool IsComplete() const { return graphicsFamily.has_value(); }
    };

    static inline std::vector<VkPhysicalDevice> FindAllPhysicalDevices(VkInstance _instance)
    {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

      return devices;
    }

    void PickSuitableDevice(VkInstance _instance);
    bool IsDeviceSuitable(VkPhysicalDevice _device);
    QueueFamily FindQueueFamilies(VkPhysicalDevice _device);

    /* Getters */
    [[nodiscard]] const VkPhysicalDevice& GetDevice() const { return m_physicalDevice; }
    [[nodiscard]] const VkPhysicalDeviceFeatures& GetFeatures() const { return m_features; }
    [[nodiscard]] const VkPhysicalDeviceProperties& GetProperties() const { return m_properties; }

   private:
    VkPhysicalDevice m_physicalDevice       = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures m_features     = {};
    VkPhysicalDeviceProperties m_properties = {};
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H
