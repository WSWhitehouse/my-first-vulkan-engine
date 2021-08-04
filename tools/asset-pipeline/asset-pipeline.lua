project "Asset-Pipeline"
    _assetpipelinedir = _tooldir .. "asset-pipeline/"

    location (_assetpipelinedir)
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
        _assetpipelinedir .. "src/**.h",
        _assetpipelinedir .. "src/**.c",
        _assetpipelinedir .. "src/**.hpp",
        _assetpipelinedir .. "src/**.cpp",
    }

    includedirs
    {
        _assetpipelinedir .. "src/",
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
