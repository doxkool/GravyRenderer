#pragma once

#include "Core/Base.h"
#include "Core/ConfigsDef.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

namespace Gravy
{
    class glfw
    {
    private:

    public:
        glfw();
        ~glfw();

        void InitWindow(GrvConfWindow* confWindow);
        void SetWindowIcon(const std::string &IconPath);
        void UpdateWindowName(std::string newTitle);
        void SwapBuffer();
        void EnableVsync(bool enabled);

        void GrabMouseInput(bool enable);

        bool GetShouldWindowClose();
        void SetShouldWindowClose();

        void SetWindowResolution(glm::vec2 resolution);
        void SetWindowMode(e_WindowMode windowMode);

        glm::vec2 GetWindowScalling();

        GLFWwindow* GetNativeWindow() { return window; }

        void Update();

        static double GetTime();

        GLFWwindow* window;
        int window_width, window_height;

        std::string m_windowName;
        bool mouseCaptured;
    };
}