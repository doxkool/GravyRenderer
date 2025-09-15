#include "glebo.h"
#include "Graphics/Renderer/OpenGL/OpenGL.h"

GLuint glebo::Create(std::vector<unsigned int> *indices)
{
	glGenBuffers(1, &EBO); GLCHECK
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); GLCHECK
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW); GLCHECK
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GLCHECK

	return EBO;
}

void glebo::Delete(uint32_t bufferSize)
{
	glDeleteBuffers(1, &EBO); GLCHECK
}

void glebo::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); GLCHECK
}

void glebo::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GLCHECK
}