#include "glvao.h"
#include "Graphics/API/OpenGL/OpenGL.h"

namespace Gravy
{
	GLuint glvao::Create()
	{
		//glCreateVertexArrays(1, &VAO); GLCHECK
		glGenVertexArrays(1, &VAO); GLCHECK
		return VAO;
	}

	void glvao::LinkAttribF(GLuint layout, GLuint numComponents, GLsizei stride, void* offset)
	{
		glVertexAttribPointer(layout, numComponents, GL_FLOAT, GL_FALSE, stride, offset); GLCHECK
		glEnableVertexAttribArray(layout); GLCHECK
	}

	void glvao::Bind()
	{
		glBindVertexArray(VAO); GLCHECK
	}

	void glvao::UnBind()
	{
		glBindVertexArray(0); GLCHECK
	}

	void glvao::Delete(uint32_t bufferSize)
	{
		glDeleteVertexArrays(bufferSize, &VAO); GLCHECK
	}
}