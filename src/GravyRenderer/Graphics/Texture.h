#pragma once

#include "Core/Base.h"

#include "Graphics/Renderer/OpenGL/gltexture.h"

namespace Gravy
{
    class Texture : public OpenGLTexture
    {
    private:
        
    public:
        Texture() {}
        ~Texture() {}
        
        // Create a texture
        //virtual void Create(const std::string& Texture_Path, ColorSpace colorSpace = ColorSpace::RGB);
        //virtual void SetActiveTexture(int GlTexture);
        //virtual void BindToSlot(uint32_t slot);
        //virtual void Bind();
        //virtual void UnBind();
    };    
}