#ifndef MFVE_EXTENSIONS_H
#define MFVE_EXTENSIONS_H

#include "ValidationLayers.h"
#include "Vk_Base.h"

namespace MFVE::Vulkan
{
  class Extensions
  {
   public:
    /* Constructor & Destructor */
    Extensions()  = default;
    ~Extensions() = default;

    /* Copy Constructors */
    Extensions(const Extensions&) = default;
    Extensions(Extensions&&)      = default;

    /* Assignment Operator */
    Extensions& operator=(const Extensions&) = default;
    Extensions& operator=(Extensions&&) = default;

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

    [[nodiscard]] bool AreExtensionsSupported(
      const std::vector<VkExtensionProperties>& _supportedExtensions = GetSupported()) const;

    /* Add Extensions */
    void Add(std::string_view _extension);
    void Add(size_t _extensionCount, const char** _extensions);

   public: /* Getters */
    [[nodiscard]] inline std::vector<const char*>& Get() { return m_extensions; }
    [[nodiscard]] inline const std::vector<const char*>& Get() const { return m_extensions; }

    [[nodiscard]] inline size_t GetCount() const { return m_extensions.size(); }
    [[nodiscard]] inline const char* const* GetNames() const { return m_extensions.data(); }

   private: /* Variables */
    std::vector<const char*> m_extensions{};
  };
}

#endif // MFVE_EXTENSIONS_H
