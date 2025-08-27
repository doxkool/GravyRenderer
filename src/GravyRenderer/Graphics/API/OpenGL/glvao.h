#pragma once

#include "Core/Base.h"

#include "glibo.h"

#include <GL/glew.h>

class glvao
{
private:
    GLuint VAO;
public:
    GLuint Create();

    void LinkAttribF(GLuint layout, GLuint numComponents, GLsizei stride, void* offset);

    void Bind();
    void UnBind();

    void Delete(uint32_t bufferSize);
};