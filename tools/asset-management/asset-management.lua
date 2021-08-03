project "Asset-Management"
    _assetmanagementdir = _tooldir .. "asset-management/"

    location (_assetmanagementdir)
    uuid (os.uuid())
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir (_targetdir)
    objdir    (_objdir)

    files
    {
        --# App Src #--
        _assetmanagementdir .. "src/**.h",
        _assetmanagementdir .. "src/**.c",
        _assetmanagementdir .. "src/**.hpp",
        _assetmanagementdir .. "src/**.cpp",
    }

    includedirs
    {
        _assetmanagementdir .. "src/",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
