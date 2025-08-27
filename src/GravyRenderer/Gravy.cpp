#include "Gravy.h"

namespace Gravy
{
    Window m_Window;

    void Init()
    {
        Logger::Init();
    }

    Window* CreatWindowInst(GrvConfWindow* confWindow)
    {
        m_Window.InitWindow(confWindow);
        return &m_Window;
    }
}