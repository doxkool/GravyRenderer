#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

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