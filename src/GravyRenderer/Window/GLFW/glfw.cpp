#include "Graphics/Renderer/OpenGL/OpenGL.h"
#include "glfw.h"

#include "Core/Input.h"

#include <stb_image.h>

namespace Renderer
{
    void error_callback(int error_code, const char* description)
    {
        LOG_ERROR("\nERROR::{} >> {}", error_code, description);
    }

    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        OpenGL::SetFrameBufferRes(width, height);
    }
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        // auto state = glfwGetKey(window, key);
        Input::RecieveKeyCallback(key, scancode, action, mods);
    }

    void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
        Input::RecieveMouseCallback(xpos, ypos);
    }

    void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        Input::RecieveMouseScrollCallback(xoffset, yoffset);
    }

    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        Input::RecieveMouseButtonCallback(button, action, mods);
    }

    int glfw::Init(RendererSpec* confInit)
    {
        /* Initialize the library */
        if (!glfwInit())
        {
            LOG_ERROR("ERROR! GLFW_INIT_FAILED");
            return -1;
        }

        #ifdef DEBUG
            //LOG_WARN("Running in Debug mode!");
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        #endif

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, confInit->apiVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, confInit->apiVersionMinor);

        return 0;
    }

    void glfw::Shutdown()
    {
        glfwTerminate();
    }

    int glfw::Create(WindowSpec* confWindow)
    {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (!primaryMonitor) {
            // Handle error getting primary monitor
            LOG_CRITICAL("GLFW ERROR :: Failed to get the primary monitor!");
            glfwTerminate();
            return -1;
        }

        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

        m_windowName = confWindow->windowName;
        glfwWindowHint(GLFW_RESIZABLE, confWindow->windowResizable);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, confWindow->transparentFB);

        switch (confWindow->windowMode)
        {
        case windowed:
            window = glfwCreateWindow(confWindow->windowResX, confWindow->windowResY, confWindow->windowName, NULL, NULL);
            window_width = confWindow->windowResX;
            window_height = confWindow->windowResY;
            break;
        
        case borderless:
            LOG_WARN("Borderless mode is not supported yet!");
            window = glfwCreateWindow(confWindow->windowResX, confWindow->windowResY, confWindow->windowName, NULL, NULL);
            break;
        
        case fullscreen:
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            glfwWindowHint(GLFW_FLOATING, true);

            window = glfwCreateWindow(mode->width, mode->height, confWindow->windowName, primaryMonitor, NULL);
            window_width = mode->width;
            window_height = mode->height;
            break;
        
        default:
            window = glfwCreateWindow(confWindow->windowResX, confWindow->windowResY, confWindow->windowName, NULL, NULL);
            window_width = confWindow->windowResX;
            window_height = confWindow->windowResY;
            break;
        }

        #ifdef LINUX_WAYLAND
            glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
            glfwWindowHintString(GLFW_WAYLAND_APP_ID, confWindow->windowName);
        #endif
        #ifdef LINUX_X11
            glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
            glfwWindowHintString(GLFW_X11_CLASS_NAME, confWindow->windowName);
            glfwWindowHintString(GLFW_X11_INSTANCE_NAME, confWindow->windowName);
        #endif

        if (!window)
        {
            glfwTerminate();
            LOG_CRITICAL("ERROR! FAILED TO CREATE WINDOW");
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        // Setting the callbacks
        glfwSetErrorCallback(error_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, mouse_scroll_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);

        Input::BindWindow(this);

        return 0;
    }

    void glfw::SetWindowIcon(const std::string &IconPath)
    {
        if(IconPath.c_str())
        {
            GLFWimage images[1];
            images[0].pixels = stbi_load(IconPath.c_str(), &images[0].width, &images[0].height, 0, 4); //rgba channels 
            glfwSetWindowIcon(window, 1, images); 
            stbi_image_free(images[0].pixels);
        }
    }

    void glfw::UpdateWindowName(std::string newTitle)
    {
        glfwSetWindowTitle(window, newTitle.c_str());
    }

    void glfw::SwapBuffer()
    {
        glfwSwapBuffers(window);
    }

    void glfw::EnableVsync(bool enabled)
    {
        if(enabled)
        {
            glfwSwapInterval(1);
        }else{
            glfwSwapInterval(0);
        }
    }

    bool* glfw::IsVsyncEnable()
    {
        return &b_vsync;
    }

    void glfw::GrabMouseInput(bool enable)
    {
        if (enable)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseCaptured = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseCaptured = false;
        }
    }

    bool glfw::IsMouseGrabed()
    {
        return mouseCaptured;
    }

    glm::vec2 glfw::GetCurrentResolution()
    {
        return OpenGL::GetFrameBufferRes();
    }

    bool glfw::GetShouldWindowClose()
    {
        return glfwWindowShouldClose(window);
    }

    void glfw::SetShouldWindowClose()
    {
        glfwSetWindowShouldClose(window, true);
    }

    void glfw::SetWindowResolution(glm::vec2 resolution)
    {
        glfwSetWindowSize(window, resolution.x, resolution.y);
        OpenGL::SetFrameBufferRes(resolution.x, resolution.y);
    }

    void glfw::SetWindowMode(e_WindowMode windowMode)
    {

    }

    glm::vec2 glfw::GetWindowScalling()
    {
        glm::vec2 scalling = glm::vec2(0, 0);

        glfwGetWindowContentScale(window, &scalling.x, &scalling.y);

        return scalling;
    }

    void glfw::Update()
    {
        glfwPollEvents();
    }

    int glfw::PollKeyInput(int key)
    {
        return glfwGetKey(window, key);
    }

    double glfw::GetTime()
    {
        return glfwGetTime();
    }
}