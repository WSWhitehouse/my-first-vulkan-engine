--# Application #--

-- Change these variables to customise your app!
-- Don't edit anything below the "Workspace" comment or any other premake/lua files unless you know what you're doing

AppName = "App"  -- Name of your application
AppVer = "1.0.0" -- Version of your application

--# Workspace #--
workspace "my-first-vulkan-engine"
    architecture ("x86_64")
    startproject (AppName)

    configurations { "Debug", "Release", "Dist" }

    --# Directories #--
    _outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

    _targetdir = "%{wks.location}/bin/" .. _outputdir
    _objdir    = "%{wks.location}/bin-int/" .. _outputdir

    _apptargetdir = _targetdir .. AppName .. "-" .. AppVer
    _appobjdir = _objdir .. AppName .. "-" .. AppVer

    _assetdir = "%{wks.location}/assets/"

    --# Dependencies #--
    include "vendor/vendor.lua"

    --# Projects #--
    include "mfve/engine-premake"
    include "app/app-premake"