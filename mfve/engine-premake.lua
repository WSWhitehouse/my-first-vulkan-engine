project "MFVE"
    _enginefiledir = "%{wks.location}/mfve/"

    location(_enginefiledir)
    uuid(os.uuid())
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir(_apptargetdir)
    objdir(_appobjdir)

    defines {
        --# GLFW #--
        "GLFW_INCLUDE_VULKAN",

        --# glm #--
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE"
    }

    files {
        --# mfve Src #--
        _enginefiledir .. "src/**.h",
        _enginefiledir .. "src/**.c",
        _enginefiledir .. "src/**.hpp",
        _enginefiledir .. "src/**.cpp",

        --# glm #--
        "%{IncludeDir.glm}/glm/**.hpp",
        "%{IncludeDir.glm}/glm/**.inl",

        --# Cpp Log #--
        "%{IncludeDir.CppLog}/CppLog/**.h"
    }

    includedirs {
        _enginefiledir .. "src/",

        --# Vendor #--
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.CppLog}",

        --# Vulkan SDK #--
        "%{VULKAN_SDK}/include/"
    }

    libdirs {
        --# Vulkan SDK #--
        "%{VULKAN_SDK}/lib/"
    }

    links {
        --# Vendor #--
        "GLFW"
    }

    filter "action: vs*"
        pchheader("mfve_pch.h")
        pchsource(_enginefiledir .. "src/mfve_pch.cpp")

    filter "action: not vs*"
        pchheader ("mfve_pch.h")

    filter "system:linux"
    links {
        --# OS Links #--
        "dl",
        "pthread",
        "X11",
        "Xxf86vm",
        "Xrandr",
        "Xi",

        --# SPIR-V & ShaderC #--
        "shaderc_combined",
        "spirv-cross-core",
        "spirv-cross-glsl",

        --# Vulkan SDK #--
        "vulkan"
    }

    filter "system:windows"
    systemversion "latest"
    defines { "_CRT_SECURE_NO_WARNINGS" }

    links {
        --# OS Links #--
        "kernel32.lib",
        "user32.lib",
        "gdi32.lib",
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "odbc32.lib",
        "odbccp32.lib",

        --# SPIR-V & ShaderC #--
        "shaderc_combined.lib",
        --"shaderc_shared.lib",
        "spirv-cross-core.lib",
        "spirv-cross-glsl.lib",

        --# Vulkan SDK #--
        "vulkan-1.lib",
        "VkLayer_utils.lib"
    }

    filter "configurations:Debug"
    defines { "DEBUG", "MFVE_DEBUG", "MFVE_ENABLE_LOGGER" }
    runtime "Debug"
    symbols "On"

    filter "configurations:Release"
    defines { "NDEBUG", "MFVE_RELEASE", "MFVE_ENABLE_LOGGER" }
    runtime "Release"
    optimize "On"

    filter "configurations:Dist"
    defines { "NDEBUG", "MFVE_DIST" }
    runtime "Release"
    optimize "On"