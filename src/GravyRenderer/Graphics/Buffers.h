#pragma once

#include "Core/Base.h"

#include "Graphics/Renderer/OpenGL/glvao.h"
#include "Graphics/Renderer/OpenGL/glvbo.h"
#include "Graphics/Renderer/OpenGL/glebo.h"
#include "Graphics/Renderer/OpenGL/glibo.h"
#include "Graphics/Renderer/OpenGL/glfbo.h"

namespace Renderer
{
    class VertexArray : public glvao
    {
    private:
        
    public:
        //// Create a Vertex Array Object
        //uint Create();
        //// Link the attributes of the VBO to the VAO
        //void LinkAttribF(unsigned int layout, unsigned int numComponents, int stride, void* offset);
        //// Bind the Vertex Array Object
        //void Bind();
        //// Unbind the Vertex Array Object
        //void UnBind();
        //// Delete the Vertex Array Object
        //void Delete(uint32_t bufferSize);
    };

    class VertexBuffer : public glvbo
    {
    private:
        
    public:
        //// Create a Vertex Buffer Object
        //uint Create(uint32_t size);
        //// Create a Vertex Buffer Object
	    //uint Create(std::vector<float> *vertices);
        //// Create a Vertex Buffer Object
	    //uint Create(float *vertices, uint32_t size);
        //// Create a Vertex Buffer Object
	    //uint Create(std::vector<Vertex> *vertices);
        //// Bind the Vertex Buffer Object
        //void Bind();
        //// Unbind the Vertex Buffer Object
        //void UnBind();
        //// Delete the Vertex Buffer Object
        //void Delete(uint32_t bufferSize);
        //// Send data to the Vertex Buffer Object
        //void SetData(const void *data, uint32_t size);
    };

    class ElementBuffer : public glebo
    {
    private:
        
    public:
        //// Create an Element Buffer Object
        //uint Create(std::vector<unsigned int> *indices);
        //// Bind the Element Buffer Object
        //void Bind();
        //// Unbind the Element Buffer Object
        //void UnBind();
        //// Delete the Element Buffer Object
        //void Delete(uint32_t bufferSize);
    };

    class IndexBuffer : public glibo
    {
    private:
        
    public:
        //// Create an Index Buffer Object
        //uint Create(uint32_t* indices, uint32_t count);
        //// Bind the Index Buffer Object
        //void Bind();
        //// Unbind the Index Buffer Object
        //void UnBind();
        //// Delete the Index Buffer Object
        //void Delete();
        //// Get the count of the Index Buffer Object
        //uint32_t GetCount();
    };

    class FrameBuffer : public glfbo
    {
    private:
        glfbo* FBO;
    public:

        FrameBuffer();
        ~FrameBuffer();

        // Create a Frame Buffer Object
        virtual unsigned int Create(float width, float height, bool shadowMap = false);
        // Bind the Frame Buffer Object
        virtual void Bind();
        // Unbind the Frame Buffer Object
        virtual void UnBind();
        // Delete the Frame Buffer Object
        virtual void Delete();
        // Rescale the Frame Buffer Object
        virtual void Rescale(float width, float height);
        // Get the texture of the Frame Buffer Object
        virtual unsigned int GetTexture();
    };
}
