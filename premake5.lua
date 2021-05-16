workspace "Vk-Test"
    architecture "x86_64"
    configurations {"Debug", "Release"}

_outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
_targetdir = "%{wks.location}/bin/" .. _outputdir
_objdir    = "%{wks.location}/bin-int/" .. _outputdir


include "Engine"