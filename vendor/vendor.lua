_vendordir = "%{wks.location}/vendor/"

--# Include Directory #--
IncludeDir = {}
IncludeDir["GLFW"]      = _vendordir .. "GLFW/include/"
IncludeDir["glm"]       = _vendordir .. "glm/"
IncludeDir["CppLog"]   = _vendordir .. "CppLog/"

--# Lib Directory #--
LibDir = {}

--# Lib #--
Lib = {}

--# Vulkan SDK (found by OS) #--
VULKAN_SDK = os.getenv("VULKAN_SDK")

--# Vendor #--
include "GLFW-premake"
include "premake/premake5"
