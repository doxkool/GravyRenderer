#include "Shader.h"

namespace Gravy
{
    void Shader::Create(const char* _vertexShaderFile, const char* _fragmentShaderFile, const char* _geometryShaderFile)
    {
        loadShader(_vertexShaderFile, _fragmentShaderFile, _geometryShaderFile);
    
        //vertexShaderFile = _vertexShaderFile;
        //fragmentShaderFile = _fragmentShaderFile;
        //geometryShaderFile = _geometryShaderFile;
    }

    //void Shader::Set1i(int value, std::string name)
    //{
    //    shader.set1i(value, name.c_str());
    //}
    //
	//void Shader::Set1f(float value, std::string name)
    //{
    //    shader.set1f(value, name.c_str());
    //}
    //
	//void Shader::SetVec2f(glm::fvec2 value, std::string name)
    //{
    //    shader.setVec2f(value, name.c_str());
    //}
    //
	//void Shader::SetVec3f(glm::fvec3 value, std::string name)
    //{
    //    shader.setVec3f(value, name.c_str());
    //}
    //
	//void Shader::SetVec4f(glm::fvec4 value, std::string name)
    //{
    //    shader.setVec4f(value, name.c_str());
    //}
    //
	//void Shader::SetMat3fv(glm::mat3 value, std::string name, bool transpose)
    //{
    //    shader.setMat3fv(value, name.c_str(), transpose);
    //}
    //
	//void Shader::SetMat4fv(glm::mat4 value, std::string name, bool transpose)
    //{
    //    shader.setMat4fv(value, name.c_str(), transpose);
    //}
    //
    //void Shader::Bind()
    //{
    //    shader.useShaderProgram();
    //}
    //
	//void Shader::UnBind()
    //{
    //    shader.clearShaderProgram();
    //}
}