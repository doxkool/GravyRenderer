#pragma once

#include "Core/Time.h"

#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/Primitive.h"
#include "Graphics/Shader.h"

namespace Gravy
{
    class Model
    {
    private:
        void LoadOBJ(std::string filename);
        void LoadGLTF(std::string filename);

        glm::mat4 ModelMatrix = glm::mat4(1.0f);

    public:
        Model() : Name("Model") {}
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

        virtual void SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        {
            Transform.Position = position;
            Transform.Rotation = rotation;
            Transform.Scale = scale;
        }

        virtual void Move(glm::vec3 unit)
        {
            Transform.Position = Transform.Position + unit * Time::GetDeltaTime();
        }

        virtual void Rotate(glm::vec3 unit)
        {
            Transform.Rotation = Transform.Rotation + unit * Time::GetDeltaTime();
        }

        virtual void Scale(glm::vec3 unit)
        {
            Transform.Scale = Transform.Scale + unit * Time::GetDeltaTime();;
        }

        virtual glm::mat4 UpdateMatrix();

        virtual void Render(Shader* shader, Camera* camera);

    public:
        Transform_t Transform;

        std::string Name = "Model";
        //std::vector<std::string> ShadersPath;
        std::vector<Mesh> Meshes;
        std::vector<Material> Materials;
    };
}