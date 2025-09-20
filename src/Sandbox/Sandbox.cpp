#include "Gravy.h"

#include "ImGUI.h"
#include "Audio.h"

using namespace Renderer;

Window* m_window = nullptr;
Camera MainCam;

Audio m_Audio;
ImGUI m_ImGUI;
int Audio1ID = -1;

void CheckForInput()
{
    auto m_Window = GetWindowInst();

    if (Input::IsKeyJustPressed(KEY_GRAVE_ACCENT))
    {
        LOG_INFO("Escape key pressed, exiting Sandbox...");
        CloseWindow();
    }

    if (Input::IsMouseButtonJustPressed(MOUSE_RIGHT_CLICK))
    {
        if (Input::IsMouseGrabed())
        {
            Input::SetCursorMode(released);
        }
        else
        {
            Input::SetCursorMode(grabed);
        }
    }

    if (Input::IsKeyPressed(KEY_W))              { MainCam.Move(FORWARD); }
    if (Input::IsKeyPressed(KEY_S))              { MainCam.Move(BACKWARD); }
    if (Input::IsKeyPressed(KEY_A))              { MainCam.Move(LEFT); }
    if (Input::IsKeyPressed(KEY_D))              { MainCam.Move(RIGHT); }
    if (Input::IsKeyPressed(KEY_SPACE))          { MainCam.Move(UP); }
    if (Input::IsKeyPressed(KEY_LEFT_CONTROL))   { MainCam.Move(DOWN); }

    if(Input::IsKeyJustPressed(KEY_1))
    {
        m_Audio.PlayAudioTrack(Audio1ID);
    }

    if(Input::IsKeyJustPressed(KEY_0))
    {
        m_Audio.StopAllAudio();
    }

    MainCam.b_MouseInput = Input::IsMouseGrabed();
}

void Run()
{
    m_window = GetWindowInst();

    SetClearColor(GRAY);

    t_AudioTrackInfo audio1 = {
        .filePath   = "assets/musics/Ice_and_Snow.mp3",
        .loop       = true,
        .audioBus   = 0
    };
    Audio1ID = m_Audio.LoadAudioTrack(&audio1);

    Model floor0(Quad);
    Shader floor0Shader;
    floor0Shader.loadShader(FLAT_VER_SHADER, FLAT_FRAG_SHADER);
    floor0.SetTransform({0.f, -2.0f, 0.0f}, {90.f, 0.0f, 0.0f}, {10.0f, 10.0f, 10.0f});

    Model cube0(Cube);
    Shader cube0Shader;
    cube0Shader.loadShader(FLAT_VER_SHADER, FLAT_FRAG_SHADER);

    Light light0;
    light0.CreateShadowMap();

    MainCam.Position = {0.0f, 0.0f, -4.0f};
    MainCam.MovementSpeed = 25.0f;
    SetMainCamera(&MainCam);
    
    while (IsRunning())
    {
        CheckForInput();

        cube0.Rotate({0.0f, 50.0f, 0.0f});

        // --- Render Scene
        NewFrame();

        MainCam.Update();

        cube0.UpdateMatrix();
        floor0.UpdateMatrix();

        cube0.Render(&cube0Shader, &MainCam);
        floor0.Render(&floor0Shader, &MainCam);

        m_ImGUI.RenderGUI();

        EndFrame();
    }
}

int main()
{
    WindowSpec windowSpec = {
        .windowName         = "Sandbox",
        .windowResX         = 1920,
        .windowResY         = 1080,
        .windowMode         = windowed,
        .vsync              = true,
        .windowResizable    = true,
        .transparentFB      = false
    };

    RendererSpec rendererSpec = {
        .windowSpec = windowSpec,
        .renderingAPI = Opengl,
        .apiVersionMajor = 4,
        .apiVersionMinor = 6,
        .apiEnableMessageCallBack = true,
        .apiLoggingLevel = trace
    };
    
    int ret_Gravy = Init(&rendererSpec);

    m_Audio.Init();
    m_ImGUI.Init();

    if(ret_Gravy == 0)
    {
        Run();
    }

    Shutdown();

    m_ImGUI.Shutdown();
    m_Audio.Shutdown();

    return 0;
}