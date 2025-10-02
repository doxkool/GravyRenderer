#pragma once

#include "Core/Base.h"

#include "Graphics/Renderer/OpenGL/OpenGL.h"

#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/Primitive.h"
#include "Graphics/Shader.h"

namespace Renderer
{
    class Light
    {
    private:
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;

    public:
        Light() {}
        ~Light() {}

        virtual void CreateShadowMap();

        virtual void UpdateMatrices();
        virtual glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; }

        virtual void Render();

    public:
        Transform_t Transform;

        float nearPlane = 1.0f, farPlane = 10.0f;

        unsigned int shadowMap = -1;
        glm::vec2 shadowRes = {1024, 1024};
        bool shadowEnabled = false;

        //Mesh m_Mesh;
        //Material m_Material;
        //Shader m_shader;
    };
}