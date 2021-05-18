project "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir (_targetdir .. "/%{prj.name}")
    objdir    (_objdir .. "/%{prj.name}")

    _enginefiledir = "%{wks.location}/Engine/"

    files
    {
        --# Engine Src #--
        _enginefiledir .. "src/**.h",
        _enginefiledir .. "src/**.c",
        _enginefiledir .. "src/**.hpp",
        _enginefiledir .. "src/**.cpp",

        --# glm #--
        "%{IncludeDir.glm}/glm/**.hpp",
        "%{IncludeDir.glm}/glm/**.inl"
    }

    includedirs
    {
        _enginefiledir .. "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.VulkanSDK}"
    }

    links
    {
        "GLFW"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"