project "GLFW"
    _glfwfiledir = "%{wks.location}/vendor/GLFW/"

    location (_glfwfiledir)
    kind "StaticLib"
    language "C"
    systemversion "latest"
    --staticruntime "On"

    targetdir (_apptargetdir)
    objdir    (_appobjdir)

    files
    {
        "GLFW/include/GLFW/glfw3.h",
        "GLFW/include/GLFW/glfw3native.h",
        "GLFW/src/glfw_config.h",
        "GLFW/src/context.c",
        "GLFW/src/init.c",
        "GLFW/src/input.c",
        "GLFW/src/monitor.c",
        "GLFW/src/vulkan.c",
        "GLFW/src/window.c"
    }

    filter "system:linux"
        pic "On"

        files
        {
            "GLFW/src/x11_init.c",
            "GLFW/src/x11_monitor.c",
            "GLFW/src/x11_window.c",
            "GLFW/src/xkb_unicode.c",
            "GLFW/src/posix_time.c",
            "GLFW/src/posix_thread.c",
            "GLFW/src/glx_context.c",
            "GLFW/src/egl_context.c",
            "GLFW/src/osmesa_context.c",
            "GLFW/src/linux_joystick.c"
        }

        defines
        {
            "_GLFW_X11"
        }

    filter "system:windows"
        files
        {
            "GLFW/src/win32_init.c",
            "GLFW/src/win32_joystick.c",
            "GLFW/src/win32_monitor.c",
            "GLFW/src/win32_time.c",
            "GLFW/src/win32_thread.c",
            "GLFW/src/win32_window.c",
            "GLFW/src/wgl_context.c",
            "GLFW/src/egl_context.c",
            "GLFW/src/osmesa_context.c"
        }

        defines
        {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS",
            "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH" 
        }

    filter "configurations:Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- Must change runtime to Release for Windows.
    filter { "configurations:Debug", "system:windows" }
        runtime "Release"

    filter { "configurations:Debug", "system:not windows" }
        runtime "Debug"
