#pragma once

#include "Core/Logger.h"
#include "Core/Defines.h"

#include "Window/GLFW/glfw.h"

namespace Gravy
{
    class Window
    {
    private:
        glfw m_glfw;
    public:
        Window() {}
        ~Window() {}

        void InitWindow(GrvConfWindow* confWindow);
        void SwapScreenBuffer();

        bool GetWindowShouldClose();

        glfw* GetNativeWindow() { return &m_glfw; }

        void ChangeResolution(glm::vec2 resolution);
        void ChangeWindowMode(e_WindowMode windowMode);

        static GrvConfWindow* GetWindowSettings();

        void Update();

        void Quit();
    };
}