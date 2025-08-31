#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "Primitive.h"
#include "Shader.h"

namespace Gravy
{
    class Model
    {
    private:
        void LoadOBJ(std::string filename);
        void LoadGLTF(std::string filename);
    public:
        Model() {}
        Model(std::string filename);
        Model(Primitive _type);
        Model(const Model&) = default;
        ~Model() {}

        virtual void LoadModel(std::string filename);
        virtual void LoadPrimitive(Primitive primitive);

        virtual std::vector<Material>& GetMaterials();
        virtual void AddMaterial(unsigned int materialID, Material& material);
        virtual void RemoveMaterial(unsigned int materialID);
        virtual void SetActiveMaterial(unsigned int materialID);

        virtual void SetPosition(glm::vec3 position) { Position = position; }
        virtual void SetRotation(glm::vec3 rotation) { Rotation = rotation; }
        virtual void SetScale(glm::vec3 scale) { Scale = scale; }

        virtual void Render(Shader* shader, Camera* camera);

    public:
        glm::vec3 Position  = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation  = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale     = {1.0f, 1.0f, 1.0f};

        //std::vector<std::string> ShadersPath;
        std::vector<Mesh> Meshes;
        std::vector<Material> Materials;
    };
}