project "WIPE"
    location "%{wks.location}/Engine/"
    uuid = os.uuid()
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir (_targetdir)
    objdir    (_objdir)

    _enginefiledir = "%{wks.location}/Engine/"

    files
    {
        --# Engine Src #--
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",

        --# glm #--
        "%{IncludeDir.glm}/glm/**.hpp",
        "%{IncludeDir.glm}/glm/**.inl"
    }

    includedirs
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
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