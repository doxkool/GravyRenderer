#pragma once

#include "Keycode.h"

#include <GLFW/glfw3.h>
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

namespace Input
{
    void BindWindow(GLFWwindow* window);

    void RecieveKeyCallback(int key, int scancode, int action, int mods);
    void RecieveMouseCallback(double xpos, double ypos);
    void RecieveMouseButtonCallback(int button, int action, int mods);
    void RecieveMouseScrollCallback(double xoffset, double yoffset);

    bool IsKeyPressed(int key);
    bool IsKeyJustPressed(int key);
    bool IsKeyJustReleased(int key);
    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonJustPressed(int button);

    void SetCursorMode(CursorMode mode);
    CursorMode GetCursorMode();

    bool IsMouseGrabed();

    glm::vec2 GetMouseCursorPosition();
    void SetMouseCursorPosition();

    void ResetKeyBuffer();
    void ResetMouseBuffer();
}