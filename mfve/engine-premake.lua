_enginefiledir = "%{wks.location}/mfve/"

project "MFVE"
    location = _enginefiledir
    uuid = os.uuid()
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir (_targetdir)
    objdir    (_objdir)

    pchheader = _enginefiledir .. "src/mfve_pch.h"
    pchsource = _enginefiledir .. "src/mfve_pch.cpp"

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
        "%{IncludeDir.CppLog}/log/**.h"
    }

    includedirs
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.CppLog}",
        "%{IncludeDir.VulkanSDK}"
    }

    links
    {
        "GLFW",
        "%{Lib.VulkanSDK}"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"

    filter "system:linux"
        links
        {
            "dl",
            "pthread",
            "X11",
            "Xxf86vm",
            "Xrandr",
            "Xi"
        }