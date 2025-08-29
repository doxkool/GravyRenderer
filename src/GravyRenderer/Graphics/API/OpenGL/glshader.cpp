#include "glshader.h"
#include "Graphics/API/OpenGL/OpenGL.h"

#include <glm/gtc/type_ptr.hpp>

#include "Core/Tracing.h"

namespace Gravy
{
	//Ref<OpenGLShader> OpenGLShader::Create(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile)
	//{
	//		return CreateRef<OpenGLShader>(vertexShaderFile, fragmentShaderFile,geometryShaderFile);
	//}

	OpenGLShader::OpenGLShader(const char* _vertexShaderFile, const char* _fragmentShaderFile, const char* _geometryShaderFile)
	{
		loadShader(_vertexShaderFile, _fragmentShaderFile, _geometryShaderFile);
	}

	void OpenGLShader::loadShader(const char* _vertexShaderFile, const char* _fragmentShaderFile, const char* _geometryShaderFile)
	{
		ZoneScoped;

		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int geometryShader;

		vertexShaderFile = _vertexShaderFile;
		fragmentShaderFile = _fragmentShaderFile;
		geometryShaderFile = _geometryShaderFile;

		std::string str_src;
		const GLchar *src;
		int success;
		char infoLog[512];

		if (vertexShaderFile != NULL && fragmentShaderFile != NULL)
		{
			// build and compile our shader program
			// ------------------------------------
			// vertex shader
			vertexShader = glCreateShader(GL_VERTEX_SHADER); GLCHECK
			str_src = ReadShaderFile(vertexShaderFile);
			src = str_src.c_str();
			glShaderSource(vertexShader, 1, &src, NULL); GLCHECK
			glCompileShader(vertexShader); GLCHECK
			// check for shader compile errors
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); GLCHECK
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); GLCHECK
				LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n {}", infoLog);
			}
			else
			{
				LOG_TRACE("Shader compilation succeded: {}", vertexShaderFile);
			}
		}

		if (fragmentShaderFile != NULL)
		{
			// fragment shader
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); GLCHECK
			str_src = ReadShaderFile(fragmentShaderFile);
			src = str_src.c_str();
			glShaderSource(fragmentShader, 1, &src, NULL); GLCHECK
			glCompileShader(fragmentShader); GLCHECK
			// check for shader compile errors
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); GLCHECK
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); GLCHECK
				LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}", infoLog);
			}
			else
			{
				LOG_TRACE("Shader compilation succeded: {}", fragmentShaderFile);
			}
		}

		if (geometryShaderFile != NULL)
		{
			// geometry shader
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER); GLCHECK
			str_src = ReadShaderFile(geometryShaderFile);
			src = str_src.c_str();
			glShaderSource(geometryShader, 1, &src, NULL); GLCHECK
			glCompileShader(geometryShader); GLCHECK
			// check for shader compile errors
			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success); GLCHECK
			if (!success)
			{
				glGetShaderInfoLog(geometryShader, 512, NULL, infoLog); GLCHECK
				LOG_ERROR("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n {}", infoLog);
			}
			else
			{
				LOG_TRACE("Shader compilation succeded: {}", geometryShaderFile);
			}
		}

		// link shaders
		ID = glCreateProgram(); GLCHECK
		glAttachShader(ID, vertexShader); GLCHECK
		glAttachShader(ID, fragmentShader); GLCHECK
		if (geometryShaderFile != NULL)
		{
			glAttachShader(ID, geometryShader); GLCHECK
		}
		glLinkProgram(ID); GLCHECK
		// check for linking errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success); GLCHECK
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog); GLCHECK
			LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
		}
		glDeleteShader(vertexShader); GLCHECK
		glDeleteShader(fragmentShader); GLCHECK
		if (geometryShaderFile != NULL)
		{
			glDeleteShader(geometryShader); GLCHECK
		}	
	}

	OpenGLShader::~OpenGLShader()
	{
		//deleteShaderProgram();
	}

	std::string OpenGLShader::ReadShaderFile(const char* ShaderFile)
	{
		// 1. retrieve the vertex/fragment source code from filePath
	    std::string shaderCode;
	    std::ifstream vShaderFile;
	    // ensure ifstream objects can throw exceptions:
	    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	    try 
	    {
	        // open files
	        vShaderFile.open(ShaderFile);
	        std::stringstream vShaderStream;
	        // read file's buffer contents into streams
	        vShaderStream << vShaderFile.rdbuf();
	        // close file handlers
	        vShaderFile.close();
	        // convert stream into string
	        shaderCode   = vShaderStream.str();
	    }
	    catch (std::ifstream::failure& e)
	    {
			LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", e.what());
	    }
	
		return shaderCode.c_str();
	}

	void OpenGLShader::Bind() {
	    glUseProgram(ID); GLCHECK
	}

	void OpenGLShader::UnBind() {
	    glUseProgram(0); GLCHECK
	}

	// Deletes the Shader Program
	void OpenGLShader::Delete()
	{
		LOG_ERROR("Deleting shader program ID #{}", this->ID);
		glDeleteProgram(ID); GLCHECK
	}

	void OpenGLShader::Set1i(GLint value, std::string name)
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value); GLCHECK
	}

	void OpenGLShader::Set1f(GLfloat value, std::string name)
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value); GLCHECK
	}

	void OpenGLShader::SetVec2f(glm::fvec2 value, std::string name)
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); GLCHECK
	}

	void OpenGLShader::SetVec3f(glm::fvec3 value, std::string name)
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); GLCHECK
	}

	void OpenGLShader::SetVec4f(glm::fvec4 value, std::string name)
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); GLCHECK
	}

	void OpenGLShader::SetMat3fv(glm::mat3 value, std::string name, GLboolean transpose)
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, glm::value_ptr(value)); GLCHECK
	}

	void OpenGLShader::SetMat4fv(glm::mat4 value, std::string name, GLboolean transpose)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, glm::value_ptr(value)); GLCHECK
	}

	int OpenGLShader::GetID() const
	{
		return ID;
	}
}