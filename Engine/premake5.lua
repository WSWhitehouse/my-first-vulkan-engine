project "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir (_targetdir .. "/%{prj.name}")
    objdir    (_objdir .. "/%{prj.name}")

    _filedir = "%{wks.location}/Engine/"

    files
    { 
        _filedir .. "src/**.h", 
        _filedir .. "src/**.c", 
        _filedir .. "src/**.hpp", 
        _filedir .. "src/**.cpp" 
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

