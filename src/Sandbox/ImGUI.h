#pragma once

#include "Gravy.h"

class ImGUI
{
private:
    
public:
    ImGUI() {}
    ~ImGUI() {}
    void Init();
    void Shutdown();
    void NewFrame();
    void EndFrame();
};
