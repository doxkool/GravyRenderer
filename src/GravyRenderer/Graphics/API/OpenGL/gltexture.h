#pragma once

#include "Core/Base.h"
#include "Core/Defines.h"
#include "Core/Logger.h"

#include <GL/glew.h>

namespace Gravy
{
	class OpenGLTexture
	{
	public:
		OpenGLTexture();
		OpenGLTexture(const std::string &texPath, ColorSpace colorSpace = ColorSpace::RGB);
		OpenGLTexture(void* data, uint32_t size, ColorSpace colorSpace = ColorSpace::RGB);
		~OpenGLTexture();

		virtual uint32_t LoadTexture(const std::string &texPath, ColorSpace colorSpace = ColorSpace::RGB);
		virtual uint32_t SetData(void* data, uint32_t size);
		virtual uint32_t LoadSkyBox(std::vector<std::string> faces);
		virtual void SetActiveTexture(int GlTexture);

		virtual void Bind();
		virtual void UnBind();
		virtual void BindTextureSlot(uint32_t slot);

		virtual void Delete();

		static Ref<OpenGLTexture> Create(const std::string &texPath, ColorSpace colorSpace = ColorSpace::RGB);

		uint32_t ID;
		bool b_hasAlpha = false;
		bool b_IsSkyBox = false;
		std::string TexturePath;

		ColorSpace TextureColorSpace;

	private:
	};
}