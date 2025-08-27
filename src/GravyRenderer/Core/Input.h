#pragma once

#include "Core/Defines.h"

#include "Base.h"
#include "Keycode.h"
#include "Window/Window.h"

#include <glm/glm.hpp>

enum key_actions {
    key_none = 0,
    key_pressed,
    key_repeated,
    key_released    
};

enum mouse_button_actions {
    button_none = 0,
    button_pressed,
    button_released
};

enum CursorMode {
    grabed = 0,
    released
};

namespace Gravy
{
    class Input
    {
    private:
    public:
        static void RecieveKeyCallback(int key, int scancode, int action, int mods);
        static void RecieveMouseCallback(double xpos, double ypos);
        static void RecieveMouseButtonCallback(int button, int action, int mods);
        static void RecieveMouseScrollCallback(double xoffset, double yoffset);

        static bool IsKeyPressed(int key);
        static bool IsKeyJustPressed(int key);
        static bool IsKeyJustReleased(int key);

        static bool IsMouseButtonPressed(int button);
        static bool IsMouseButtonJustPressed(int button);

        static void SetCursorMode(Window *p_window, CursorMode mode);
        static CursorMode GetCursorMode(Window *p_window);

        static glm::vec2 GetMouseCursorPosition();
        static void SetMouseCursorPosition();

        static void ResetKeyBuffer();
        static void ResetMouseBuffer();
    };
}