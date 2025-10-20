#pragma once

#include "Core/Time.h"

#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/Primitive.h"
#include "Graphics/Shader.h"

namespace Renderer
{
    class Model
    {
    private:
        void LoadOBJ(std::string filename);
        void LoadGLTF(std::string filename);

        glm::mat4 ModelMatrix = glm::mat4(1.0f);

        std::vector<Material> Materials;

    public:
        Model() : Name("Model") {}
        Model(std::string filename);
        Model(Primitive _type);
        Model(const Model&) = default;
        ~Model() {}

        virtual void LoadModel(std::string filename);
        virtual void LoadPrimitive(Primitive primitive);

        virtual void SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        {
            Transform.Position = position;
            Transform.Rotation = rotation;
            Transform.Scale = scale;
            UpdateMatrix();
        }

        virtual void Move(glm::vec3 unit)
        {
            Transform.Position = Transform.Position + unit * Time::GetDeltaTime();
            UpdateMatrix();
        }

        virtual void Rotate(glm::vec3 unit)
        {
            Transform.Rotation = Transform.Rotation + unit * Time::GetDeltaTime();
            UpdateMatrix();
        }

        virtual void Scale(glm::vec3 unit)
        {
            Transform.Scale = Transform.Scale + unit * Time::GetDeltaTime();
            UpdateMatrix();
        }

        virtual glm::mat4 UpdateMatrix();
        virtual glm::mat4 GetModelMatrix() { return ModelMatrix; }

        virtual void Render();

    public:
        Transform_t Transform;
        std::string Name = "Model";
        std::vector<Mesh> Meshes;
    };
}