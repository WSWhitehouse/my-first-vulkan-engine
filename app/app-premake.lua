project (AppName)
    _appfiledir = "%{wks.location}/app/"

    location (_appfiledir)
    uuid (os.uuid())
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    --staticruntime "on"

    targetdir (_apptargetdir)
    objdir    (_appobjdir)

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
        "%{IncludeDir.CppLog}/CppLog/**.h",

        --# whereami #--
        "%{IncludeDir.whereami}/**.h",
        "%{IncludeDir.whereami}/**.c"
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
        "%{IncludeDir.whereami}",

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
        --# MFVE #--
        "MFVE",

        --# Vendor #--
        "GLFW"
    }

    filter "action: vs*"
        pchheader  ("pch.h")
        pchsource  (_enginefiledir .. "src/pch.cpp")

    filter "action: not vs*"
        pchheader ("pch.h")

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

            --# SPIR-V & ShaderC #--
            "shaderc_combined",
            "spirv-cross-core",
            "spirv-cross-glsl",

            --# Vulkan SDK #--
            "vulkan"
        }

    filter "system:windows"
        systemversion "latest"

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

            --# SPIR-V & ShaderC #--
            "shaderc_combined.lib",
            "spirv-cross-core.lib",
            "spirv-cross-glsl.lib",

            --# Vulkan SDK #--
            "vulkan-1.lib",
            "VkLayer_utils.lib"
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release" 
        runtime "Release"
        optimize "On"

-- Must change runtime to Release for Windows.
    filter { "configurations:Debug", "system:windows" }
        runtime "Release"

    filter { "configurations:Debug", "system:not windows" }
        runtime "Debug"

-- Post build command to generate assets (currently only moving them to target dir)
    filter { "configurations:Release", "system:not windows" }
        postbuildcommands{
            "{ECHO} Generating Assets...",

            -- Removing pre-existing assets & cache folder
            "{RMDIR} " .. _apptargetdir .. "/assets",
            "{RMDIR} " .. _apptargetdir .. "/cache",

            -- Copying assets directory to app target directory
            "{COPYDIR} " .. _assetdir .. " " .. _apptargetdir,

            "{ECHO} Finished Generating Assets"
        }

    filter { "configurations:Release", "system:windows" }
        postbuildcommands{
            "{ECHO} Generating Assets...",

            -- Removing pre-existing assets & cache folder
            "{RMDIR} " .. _apptargetdir .. "/assets",
            "{RMDIR} " .. _apptargetdir .. "/cache",

            -- Copying assets directory to app target directory
            "{MKDIR} " .. _apptargetdir .. "/assets",
            "{COPYDIR} " .. _assetdir .. " " .. _apptargetdir .. "/assets",

            "{ECHO} Finished Generating Assets"
        }
