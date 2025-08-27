#include "Buffers.h"

namespace Gravy
{/*
    uint VertexArray::Create()
    {
        return VAO.Create();
    }

    void VertexArray::LinkAttribF(unsigned int layout, unsigned int numComponents, int stride, void* offset)
    {
        VAO.LinkAttribF(layout, numComponents, stride, offset);
    }
    
    void VertexArray::Bind()
    {
        VAO.Bind();
    }
    
    void VertexArray::UnBind()
    {
        VAO.UnBind();
    }
    
    void VertexArray::Delete(uint32_t bufferSize)
    {
        VAO.Delete(bufferSize);
    }

// ----- Vertex Buffer -----

    uint VertexBuffer::Create(uint32_t size)
    {
        return VBO.Create(size);
    }

    uint VertexBuffer::Create(std::vector<float> *vertices)
	{
        return VBO.Create(vertices);
    }

    uint VertexBuffer::Create(float *vertices, uint32_t size)
	{
        return VBO.Create(vertices, size);
    }

    uint VertexBuffer::Create(std::vector<Vertex> *vertices)
    {
        return VBO.Create(vertices);
    }

    void VertexBuffer::Bind()
    {
        VBO.Bind();
    }
    
    void VertexBuffer::UnBind()
    {
        VBO.UnBind();
    }
    
    void VertexBuffer::Delete(uint32_t bufferSize)
    {
        VBO.Delete(bufferSize);
    }

// ----- Ellement Buffer -----

uint ElementBuffer::Create(std::vector<unsigned int> *indices)
    {
        return EBO.Create(indices);
    }

    void ElementBuffer::Bind()
    {
        EBO.Bind();
    }
    
    void ElementBuffer::UnBind()
    {
        EBO.UnBind();
    }
    
    void ElementBuffer::Delete(uint32_t bufferSize)
    {
        EBO.Delete(bufferSize);
    }

// ----- Index Buffer -----

    uint IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        return IBO.Create(indices, count);
    }

    void IndexBuffer::Bind()
    {
        IBO.Bind();
    }
    
    void IndexBuffer::UnBind()
    {
        IBO.UnBind();
    }
    
    void IndexBuffer::Delete()
    {
        IBO.Delete();
    }

    uint32_t IndexBuffer::GetCount()
    {
        return IBO.GetCount();
    }
*/
// ----- Frame Buffer -----

    FrameBuffer::FrameBuffer()
    {
        FBO = new glfbo;
    }

    FrameBuffer::~FrameBuffer()
    {
        delete FBO;
    }

    unsigned int FrameBuffer::Create(float width, float height, bool shadowMap)
    {
        return FBO->Create(width, height, shadowMap);
    }


    void FrameBuffer::Bind()
    {
        FBO->Bind();
    }
    
    void FrameBuffer::UnBind()
    {
        FBO->UnBind();
    }
    
    void FrameBuffer::Delete()
    {
        FBO->Delete();
    }

    void FrameBuffer::Rescale(float width, float height)
    {
        FBO->Rescale(width, height);
    }

    unsigned int FrameBuffer::GetTexture()
    {
    	return FBO->GetTexture();
    }
}
