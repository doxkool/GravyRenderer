#include "Gravy.h"

void Run()
{
    Gravy::SetClearColor(GRAY);

    t_AudioTrackInfo audio1 = {
        .filePath   = "assets/musics/Ice_and_Snow.mp3",
        .loop       = true,
        .audioBus   = 0
    };
    auto IceandSnow = Gravy::LoadAudioTrack(&audio1);

    //t_AudioTrackInfo audio2 = {
    //    .filePath   = "assets/musics/Go_On_Without_Me.mp3",
    //    .loop       = false,
    //    .audioBus   = 0
    //};
    //auto GoOnWithoutMe = Gravy::LoadAudioTrack(&audio2);

    //t_AudioTrackInfo audio3 = {
    //    .filePath   = "assets/musics/Magic_in_the_Garden.mp3",
    //    .loop       = true,
    //    .audioBus   = 0
    //};
    //auto MagicInTheGarden = Gravy::LoadAudioTrack(&audio3);

    Gravy::Model cube0;
    cube0.LoadPrimitive(Cube);

    Gravy::Shader cube0Shader;
    cube0Shader.loadShader(FLAT_VER_SHADER, FLAT_FRAG_SHADER);

    Gravy::Camera camera;
    camera.SetPosition({0.0f, 0.0f, -4.0f});
    
    while (Gravy::IsRunning())
    {
        Gravy::NewFrame();

        if(Gravy::Input::IsKeyJustPressed(KEY_GRAVE_ACCENT))
        {
            Gravy::Shutdown();
        }

        if(Gravy::Input::IsKeyJustPressed(KEY_1))
        {
            Gravy::PlayAudioTrack(IceandSnow);
            Gravy::SetClearColor(DARK_ORANGE);
        }

        if(Gravy::Input::IsKeyJustPressed(KEY_2))
        {
            //Gravy::PlayAudioTrack(GoOnWithoutMe);
            Gravy::SetClearColor(ORANGE);
        }

        if(Gravy::Input::IsKeyJustPressed(KEY_3))
        {
            //Gravy::PlayAudioTrack(MagicInTheGarden);
            Gravy::SetClearColor(RED);
        }

        if(Gravy::Input::IsKeyJustPressed(KEY_0))
        {
            Gravy::StopAllAudio();
            Gravy::SetClearColor(GRAY);
        }

        cube0.Render(&cube0Shader, &camera);

        cube0.Rotation.y = ( cube0.Rotation.y + 50 * Gravy::GetDeltaTime() );

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

    int ret = Gravy::Init(&confInit, &confWindow);
    if(ret == success)
    {
        Run();
    }

    return 0;
}