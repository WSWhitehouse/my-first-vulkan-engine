#ifndef WIPE_EXTENSIONS_H
#define WIPE_EXTENSIONS_H

#include "ValidationLayers.h"
#include "Vk_Base.h"

namespace WIPE
{
  class Extensions
  {
   public:
    /* Constructor & Destructor */
    Extensions()  = delete;
    ~Extensions() = delete;

    /* Copy Constructors */
    Extensions(const Extensions&) = delete;
    Extensions(Extensions&&)      = delete;

    /* Assignment Operator */
    Extensions& operator=(const Extensions&) = delete;
    Extensions& operator=(Extensions&&) = delete;

   public:
    /**
     * \brief Get all supported vulkan extensions
     * \return vector of VkExtensionProperties
     */
    static inline std::vector<VkExtensionProperties> GetSupported()
    {
      // Get number of extensions
      uint32_t count = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

      // Populate vector
      std::vector<VkExtensionProperties> extensions(count);
      vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

      return extensions;
    }

    /**
     * \brief Get all vulkan extensions required by the application
     * \return vector of all required extensions
     */
    static inline std::vector<const char*> GetRequired()
    {
      std::vector<const char*> extensions;

      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      extensions.insert(extensions.cend(), glfwExtensions, glfwExtensions + glfwExtensionCount);

      if (ValidationLayers::Active())
      {
        extensions.insert(
          extensions.cend(),
          ValidationLayers::ExtensionNames(),
          ValidationLayers::ExtensionNames() + ValidationLayers::ExtensionCount());
      }

      return extensions;
    }
  };
}

#endif // WIPE_EXTENSIONS_H
