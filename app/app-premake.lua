project "App"
    _appfiledir = "%{wks.location}/app/"

    location (_appfiledir)
    uuid (os.uuid())
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir (_targetdir)
    objdir    (_objdir)

    defines
    {
        --# GLFW #--
        "GLFW_INCLUDE_VULKAN",

        --# glm #--
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE"
    }

    files
    {
        --# App Src #--
        _appfiledir .. "src/**.h",
        _appfiledir .. "src/**.c",
        _appfiledir .. "src/**.hpp",
        _appfiledir .. "src/**.cpp",

        --# glm #--
        "%{IncludeDir.glm}/glm/**.hpp",
        "%{IncludeDir.glm}/glm/**.inl",

        --# Cpp Log #--
        "%{IncludeDir.CppLog}/CppLog/**.h"
    }

    includedirs
    {
        _appfiledir .. "src/",

        --# MFVE #--
        _enginefiledir .. "src/",

        --# Vendor #--
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.CppLog}",
    }

    libdirs
    {
        --# Vulkan SDK #--
        "%{VULKAN_SDK}/lib/"
    }

    links
    {
        --# MFVE #--
        "MFVE"
    }

    filter "action:vs*"
        pchheader  ("pch.h")
        pchsource  (_enginefiledir .. "src/pch.cpp")

    filter "action:not vs*"
        --pchheader ("pch.h")

    filter "system:linux"
        links
        {
            --# OS Links #--
            "dl",
            "pthread",
            "X11",
            "Xxf86vm",
            "Xrandr",
            "Xi",

            --# Vulkan SDK #--
            "vulkan"
        }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS" }

        links
        {
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