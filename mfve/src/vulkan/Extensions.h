#ifndef MFVE_EXTENSIONS_H
#define MFVE_EXTENSIONS_H

#include "ValidationLayers.h"
#include "Vk_Base.h"

namespace MFVE
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

    /* Getters */
    [[nodiscard]] size_t ExtensionCount() const { return ExtensionVector().size(); }
    [[nodiscard]] const char* const* ExtensionNames() const { return ExtensionVector().data(); }
    [[nodiscard]] const std::vector<const char*>& ExtensionVector() const { return m_extensions; }

    void Add(const std::vector<const char*>& _extensions)
    {
      m_extensions.insert(m_extensions.cend(), _extensions.cbegin(), _extensions.cend());
    }

    void Add(const char* const* _extensions, const size_t& _count)
    {
      m_extensions.insert(m_extensions.cend(), _extensions, _extensions + _count);
    }

   private:
    std::vector<const char*> m_extensions = {};
  };
}

#endif // MFVE_EXTENSIONS_H
