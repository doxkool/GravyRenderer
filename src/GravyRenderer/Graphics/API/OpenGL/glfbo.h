#pragma once

#include "Core/Logger.h"
#include "Core/Base.h"

#include <GL/glew.h>

class glfbo
{
private:
    GLuint FBO;         // Frame buffer object
    GLuint RBO;         // Render buffer object
    GLuint texture_id;
public:
    glfbo() {}
    ~glfbo() {}

    GLuint Create(float width, float height, bool shadowMap = false, ColorSpace colorSpace = ColorSpace::RGB);

    virtual void Bind();
    virtual void UnBind();
    virtual void Delete();
    virtual void Rescale(glm::vec2 resolution);
    virtual void Rescale(float width, float height);

    virtual GLuint GetTexture();
};