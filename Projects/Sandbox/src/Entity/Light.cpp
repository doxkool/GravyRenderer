#include "Entity/Light.h"

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
            if (LightType == PointLight || LightType == SpotLight) {
                //m_LightProjection = glm::perspective(glm::radians(45.0f), m_ShadowRes.x / m_ShadowRes.y, nearPlane, farPlane);
                m_LightProjection = glm::perspective<float>(glm::radians(45.0f), 1, nearPlane, farPlane);
            }

            if (LightType == DirectionalLight) {
                m_LightProjection = glm::ortho(-250.0f, 250.0f, -250.0f, 250.0f, nearPlane, farPlane);
            }

            m_LightView = glm::lookAt(Transform.Position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
            m_LightSpaceMatrix = m_LightProjection * m_LightView;
        }
    }

    void Light::Render()
    {

    }
}