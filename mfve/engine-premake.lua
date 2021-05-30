_enginefiledir = "%{wks.location}/mfve/"

project "MFVE"
    location (_enginefiledir)
    uuid (os.uuid())
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir (_targetdir)
    objdir    (_objdir)

    pchheader  ("mfve_pch.h")
    pchsource  (_enginefiledir .. "src/mfve_pch.cpp")

    defines
    {
        --# glm #--
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE"
    }

    files
    {
        --# mfve Src #--
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",

        --# glm #--
        "%{IncludeDir.glm}/glm/**.hpp",
        "%{IncludeDir.glm}/glm/**.inl",

        --# Cpp Log #--
        "%{IncludeDir.CppLog}/CppLog/**.h"
    }

    includedirs
    {
        "src",

        --# Vendor #--
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.CppLog}",

        --# Vulkan SDK #--
        "%{VULKAN_SDK}/include/"
    } 
    
    libdirs
    {
        --# Vulkan SDK #--
        "%{VULKAN_SDK}/lib/"
    }

    links
    {
        --# Vendor #--
        "GLFW"
    }

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