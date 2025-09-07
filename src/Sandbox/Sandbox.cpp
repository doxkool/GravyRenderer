#include "Gravy.h"

#include "ImGUI.h"
#include "Audio.h"

Audio m_Audio;
ImGUI m_ImGUI;

Gravy::Camera MainCam;
int Audio1ID = -1;

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
        if (Gravy::Input::IsMouseGrabed())
        {
            Gravy::Input::SetCursorMode(released);
        }
        else
        {
            Gravy::Input::SetCursorMode(grabed);
        }
    }

    if (Gravy::Input::IsKeyPressed(KEY_W))              { MainCam.Move(FORWARD); }
    if (Gravy::Input::IsKeyPressed(KEY_S))              { MainCam.Move(BACKWARD); }
    if (Gravy::Input::IsKeyPressed(KEY_A))              { MainCam.Move(LEFT); }
    if (Gravy::Input::IsKeyPressed(KEY_D))              { MainCam.Move(RIGHT); }
    if (Gravy::Input::IsKeyPressed(KEY_SPACE))          { MainCam.Move(UP); }
    if (Gravy::Input::IsKeyPressed(KEY_LEFT_CONTROL))   { MainCam.Move(DOWN); }

    if(Gravy::Input::IsKeyJustPressed(KEY_1))
    {
        m_Audio.PlayAudioTrack(Audio1ID);
    }

    if(Gravy::Input::IsKeyJustPressed(KEY_0))
    {
        m_Audio.StopAllAudio();
    }

    MainCam.EnableMouseInput(Gravy::Input::IsMouseGrabed());

}

void Run()
{
    Gravy::SetClearColor(GRAY);

    bool vsync = Gravy::GetVsync();

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

        MainCam.Update();

        CheckForInput();

        ImGui::Begin("Perf Monitor");
        ImGui::Text("%.1f FPS | %.3f Miliseconds", ImGui::GetIO().Framerate, 1 / ImGui::GetIO().Framerate * 1000.0f);
        ImGui::SameLine();
        if(ImGui::Checkbox("vSync", &vsync)) { Gravy::SetVsync(vsync); };
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