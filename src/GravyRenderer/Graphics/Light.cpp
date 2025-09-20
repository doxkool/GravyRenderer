#include "Graphics/Light.h"

namespace Renderer
{
    void Light::CreateShadowMap()
    {
        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowRes.x, shadowRes.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
    }

    void Light::Render()
    {
        if (m_shader.ID != -1) {
            m_shader.Bind();

            if (shadowEnabled) {
                
            }

            m_shader.UnBind();
        }
    }
}