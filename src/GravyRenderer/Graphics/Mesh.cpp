#include "Mesh.h"

namespace Gravy
{
    void Mesh::Create(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices)
    {
        VAO.Create();
        VBO.Create(&_vertices);
        EBO.Create(&_indices);

        vertices = _vertices;
        indices = _indices;

        VAO.Bind();
        VBO.Bind();

        VAO.LinkAttribF(0, 3, sizeof(Vertex), (void *)offsetof(Vertex, posX));
        VAO.LinkAttribF(1, 2, sizeof(Vertex), (void *)offsetof(Vertex, texCoordX));
        VAO.LinkAttribF(2, 3, sizeof(Vertex), (void *)offsetof(Vertex, normalX));

        VAO.UnBind();
        VBO.UnBind();
        EBO.UnBind();
    }

    void Mesh::PushVertex(Vertex vertex)
    {
        vertices.push_back(vertex);
    }

    void Mesh::PushIndex(uint32_t index)
    {
        indices.push_back(index);
    }

    void Mesh::Draw()
    {        
        VAO.Bind();
        VBO.Bind();
        
        // ---- RENDERING ----
        OpenGL::DrawArray(indices.size());
        
        VAO.UnBind();
        VBO.UnBind();
    }
}