#pragma once

#include "Core/Base.h"

#include "Graphics/Renderer/OpenGL/OpenGL.h"

#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/Primitive.h"
#include "Graphics/Shader.h"

namespace Gravy
{
    class Light
    {
    private:
        unsigned int shadowMap = -1;
        glm::vec2 shadowRes = {1024, 1024};
        bool shadowEnabled = false;

    public:
        Light() {}
        ~Light() {}

        virtual void CreateShadowMap();

        virtual void Render();

    public:
        Transform_t Transform;

        Mesh m_Mesh;
        Material m_Material;
        Shader m_shader;
    };
}