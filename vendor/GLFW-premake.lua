project "GLFW"
    location "%{wks.location}/vendor/GLFW/"
    kind "StaticLib"
    language "C"
    systemversion "latest"
    staticruntime "On"

    targetdir (_targetdir)
    objdir    (_objdir)

    _glfwfiledir = "%{wks.location}/vendor/GLFW/"

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
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"