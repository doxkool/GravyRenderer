#include "Gravy.h"

void Run()
{
    Gravy::SetClearColor(DARK_ORANGE);
    
    while (Gravy::IsRunning())
    {
        Gravy::NewFrame();

        if(Gravy::Input::IsKeyJustPressed(KEY_GRAVE_ACCENT))
        {
            Gravy::Shutdown();
        }

        if(Gravy::Input::IsKeyJustPressed(KEY_1))
        {
            Gravy::PlayWavAudio("test.wav");
        }

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