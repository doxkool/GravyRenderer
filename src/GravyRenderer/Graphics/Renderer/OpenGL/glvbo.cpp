#include "glvbo.h"

#include "Graphics/Renderer/OpenGL/OpenGL.h"

namespace Gravy
{
	GLuint glvbo::Create(uint32_t bufferSize)
	{
		glGenBuffers(1, &VBO); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK

		return VBO;
	}

	GLuint glvbo::Create(std::vector<float> *vertices)
	{
		glGenBuffers(1, &VBO); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), &vertices->front(), GL_STATIC_DRAW); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK

		return VBO;
	}

	GLuint glvbo::Create(std::vector<Vertex> *vertices)
	{
		glGenBuffers(1, &VBO); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->front(), GL_STATIC_DRAW); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, 0); GLCHECK

		return VBO;
	}

	GLuint glvbo::Create(float *vertices, uint32_t bufferSize)
	{
		glGenBuffers(1, &VBO); GLCHECK
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW); GLCHECK
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

	void glvbo::Delete(uint32_t bufferSize)
	{
		glDeleteBuffers(bufferSize, &VBO); GLCHECK
	}

	void glvbo::SendData(const void *data, uint32_t bufferSize)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO); GLCHECK
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, data); GLCHECK
	}
}