#pragma once

#include "Core/Base.h"
#include "Core/Defines.h"
#include "Core/Logger.h"

#include <GL/glew.h>

namespace Renderer
{
	class OpenGLTexture
	{
	public:
		OpenGLTexture() {}
		OpenGLTexture(uint32_t width, uint32_t height);
		OpenGLTexture(const std::string &texPath, ColorSpace colorSpace = ColorSpace::RGB);
		OpenGLTexture(void* data, uint32_t size, ColorSpace colorSpace = ColorSpace::RGB);
		~OpenGLTexture() {}

		virtual uint32_t LoadTexture(const std::string &texPath, ColorSpace colorSpace = ColorSpace::RGB);
		virtual uint32_t SetData(void* data, uint32_t size);
		virtual void SetActiveTexture(int GlTexture);

		virtual void Bind();
		virtual void UnBind();
		virtual void BindTextureSlot(uint32_t slot);

		virtual void Delete();

		static Ref<OpenGLTexture> Create(uint32_t width, uint32_t height);
		static Ref<OpenGLTexture> Create(void* data, uint32_t size, ColorSpace colorSpace = ColorSpace::RGB);
		static Ref<OpenGLTexture> Create(const std::string &texPath, ColorSpace colorSpace = ColorSpace::RGB);

		uint32_t ID;
		bool b_hasAlpha = false;
		std::string TexturePath;

		ColorSpace TextureColorSpace;

	private:
		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;
	};
}