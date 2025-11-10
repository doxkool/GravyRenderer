#pragma once

#include "Texture.h"

namespace Renderer
{
    class Material : public OpenGLTexture
    {
    public:
        Material() = default;
        Material(const char* diffuseFile, const char* specularFile = nullptr, ColorSpace colorSpace = ColorSpace::RGB)
            : OpenGLTexture(diffuseFile, colorSpace)
        {
            Diffuse_Texture.Create(diffuseFile);
            if (specularFile)
            {
                Specular_Texture.Create(specularFile);
            }
        }
        Material(const Material&) = default;
        ~Material() = default;

        std::string Name;

        Texture Diffuse_Texture;
        Texture Specular_Texture;

        glm::vec3 Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        glm::vec3 Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 Specular = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 Emission = glm::vec3(0.0f, 0.0f, 0.0f);
        float Shininess = 0.0f;
        float Illumination = 0.0f;
        float Roughness = 0.0f;
        float Metallic = 0.0f;
        float Sheen = 0.0f;
    };
}