#pragma once

#include "Core/Base.h"

#include "Graphics/API/OpenGL/glshader.h"

namespace Gravy
{
    class Shader : public OpenGLShader
    {
    private:
        
    public:
        Shader() {}
        ~Shader() {}
        
        // Create a shader program
        void Create(const char* _vertexShaderFile, const char* _fragmentShaderFile, const char* _geometryShaderFile = nullptr);
        
        //// Set a integer value to the shader program
        //void Set1i(int value, std::string name);
        //// Set a float value to the shader program
	    //void Set1f(float value, std::string name);
        //// Set a 2D vector value to the shader program
	    //void SetVec2f(glm::fvec2 value, std::string name);
        //// Set a 3D vector value to the shader program
	    //void SetVec3f(glm::fvec3 value, std::string name);
        //// Set a 4D vector value to the shader program
	    //void SetVec4f(glm::fvec4 value, std::string name);
        //// Set a 3x3 matrix value to the shader program
	    //void SetMat3fv(glm::mat3 value, std::string name, bool transpose = false);
        //// Set a 4x4 matrix value to the shader program
	    //void SetMat4fv(glm::mat4 value, std::string name, bool transpose = false);
        //// Use the shader program
        //void Bind();
        //// Clear the shader program
	    //void UnBind();
        //
        //const char* vertexShaderFile;
        //const char* fragmentShaderFile;
        //const char* geometryShaderFile;
    };   
}