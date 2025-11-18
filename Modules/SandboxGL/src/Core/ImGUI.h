#pragma once

#include "RendererGL.h"

class ImGUI
{
private:
    
public:
    ImGUI() {}
    ~ImGUI() {}
    void Init(GLFWwindow* glfwWindow);
    void Shutdown();
    void NewFrame();
    void EndFrame();
    void RenderGUI();
};
