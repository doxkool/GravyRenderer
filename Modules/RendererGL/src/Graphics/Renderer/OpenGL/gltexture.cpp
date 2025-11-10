#include "gltexture.h"

#include "Graphics/Renderer/OpenGL/OpenGL.h"

#include <stb_image.h>

namespace Renderer
{
	Ref<OpenGLTexture> OpenGLTexture::Create(uint32_t width, uint32_t height)
	{
		return CreateRef<OpenGLTexture>(width, height);
	}

	Ref<OpenGLTexture> OpenGLTexture::Create(void* data, uint32_t size, ColorSpace colorSpace)
	{
		return CreateRef<OpenGLTexture>(data, size, colorSpace);
	}

	Ref<OpenGLTexture> OpenGLTexture::Create(const std::string &texPath, ColorSpace colorSpace)
	{
		return CreateRef<OpenGLTexture>(texPath, colorSpace);
	}

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &ID);
		glTextureStorage2D(ID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture::OpenGLTexture(void* data, uint32_t size, ColorSpace colorSpace)
	{
		TextureColorSpace = colorSpace;
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		m_Width = 1;
		m_Height = 1;

		glCreateTextures(GL_TEXTURE_2D, 1, &ID); GLCHECK
		glTextureStorage2D(ID, 1, m_InternalFormat, m_Width, m_Height); GLCHECK

		glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLCHECK
		glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLCHECK

		glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT); GLCHECK
		glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT); GLCHECK

		glTextureSubImage2D(ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data); GLCHECK
	}

	OpenGLTexture::OpenGLTexture(const std::string &texPath, ColorSpace colorSpace)
	{
		LoadTexture(texPath, TextureColorSpace);
	}

	uint32_t OpenGLTexture::LoadTexture(const std::string &texPath, ColorSpace colorSpace)
	{
		TextureColorSpace = colorSpace;

		TexturePath = texPath.c_str();

		glGenTextures(1, &ID); GLCHECK
		glBindTexture(GL_TEXTURE_2D, ID); GLCHECK

		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLCHECK
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLCHECK

		stbi_set_flip_vertically_on_load(true);

		int width, height, nrChannels;

		if (texPath.c_str())
		{
			unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				if (nrChannels == 1)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GLCHECK
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GLCHECK

					if(TextureColorSpace == ColorSpace::RGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); GLCHECK
					}else if (TextureColorSpace == ColorSpace::SRGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data); GLCHECK
					}
					b_hasAlpha = false;
				}
				if (nrChannels == 2)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GLCHECK
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GLCHECK

					if(TextureColorSpace == ColorSpace::RGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data); GLCHECK
					}else if (TextureColorSpace == ColorSpace::SRGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data); GLCHECK
					}
					b_hasAlpha = false;
				}
				if (nrChannels == 3)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GLCHECK
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GLCHECK

					if(TextureColorSpace == ColorSpace::RGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); GLCHECK
					}else if (TextureColorSpace == ColorSpace::SRGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); GLCHECK
					}
					b_hasAlpha = false;
				}
				if (nrChannels == 4)
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GLCHECK
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GLCHECK
					if(TextureColorSpace == ColorSpace::RGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); GLCHECK
					}else if (TextureColorSpace == ColorSpace::SRGB)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); GLCHECK
					}
					b_hasAlpha = true;
				}

				glGenerateMipmap(GL_TEXTURE_2D); GLCHECK
				LOG_TRACE("Texture loaded : {}", texPath);
			}
			else
			{
				LOG_ERROR("Failed to load texture : {}", texPath);

				return -1;
			}
			stbi_image_free(data);
		}

		return ID;
	}

	uint32_t OpenGLTexture::SetData(void* data, uint32_t size)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		m_Width = 1;
		m_Height = 1;

		glCreateTextures(GL_TEXTURE_2D, 1, &ID); GLCHECK
		glTextureStorage2D(ID, 1, m_InternalFormat, m_Width, m_Height); GLCHECK

		glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLCHECK
		glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLCHECK

		glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT); GLCHECK
		glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT); GLCHECK

		glTextureSubImage2D(ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data); GLCHECK

		return ID;
	}

	void OpenGLTexture::SetActiveTexture(int GlTexture)
	{
		glActiveTexture(GlTexture); GLCHECK
	}

	void OpenGLTexture::Bind()
	{	
		glBindTexture(GL_TEXTURE_2D, ID); GLCHECK
	}

	void OpenGLTexture::UnBind()
	{	
		glBindTexture(GL_TEXTURE_2D, 0); GLCHECK
	}

	void OpenGLTexture::BindTextureSlot(uint32_t slot)
	{
		glBindTextureUnit(slot, ID); GLCHECK
	}

	void OpenGLTexture::Delete()
	{
		glDeleteTextures(1, &ID); GLCHECK
	}
}