include "vendor/vendor.lua"

--# Workspace #--
workspace "my-first-vulkan-engine"
    architecture "x86_64"
    startproject "mfve"

    configurations {"Debug", "Release"}

--# Directories #--
_outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
_targetdir = "%{wks.location}/bin/" .. _outputdir
_objdir    = "%{wks.location}/bin-int/" .. _outputdir

--# Dependencies #--
include "vendor/premake"
include "vendor/GLFW-premake"

--# Projects #--
include "mfve/engine-premake"