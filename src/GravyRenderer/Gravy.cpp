#include "Gravy.h"

namespace Gravy
{
    bool b_RendererInit = false;
    Window m_Window;
    RendererSpec* pRendererSpec = nullptr;
    WindowSpec* pWindowSpec = nullptr;
    Camera* camera = nullptr;

    // Use this first to initialize the renderer with the structs RendererSpec and WindowSpec. Return e_GrvReturnTypes : success or fail
    e_ReturnTypes Init(RendererSpec* rendererSpec)
    {
        pRendererSpec = rendererSpec;
        pWindowSpec = &rendererSpec->windowSpec;
        
        Logger::Init(pWindowSpec->windowName);
        m_Window.Init(rendererSpec);

        m_Window.Create(pWindowSpec);

        if(pRendererSpec->renderingAPI == Opengl)
        {
            LOG_INFO("Initating OpenGL renderer...");
            OpenGL::Init(pRendererSpec);
            OpenGL::SetFrameBufferRes(pWindowSpec->windowResX, pWindowSpec->windowResY);
        }

        if(pRendererSpec->renderingAPI == Vulkan)
        {
            LOG_CRITICAL("Vulkan is not implemented yet!");
            return fail;
        }

        return success;
    }

    RendererSpec* GetInitConfig()
    {
        return pRendererSpec;
    }

    WindowSpec* GetWindowConfig()
    {
        return pWindowSpec;
    }

    // Use to close the window.
    void CloseWindow()
    {
        GetWindowInst()->GetGLFW()->SetShouldWindowClose();
    }

    // Call Shutdown() to shutdown the renderer.
    void Shutdown()
    {
        LOG_DEBUG("Shuting down... bye!");

        m_Window.Quit();
    }

    // Call this function at each new frame to clear the framebuffer and poll the new event from the window.
    void NewFrame()
    {
        Time::UpdateDeltaTime();

        OpenGL::ClearBuffer();
        m_Window.Update();
    }

    // Call at the end of each frame to swap the framebuffers.
    void EndFrame()
    {
        m_Window.SwapScreenBuffer();
    }

    // Return a pointer to the Window class.
    Window* GetWindowInst()
    {
        return &m_Window;
    }

    // Swap the screen framebuffers.
    void SwapScreenBuffer()
    {
        m_Window.SwapScreenBuffer();
    }

    void SetVsync(bool enable)
    {
        m_Window.GetGLFW()->EnableVsync(enable);
    }

    bool* GetVsync()
    {
        return m_Window.GetGLFW()->IsVsyncEnable();
    }

    glm::vec2 GetCurrentResolution()
    {
        return m_Window.GetGLFW()->GetCurrentResolution();
    }

    // Return true if the renderer is runing or false if not.
    bool IsRunning()
    {
        return !m_Window.GetWindowShouldClose();
    }

    // Use to set the clear color of the framebuffer.
    void SetClearColor(glm::vec4 color)
    {
        OpenGL::SetClearColor(color);
    }

    // Clear the framebuffer.
    void ClearBuffer()
    {
        OpenGL::ClearBuffer();
    }

    float GetDeltaTime()
    {
        return Time::GetDeltaTime();
    }

    void SetMainCamera(Camera* cam)
    {
        camera = cam;
    }

    Camera* GetMainCamera()
    {
        return camera;
    }
}