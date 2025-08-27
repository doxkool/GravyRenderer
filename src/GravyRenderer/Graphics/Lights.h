#pragma once

#include "Core/Base.h"

#include "Graphics/API/OpenGL/OpenGL.h"

namespace Gravy
{
    class Lights
    {
    private:
        unsigned int m_DepthCubemap;

        unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    public:
        Lights();
        ~Lights();

        void CreateShadowMap();
    };
}