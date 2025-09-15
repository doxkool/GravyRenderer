#pragma once

#include "Core/Base.h"
#include "Core/Logger.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Gravy
{
	class OpenGLShader
	{
	private:
	    std::string ReadShaderFile(const char* ShaderFile);
	public:
		//static Ref<OpenGLShader> Create(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);

	    OpenGLShader(const char* _vertexShaderFile, const char* _fragmentShaderFile, const char* _geometryShaderFile = nullptr);
		OpenGLShader() {}
	    ~OpenGLShader();

		void loadShader(const char* _vertexShaderFile, const char* _fragmentShaderFile, const char* _geometryShaderFile = nullptr);

	    void Set1i(GLint value, std::string name);
		void Set1f(GLfloat value, std::string name);
		void SetVec2f(glm::fvec2 value, std::string name);
		void SetVec3f(glm::fvec3 value, std::string name);
		void SetVec4f(glm::fvec4 value, std::string name);
		void SetMat3fv(glm::mat3 value, std::string name, GLboolean transpose = GL_FALSE);
		void SetMat4fv(glm::mat4 value, std::string name, GLboolean transpose = GL_FALSE);

	    void Bind();
		void UnBind();
		void Delete();

		int GetID() const;

	    uint32_t ID = -1;

		const char* vertexShaderFile;
		const char* fragmentShaderFile;
		const char* geometryShaderFile;
	};
}