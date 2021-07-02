#ifndef MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H
#define MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H

#include "Vk_Base.h"
#include "QueueFamilies.h"

namespace MFVE::Vulkan
{
  class PhysicalDevice
  {
   public:
    PhysicalDevice()  = default;
    ~PhysicalDevice() = default;

    static inline std::vector<VkPhysicalDevice> FindAllPhysicalDevices(VkInstance _instance)
    {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

      return devices;
    }

    void PickSuitableDevice(VkInstance _instance, VkSurfaceKHR _surface);

    /* Getters */
    [[nodiscard]] const VkPhysicalDevice& GetDevice() const { return m_physicalDevice; }
    [[nodiscard]] const VkPhysicalDeviceFeatures& GetFeatures() const { return m_features; }
    [[nodiscard]] const VkPhysicalDeviceProperties& GetProperties() const { return m_properties; }
    [[nodiscard]] const QueueFamilies& GetQueueFamilies() const { return m_queueFamilies; }
    [[nodiscard]] const std::vector<const char*>& GetDeviceExtensions() const { return m_deviceExtensions; }

   private:
    bool IsDeviceSuitable(VkPhysicalDevice _device, VkSurfaceKHR _surface);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice _device);

   private:
    VkPhysicalDevice m_physicalDevice       = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures m_features     = {};
    VkPhysicalDeviceProperties m_properties = {};
    QueueFamilies m_queueFamilies           = {};

    const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  };

} // namespace MFVE::Vulkan

#endif // MY_FIRST_VULKAN_ENGINE_PHYSICAL_DEVICE_H
