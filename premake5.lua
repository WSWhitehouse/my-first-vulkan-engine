include "vendor/vendor.lua"

--# Workspace #--
workspace "WIPE"
    architecture "x86_64"
    startproject "Engine"

    configurations {"Debug", "Release"}

--# Directories #--
_outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
_targetdir = "%{wks.location}/bin/" .. _outputdir
_objdir    = "%{wks.location}/bin-int/" .. _outputdir

--# Dependencies #--
include "vendor/GLFW-premake"

--# Projects #--
include "Engine/engine-premake"