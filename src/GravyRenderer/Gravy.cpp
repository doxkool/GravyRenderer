#include "Gravy.h"

namespace Gravy
{
    bool b_RendererInit = false;
    Window m_Window;
    GrvConfInit* m_pconfInit = nullptr;
    Audio m_Audio;

    // Use this first to initialize the renderer with the structs GrvConfInit and GrvConfWindow. Return e_GrvReturnTypes : success or fail
    e_GrvReturnTypes Init(GrvConfInit* confInit, GrvConfWindow* confWindow)
    {
        m_pconfInit = confInit;
        
        Logger::Init(confWindow->windowName);
        m_Window.Init(confInit);
        m_Audio.Init();

        m_Window.Create(confWindow);

        if(m_pconfInit->renderingAPI == Opengl)
        {
            LOG_INFO("Initating OpenGL renderer...");
            OpenGL::Init(m_pconfInit);
        }

        if(m_pconfInit->renderingAPI == Vulkan)
        {
            LOG_CRITICAL("Vulkan is not implemented yet!");
            return fail;
        }

        b_RendererInit = true;

        return success;
    }

    // Call Shudown() to close the renderer.
    void Shutdown()
    {
        LOG_DEBUG("Shuting down... bye!");
        m_Window.Quit();
    }

    // Call this function at each new frame to clear the framebuffer and poll the new event from the window.
    void NewFrame()
    {
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

    // Return true if the renderer is runing or false if not.
    bool IsRunning()
    {
        return !m_Window.GetWindowShouldClose();
    }

    int LoadAudioTrack(t_AudioTrackInfo* audioTrackInfo)
    {
        return m_Audio.LoadAudioTrack(audioTrackInfo);
    }

    void PlayAudioTrack(int id)
    {
        m_Audio.PlayAudioTrack(id);
    }

    void PlayAudio(const char* filename, bool loop)
    {
        m_Audio.PlayAudio(filename, loop);
    }

    void StopAllAudio()
    {
        m_Audio.StopAllAudio();
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
}