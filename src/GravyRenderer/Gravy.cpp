#include "Gravy.h"

namespace Gravy
{
    bool b_RendererInit = false;
    Window m_Window;
    GrvConfInit* m_pconfInit = nullptr;

    // Use this first to initialize the renderer with the structs GrvConfInit and GrvConfWindow. Return e_GrvReturnTypes : success or fail
    e_GrvReturnTypes Init(GrvConfInit* confInit, GrvConfWindow* confWindow)
    {
        m_pconfInit = confInit;
        
        Logger::Init(confWindow->windowName);
        m_Window.Init(confInit);

        m_Window.Create(confWindow);

        if(m_pconfInit->renderingAPI == Opengl)
        {
            LOG_INFO("Initating OpenGL renderer...");
            OpenGL::Init(m_pconfInit);
            OpenGL::SetFrameBufferRes(confWindow->windowResX, confWindow->windowResY);
        }

        if(m_pconfInit->renderingAPI == Vulkan)
        {
            LOG_CRITICAL("Vulkan is not implemented yet!");
            return fail;
        }

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        b_RendererInit = true;

        auto nativeWindow = GetWindowInst()->GetGLFW()->GetNativeWindow();
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();

        return success;
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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // Call at the end of each frame to swap the framebuffers.
    void EndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_Window.SwapScreenBuffer();

        if (!IsRunning()) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
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
}