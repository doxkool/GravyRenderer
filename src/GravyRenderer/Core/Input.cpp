#include "Input.h"

namespace Gravy
{
    int keyBuffer;
    int scanCodeBuffer;
    int actionBuffer;
    int modBuffer;

    int mouse_button_buffer;
    int mouse_action_buffer;
    int mouse_mod_buffer;
    int mouse_ScrollX_buffer;
    int mouse_ScrollY_buffer;


    double mousePosX;
    double mousePosy;

    void Input::RecieveKeyCallback(int key, int scancode, int action, int mods)
    {
        keyBuffer = key;
        scanCodeBuffer = scancode;
        actionBuffer = action;
        modBuffer = mods;
    }

    void Input::RecieveMouseCallback(double xpos, double ypos)
    {
        mousePosX = xpos;
        mousePosy = ypos;
    }

    void Input::RecieveMouseButtonCallback(int button, int action, int mods)
    {
        mouse_button_buffer = button;
        mouse_action_buffer = action;
        mouse_mod_buffer = mods;
        //LOG_TRACE("Mouse Button : {} Mouse Action : {} Mouse Mods : {}",mouse_button_buffer, mouse_action_buffer, mouse_mod_buffer);
    }

    void Input::RecieveMouseScrollCallback(double xoffset, double yoffset)
    {
        mouse_ScrollX_buffer = xoffset;
        mouse_ScrollY_buffer = yoffset;
        //LOG_TRACE("Mouse Scroll X : {} Mouse Scroll Y : {}", mouse_ScrollX_buffer, mouse_ScrollY_buffer);
    }

    bool Input::IsKeyPressed(int key)
    {
        if (keyBuffer == key && (actionBuffer == key_pressed || actionBuffer == key_repeated))
        {
            //LOG_TRACE("Key {} pressed :: Action : {}",keyBuffer, actionBuffer);
            return true;
            ResetKeyBuffer();
        }

        return false;
    }

    bool Input::IsKeyJustPressed(int key)
    {
        if (keyBuffer == key && actionBuffer == key_pressed)
        {
            //LOG_TRACE("Key {} pressed :: Action : {}",keyBuffer, actionBuffer);
            ResetKeyBuffer();
            return true;
        }

        return false;
    }

    bool Input::IsKeyJustReleased(int key)
    {
        if (keyBuffer == key && actionBuffer == 0)
        {
            ResetKeyBuffer();
            return true;
        }

        return false;
    }

    bool Input::IsMouseButtonJustPressed(int button)
    {

        if (mouse_button_buffer == button && mouse_action_buffer == button_pressed)
        {
            //LOG_TRACE("{} | {}", mouse_button_buffer, mouse_action_buffer);
            ResetMouseBuffer();
            return true;
        }

        return false;
    }

    bool Input::IsMouseButtonPressed(int button)
    {

        if (mouse_button_buffer == button && mouse_action_buffer == button_pressed)
        {
            return true;
            ResetMouseBuffer();
        }

        return false;
    }

    void Input::SetCursorMode(Window *p_window, CursorMode mode)
    {
        if(mode == grabed)
        {
            p_window->GetGLFW()->GrabMouseInput(true);
            return;
        }else if(mode == released)
        {
            p_window->GetGLFW()->GrabMouseInput(false);
        }

    }

    CursorMode Input::GetCursorMode(Window *p_window)
    {
        return released;
    }

    glm::vec2 Input::GetMouseCursorPosition()
    {
        return glm::vec2(mousePosX, mousePosy);
    }

    void Input::SetMouseCursorPosition()
    {

    }

    void Input::ResetKeyBuffer()
    {
        keyBuffer = 0;
        scanCodeBuffer = 0;
        actionBuffer = 0;
        modBuffer = 0;
    }

    void Input::ResetMouseBuffer()
    {
        mouse_button_buffer = 0;
        mouse_action_buffer = 0;
        mouse_mod_buffer = 0;
        mouse_ScrollX_buffer = 0;
        mouse_ScrollY_buffer = 0;
    }
}