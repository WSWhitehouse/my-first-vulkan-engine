include "vendor/vendor.lua"

--# Workspace #--
workspace "Vk-Test"
    architecture "x86_64"
    startproject "Engine"

    configurations {"Debug", "Release"}

--# Directories #--
_outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
_targetdir = "%{wks.location}/bin/" .. _outputdir
_objdir    = "%{wks.location}/bin-int/" .. _outputdir

--# Dependencies #--
include "vendor/GLFW"

--# Projects #--
include "Engine"