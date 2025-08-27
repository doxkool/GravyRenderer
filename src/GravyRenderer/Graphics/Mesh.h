#pragma once

#include "Camera.h"

#include "Core/Defines.h"

#include "Buffers.h"
#include "Shader.h"
#include "Texture.h"
#include "Graphics/API/OpenGL/OpenGL.h"

namespace Gravy
{
    class Mesh
    {
    private:
    public:
        Mesh() {}
        ~Mesh() {}

        void Create(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices);
        void PushVertex(Vertex vertex);
        void PushIndex(uint32_t index);
        void Draw();

    public:
        const char* Name;
        int MaterialID;

        Gravy::VertexArray VAO;
        Gravy::VertexBuffer VBO;
        Gravy::ElementBuffer EBO;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
}