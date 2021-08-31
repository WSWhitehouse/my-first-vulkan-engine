_vendordir = "%{wks.location}/vendor/"

--# Include Directory #--
IncludeDir = {}
IncludeDir["GLFW"]     = _vendordir .. "GLFW/include/"
IncludeDir["glm"]      = _vendordir .. "glm/"
IncludeDir["CppLog"]   = _vendordir .. "CppLog/"
IncludeDir["whereami"] = _vendordir .. "whereami/src/"
IncludeDir["stb"]      = _vendordir .. "stb/"

--# Vulkan SDK (found by OS) #--
VULKAN_SDK = os.getenv("VULKAN_SDK")

--# Vendor #--
include "GLFW-premake"
