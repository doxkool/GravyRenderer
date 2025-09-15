#pragma once

#include "Core/Base.h"

#include <GL/glew.h>

class glebo
{
private:
    GLuint EBO;
public:
    GLuint Create(std::vector<unsigned int> *indices);

    void Delete(uint32_t bufferSize);

    void Bind();
    void UnBind();
};