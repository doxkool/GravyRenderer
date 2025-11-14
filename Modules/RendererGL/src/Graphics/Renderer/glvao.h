#pragma once

#include "glibo.h"

#include <GL/glew.h>

namespace Renderer
{
    class glvao
    {
    private:
        
    public:
        glvao();
        ~glvao() {}

        void LinkAttribF(GLuint layout, GLuint numComponents, GLsizei stride, void* offset);

        void Bind();
        void UnBind();

        void Delete(uint32_t bufferSize);

    public:
        GLuint VAO;
    };
}