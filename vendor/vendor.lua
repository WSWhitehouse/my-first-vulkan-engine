_vendordir = "%{wks.location}/vendor/"

--# Vulkan SDK (found by OS) #--
VULKAN_SDK = os.getenv("VULKAN_SDK")

--# Include Directory #--
IncludeDir = {}
IncludeDir["GLFW"]      = _vendordir .. "GLFW/include/"
IncludeDir["glm"]       = _vendordir .. "glm/"
IncludeDir["CppLog"]   = _vendordir .. "CppLog/"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/include/"

--# Lib Directory #--
LibDir = {}
LibDir["VulkanSDK"] = "%{VULKAN_SDK}/lib/"

--# Lib #--
Lib = {}
Lib["VulkanSDK"]   = "%{LibDir.VulkanSDK}/vulkan.lib"
Lib["VulkanUtils"] = "%{LibDir.VulkanSDK}/VkLayer_utils.lib"

--# Vendor #--
include "GLFW-premake"
include "premake/premake5"
