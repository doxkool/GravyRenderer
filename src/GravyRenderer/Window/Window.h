#pragma once

#include "Core/Logger.h"
#include "Core/Defines.h"

#include "Window/GLFW/glfw.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Renderer
{
    class Window
    {
    private:
        glfw m_glfw;
    public:
        Window() {}
        ~Window() {}

        int Init(RendererSpec* confInit);

        void Create(WindowSpec* confWindow);
        void SwapScreenBuffer();

        bool GetWindowShouldClose();

        glfw* GetGLFW() { return &m_glfw; }

        void ChangeResolution(glm::vec2 resolution);
        void ChangeWindowMode(e_WindowMode windowMode);

        static WindowSpec* GetWindowSettings();

        void Update();

        int PollKeyInput(int key);

        void Quit();
    };
}