#include "Gravy.h"

Gravy::Window* m_Window = nullptr;

void Run()
{
    while (!m_Window->GetWindowShouldClose())
    {
        m_Window->Update();
    }
}

int main()
{
    Gravy::Init();

    GrvConfWindow confWindow = {
        .windowName         = "Sandbox",
        .windowResX         = 1920,
        .windowResY         = 1080,
        .windowMode         = windowed,
        .windowResizable    = true,
        .transparentFB      = false,
    };


    m_Window = Gravy::CreatWindowInst(&confWindow);

    Run();
    return 0;
}