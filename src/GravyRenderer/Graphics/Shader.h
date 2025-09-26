#pragma once

#include "Core/Base.h"

#include "Graphics/Renderer/OpenGL/glshader.h"

namespace Renderer
{
    class Shader : public OpenGLShader
    {
    private:
        
    public:
        Shader() {}
        ~Shader() {}
        
        void LoadBinaryFromDisk(std::string ShaderPath);
		void SaveBinaryToDisk(std::string path);
    };   
}