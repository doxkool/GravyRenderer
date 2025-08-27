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

        virtual void Render();

    public:
        //std::vector<std::string> ShadersPath;
        std::vector<Mesh> Meshes;
        std::vector<Material> Materials;
    };
}