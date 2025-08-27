#include "Window.h"

namespace Gravy
{
    GrvConfWindow sWindowSettings;

    void Window::InitWindow(GrvConfWindow* confWindow)
    {
        m_glfw.InitWindow(confWindow);

        if(confWindow->iconPath)
        {
            m_glfw.SetWindowIcon(confWindow->iconPath);
        }
    }

    void Window::SwapScreenBuffer()
    {
        m_glfw.SwapBuffer();
    }

    bool Window::GetWindowShouldClose()
    {
        return m_glfw.GetShouldWindowClose();
    }

    void Window::ChangeResolution(glm::vec2 resolution)
    {
        m_glfw.SetWindowResolution(resolution);
    }

    void Window::ChangeWindowMode(e_WindowMode windowMode)
    {
        LOG_WARN("Function 'window::changeWindowMode' not implemented yet...");
    }

    GrvConfWindow* Window::GetWindowSettings()
    {
        return &sWindowSettings;
    }

    void Window::Update()
    {
        m_glfw.Update();
    }

    void Window::Quit()
    {
        m_glfw.SetShouldWindowClose();
    }
}