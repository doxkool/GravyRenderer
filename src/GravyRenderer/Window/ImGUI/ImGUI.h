#pragma once

#include "Core/Logger.h"
#include "Core/ConfigsDef.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace Gravy
{
    class ImGUI
    {
    private:
        
    public:
        ImGUI();
        ~ImGUI();

        void Init();
    };
}