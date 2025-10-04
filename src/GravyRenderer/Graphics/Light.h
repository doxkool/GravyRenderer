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
    enum e_LightType {
        Directional,
        Point,
        Spot
    };

    class Light
    {
    private:
        glm::mat4 m_LightProjection, m_LightView;
        glm::mat4 m_LightSpaceMatrix;

    public:
        Light(e_LightType type);
        ~Light() {}

        virtual void CreateShadowMap(glm::vec2 shadowResolution = {2048, 2048});

        virtual void UpdateMatrices();
        virtual glm::mat4 GetLightSpaceMatrix() { return m_LightSpaceMatrix; }

        virtual void Render();

    public:
        Transform_t Transform;
        e_LightType LightType;

        float nearPlane = 0.1f, farPlane = 200.0f;

        unsigned int m_ShadowMap = -1;
        glm::vec2 m_ShadowRes;

        bool b_ShadowEnabled = false;

        Shader m_DepthShader;
        FrameBuffer m_DepthMapFBO;
        Texture m_DepthMapTexture;

        //Mesh m_Mesh;
        //Material m_Material;
    };
}