#include "Graphics/Light.h"

namespace Renderer
{
    Light::Light(e_LightType type)
        : LightType(type)
    {
    }

    void Light::CreateShadowMap(glm::vec2 shadowResolution)
    {
        m_ShadowRes = shadowResolution;
        m_DepthMapFBO.Create(shadowResolution.x, shadowResolution.y, true);

        b_ShadowEnabled = true;
    }

    void Light::UpdateMatrices()
    {
        if (b_ShadowEnabled) {
            if (LightType == Point || LightType == Spot) {
                m_LightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)m_ShadowRes.x / (GLfloat)m_ShadowRes.y, nearPlane, farPlane);
            }

            if (LightType == Directional) {
                m_LightProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, nearPlane, farPlane);
            }

            m_LightView = glm::lookAt(Transform.Position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
            m_LightSpaceMatrix = m_LightProjection * m_LightView;
        }
    }

    void Light::Render()
    {

    }
}