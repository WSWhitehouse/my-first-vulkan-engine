--# Workspace #--
workspace "my-first-vulkan-engine"
    architecture "x86_64"
    startproject "MFVE"

    configurations { "Debug", "Release", "Dist" }

    --# Directories #--
    _outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"
    _targetdir = "%{wks.location}/bin/" .. _outputdir
    _objdir    = "%{wks.location}/bin-int/" .. _outputdir

    _assetdir = "%{wks.location}/assets/"

    --# Dependencies #--
    include "vendor/vendor.lua"

    --# Projects #--
    include "mfve/engine-premake"
    include "app/app-premake"