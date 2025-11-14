#include "glvbo.h"

#include "Graphics/Renderer/OpenGL.h"

namespace Renderer
{
	glvbo::glvbo()
	{
		glGenBuffers(1, &VBO); GLCHECK
	}

	GLuint glvbo::Create(uint32_t bufferSize, GLenum usage)
	{
		BufferSize = bufferSize;

		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, usage); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK

		return VBO;
	}

	GLuint glvbo::Create(std::vector<float> *vertices, GLenum usage)
	{
		BufferSize = vertices->size() * sizeof(float);

		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, BufferSize, &vertices->front(), usage); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK

		return VBO;
	}

	GLuint glvbo::Create(std::vector<Vertex> *vertices, GLenum usage)
	{
		BufferSize = vertices->size() * sizeof(Vertex);

		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, BufferSize, &vertices->front(), usage); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK

		return VBO;
	}

	GLuint glvbo::Create(float *vertices, uint32_t bufferSize, GLenum usage)
	{
		BufferSize = bufferSize;

		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, usage); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK

		return VBO;
	}

	void glvbo::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
	}

	void glvbo::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK
	}

	void glvbo::Delete()
	{
		glDeleteBuffers(BufferSize, &VBO); GLCHECK
	}

	void glvbo::SendData(const void *data, uint32_t bufferSize)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, data); GLCHECK
	}
}