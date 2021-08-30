project "MFVE"
    _enginefiledir = "%{wks.location}/mfve/"

    location(_enginefiledir)
    uuid(os.uuid())
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    --staticruntime "on"

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
        "%{IncludeDir.CppLog}/CppLog/**.h",

        --# whereami #--
        "%{IncludeDir.whereami}/**.h",
        "%{IncludeDir.whereami}/**.c"
    }

    includedirs {
        _enginefiledir .. "src/",

        --# Vendor #--
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.CppLog}",
        "%{IncludeDir.whereami}",

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
        defines { "_CRT_SECURE_NO_WARNINGS", "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH" }

        links {
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
        defines { "MFVE_DEBUG", "MFVE_ENABLE_LOGGER", "MFVE_ENABLE_VK_VALIDATION" }
        symbols "On"

    filter "configurations:Release"
        defines { "MFVE_RELEASE" }
        runtime "Release"
        optimize "On"

-- Must change runtime to Release for Windows.
    filter { "configurations:Debug", "system:windows" }
        runtime "Release"

    filter { "configurations:Debug", "system:not windows" }
        runtime "Debug"
