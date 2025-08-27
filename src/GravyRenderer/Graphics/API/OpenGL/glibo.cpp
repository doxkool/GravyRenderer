#include "glibo.h"
#include "Graphics/API/OpenGL/OpenGL.h"

GLuint glibo::Create(uint32_t* indices, uint32_t count)
{
	glCreateBuffers(1, &m_RendererID); GLCHECK

	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); GLCHECK
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW); GLCHECK

	return m_RendererID;
}

void glibo::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); GLCHECK
}

void glibo::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GLCHECK
}

void glibo::Delete()
{
	glDeleteBuffers(1, &m_RendererID); GLCHECK
}
