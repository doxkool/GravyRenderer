#include "Input.h"

namespace Input
{
    GLFWwindow* p_window        = nullptr;
    int keyBuffer               = 0;
    int scanCodeBuffer          = 0;
    int actionBuffer            = 0;
    int modBuffer               = 0;
    int mouse_button_buffer     = 0;
    int mouse_action_buffer     = 0;
    int mouse_mod_buffer        = 0;
    int mouse_ScrollX_buffer    = 0;
    int mouse_ScrollY_buffer    = 0;
    double mousePosX            = 0.0;
    double mousePosy            = 0.0;
    bool mouseCaptured          = false;

    void BindWindow(GLFWwindow* window)
    {
        p_window = window;
    }

    void RecieveKeyCallback(int key, int scancode, int action, int mods)
    {
        //keysBuffer.push_back(key);
        keyBuffer = key;
        scanCodeBuffer = scancode;
        actionBuffer = action;
        modBuffer = mods;
    }

    void RecieveMouseCallback(double xpos, double ypos)
    {
        mousePosX = xpos;
        mousePosy = ypos;
    }

    void RecieveMouseButtonCallback(int button, int action, int mods)
    {
        mouse_button_buffer = button;
        mouse_action_buffer = action;
        mouse_mod_buffer = mods;

        #ifdef TRACE_INPUT
            LOG_TRACE("Mouse Button : {} Mouse Action : {} Mouse Mods : {}", mouse_button_buffer, mouse_action_buffer, mouse_mod_buffer);
        #endif
    }

    void RecieveMouseScrollCallback(double xoffset, double yoffset)
    {
        mouse_ScrollX_buffer = xoffset;
        mouse_ScrollY_buffer = yoffset;
        #ifdef TRACE_INPUT
            LOG_TRACE("Mouse Scroll X : {} Mouse Scroll Y : {}", mouse_ScrollX_buffer, mouse_ScrollY_buffer);
        #endif
    }

    bool IsKeyPressed(int key)
    {
        auto action = glfwGetKey(p_window, key);
        if (action == key_pressed || action == key_repeated)
        {
            return true;
        }

        return false;
    }

    bool IsKeyJustPressed(int key)
    {
        if (keyBuffer == key && actionBuffer == key_pressed)
        {
            ResetKeyBuffer();
            return true;
        }
        return false;
    }

    bool IsKeyJustReleased(int key)
    {
        if (keyBuffer == key && actionBuffer == 0)
        {
            ResetKeyBuffer();
            return true;
        }
        return false;
    }

    bool IsMouseButtonJustPressed(int button)
    {
        if (mouse_button_buffer == button && mouse_action_buffer == button_pressed)
        {
            ResetMouseBuffer();
            return true;
        }
        return false;
    }

    bool IsMouseButtonPressed(int button)
    {
        if (mouse_button_buffer == button && mouse_action_buffer == button_pressed)
        {
            return true;
            ResetMouseBuffer();
        }
        return false;
    }

    void SetCursorMode(CursorMode mode)
    {
        if(mode == grabed)
        {
            glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseCaptured = true;
            return;
        }

        if(mode == released)
        {
            glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseCaptured = false;
        }
    }

    CursorMode GetCursorMode()
    {
        return released;
    }

    bool IsMouseGrabed()
    {
        return mouseCaptured;
    }

    glm::vec2 GetMouseCursorPosition()
    {
        return glm::vec2(mousePosX, mousePosy);
    }

    void SetMouseCursorPosition()
    {
    }

    void ResetKeyBuffer()
    {
        keyBuffer = 0;
        scanCodeBuffer = 0;
        actionBuffer = 0;
        modBuffer = 0;
    }

    void ResetMouseBuffer()
    {
        mouse_button_buffer = 0;
        mouse_action_buffer = 0;
        mouse_mod_buffer = 0;
        mouse_ScrollX_buffer = 0;
        mouse_ScrollY_buffer = 0;
    }
}