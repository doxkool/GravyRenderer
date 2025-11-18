#pragma once

#include "Input.h"
#include "Window/Window.h"

#include "Core/Time.h"

struct AppSpec
{
    char* name;
};

class Application
{
private:
    Window m_Window;
public:
    Application() {}
    ~Application() {}

    int Init(AppSpec* spec) { return 0; }

    Window* GetWindowInst() { return &m_Window; }
    glm::vec2 GetCurrentResolution() { return glm::vec2(0.0, 0.0); }
    
    bool IsRunning() { return true; }

    void CloseWindow() { GetWindowInst()->GetGLFW()->SetShouldWindowClose();}

    void Shutdown() {}
};
