#include "Gravy.h"

#include "ImGUI.h"
#include "Audio.h"

Audio m_Audio;
ImGUI m_ImGUI;

Gravy::Camera MainCam;
int Audio1ID = -1;
bool vsync = true;
bool b_mouseCaptured = false;

float lastX = 0;
float lastY = 0;

void CheckForInput()
{
    auto m_Window = Gravy::GetWindowInst();

    if (Gravy::Input::IsKeyJustPressed(KEY_GRAVE_ACCENT))
    {
        LOG_INFO("Escape key pressed, exiting Sandbox...");
        Gravy::CloseWindow();
    }

    if (Gravy::Input::IsMouseButtonJustPressed(MOUSE_RIGHT_CLICK))
    {
        if (b_mouseCaptured)
        {
            Gravy::Input::SetCursorMode(m_Window, released);
            b_mouseCaptured = false;
        }
        else
        {
            Gravy::Input::SetCursorMode(m_Window, grabed);
            b_mouseCaptured = true;
        }
    }

    if (Gravy::Input::IsKeyPressed(KEY_W))              { MainCam.Position += MainCam.Front * MainCam.MovementSpeed * Gravy::GetDeltaTime(); }
    if (Gravy::Input::IsKeyPressed(KEY_S))              { MainCam.Position -= MainCam.Front * MainCam.MovementSpeed * Gravy::GetDeltaTime(); }
    if (Gravy::Input::IsKeyPressed(KEY_A))              { MainCam.Position -= MainCam.Right * MainCam.MovementSpeed * Gravy::GetDeltaTime(); }
    if (Gravy::Input::IsKeyPressed(KEY_D))              { MainCam.Position += MainCam.Right * MainCam.MovementSpeed * Gravy::GetDeltaTime(); }
    if (Gravy::Input::IsKeyPressed(KEY_SPACE))          { MainCam.Position += MainCam.Up * MainCam.MovementSpeed * Gravy::GetDeltaTime(); }
    if (Gravy::Input::IsKeyPressed(KEY_LEFT_CONTROL))   { MainCam.Position -= MainCam.Up * MainCam.MovementSpeed * Gravy::GetDeltaTime(); }

    if(Gravy::Input::IsKeyJustPressed(KEY_1))
    {
        m_Audio.PlayAudioTrack(Audio1ID);
    }

    if(Gravy::Input::IsKeyJustPressed(KEY_0))
    {
        m_Audio.StopAllAudio();
    }

    glm::vec2 mousePos = Gravy::Input::GetMouseCursorPosition();

    float xoffset = mousePos.x - lastX;
    float yoffset = lastY - mousePos.y; // reversed since y-coordinates go from bottom to top

    lastX = mousePos.x;
    lastY = mousePos.y;

    if (b_mouseCaptured)
    {
        MainCam.ProcessMouseMovement(xoffset, yoffset, true);
    }
}

void Run()
{
    Gravy::SetClearColor(GRAY);

    t_AudioTrackInfo audio1 = {
        .filePath   = "assets/musics/Ice_and_Snow.mp3",
        .loop       = true,
        .audioBus   = 0
    };
    Audio1ID = m_Audio.LoadAudioTrack(&audio1);

    Gravy::Model cube0;
    cube0.LoadPrimitive(Cube);

    Gravy::Shader cube0Shader;
    cube0Shader.loadShader(FLAT_VER_SHADER, FLAT_FRAG_SHADER);

    MainCam.SetPosition({0.0f, 0.0f, -4.0f});
    
    while (Gravy::IsRunning())
    {
        m_ImGUI.NewFrame();
        Gravy::NewFrame();

        CheckForInput();

        ImGui::Begin("Perf Monitor");
        ImGui::Text("%.1f FPS | %.3f Miliseconds", ImGui::GetIO().Framerate, 1 / ImGui::GetIO().Framerate * 1000.0f);
        ImGui::SameLine();
        if(ImGui::Checkbox("vSync", &vsync)) { Gravy::GetWindowInst()->GetGLFW()->EnableVsync(vsync); };
        ImGui::End();

        cube0.Render(&cube0Shader, &MainCam);

        cube0.Rotation.y = ( cube0.Rotation.y + 50 * Gravy::GetDeltaTime() );

        m_ImGUI.EndFrame();
        Gravy::EndFrame();
    }
}

int main()
{
    GrvConfInit confInit = {
        .renderingAPI = Opengl,
        .apiVersionMajor = 4,
        .apiVersionMinor = 6,
        .apiEnableMessageCallBack = true,
        .apiLoggingLevel = trace
    };

    GrvConfWindow confWindow = {
        .windowName         = "Sandbox",
        .windowResX         = 1920,
        .windowResY         = 1080,
        .windowMode         = windowed,
        .windowResizable    = true,
        .transparentFB      = false
    };
    
    int ret_Gravy = Gravy::Init(&confInit, &confWindow);

    m_Audio.Init();
    m_ImGUI.Init();

    if(ret_Gravy == success)
    {
        Run();
    }

    Gravy::Shutdown();

    m_ImGUI.Shutdown();
    m_Audio.Shutdown();

    return 0;
}