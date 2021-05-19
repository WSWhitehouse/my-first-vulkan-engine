project "premake"
    kind "Utility"

    targetdir (_targetdir)
    objdir    (_objdir)

    files
	{
		"%{wks.location}/**premake5.lua"
	}

	postbuildmessage "Regenerating project files with Premake5!"

    filter "system:windows"
        postbuildcommands
        {
            "%{prj.location}vendor/premake/bin/premake5 %{_ACTION} --file=\"%{wks.location}premake5.lua\""
        }

    filter "system:linux"
        postbuildcommands
        {
            "premake5 %{_ACTION} --file=\"%{wks.location}premake5.lua\""
        }