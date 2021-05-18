project "GLFW"
    kind "StaticLib"
    language "C"

    targetdir (_targetdir .. "/%{prj.name}")
    objdir    (_objdir .. "/%{prj.name}")

    _glfwfiledir = "%{wks.location}/vendor/GLFW/"

    files
    {
        _glfwfiledir .. "include/GLFW/glfw3.h",
        _glfwfiledir .. "include/GLFW/glfw3native.h",
        _glfwfiledir .. "src/glfw_config.h",
        _glfwfiledir .. "src/context.c",
        _glfwfiledir .. "src/init.c",
        _glfwfiledir .. "src/input.c",
        _glfwfiledir .. "src/monitor.c",
        _glfwfiledir .. "src/vulkan.c",
        _glfwfiledir .. "src/window.c"
    }

    filter "system:linux"
        pic "On"

        systemversion "latest"
        staticruntime "On"

        files
        {
            _glfwfiledir .. "src/x11_init.c",
            _glfwfiledir .. "src/x11_monitor.c",
            _glfwfiledir .. "src/x11_window.c",
            _glfwfiledir .. "src/xkb_unicode.c",
            _glfwfiledir .. "src/posix_time.c",
            _glfwfiledir .. "src/posix_thread.c",
            _glfwfiledir .. "src/glx_context.c",
            _glfwfiledir .. "src/egl_context.c",
            _glfwfiledir .. "src/osmesa_context.c",
            _glfwfiledir .. "src/linux_joystick.c"
        }

        defines
        {
            "_GLFW_X11"
        }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        files
        {
            _glfwfiledir .. "src/win32_init.c",
            _glfwfiledir .. "src/win32_joystick.c",
            _glfwfiledir .. "src/win32_monitor.c",
            _glfwfiledir .. "src/win32_time.c",
            _glfwfiledir .. "src/win32_thread.c",
            _glfwfiledir .. "src/win32_window.c",
            _glfwfiledir .. "src/wgl_context.c",
            _glfwfiledir .. "src/egl_context.c",
            _glfwfiledir .. "src/osmesa_context.c"
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