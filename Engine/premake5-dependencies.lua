--# Vulkan SDK (found by OS) #--
VULKAN_SDK = os.getenv("VULKAN_SDK")

--# Include Directory #--
IncludeDir = {}
IncludeDir["GLFW"]      = _filedir .. "vendor/GLFW/include"
IncludeDir["glm"]       = _filedir .. "vendor/glm"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

--# Lib Directory #--
LibDir = {}
LibDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

--# Lib #--
Lib = {}
Lib["VulkanSDK"]   = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Lib["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"
