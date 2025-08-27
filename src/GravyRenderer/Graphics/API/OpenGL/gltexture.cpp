#include "gltexture.h"

#include "Graphics/API/OpenGL/OpenGL.h"

#include <stb_image.h>

Ref<OpenGLTexture> OpenGLTexture::Create(const std::string &texPath, ColorSpace colorSpace)
{
	return CreateRef<OpenGLTexture>(texPath, colorSpace);
}

OpenGLTexture::OpenGLTexture()
{
}

OpenGLTexture::OpenGLTexture(void* data, uint32_t size, ColorSpace colorSpace)
{
	TextureColorSpace = colorSpace;

	glGenTextures(1, &ID); GLCHECK
	glBindTexture(GL_TEXTURE_2D, ID); GLCHECK

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLCHECK
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLCHECK

	glTextureSubImage2D(ID, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, data); GLCHECK
}

OpenGLTexture::OpenGLTexture(const std::string &texPath, ColorSpace colorSpace)
{
	LoadTexture(texPath, TextureColorSpace);
}

OpenGLTexture::~OpenGLTexture()
{
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

			stbi_image_free(data);
			unsigned char *data = stbi_load(DEFAULT_TEX, &width, &height, &nrChannels, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); GLCHECK
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);
	}

	return ID;
}

uint32_t OpenGLTexture::SetData(void* data, uint32_t size)
{
	glGenTextures(1, &ID); GLCHECK
	glBindTexture(GL_TEXTURE_2D, ID); GLCHECK

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLCHECK
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLCHECK
	
	glTextureSubImage2D(ID, 0, 0, 0, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, data); GLCHECK

	return ID;
}

uint32_t OpenGLTexture::LoadSkyBox(std::vector<std::string> faces)
{
    glGenTextures(1, &ID); GLCHECK
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID); GLCHECK

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            ); GLCHECK
            stbi_image_free(data);
			LOG_TRACE("Texture loaded : {}", faces[i]);
        }
        else
        {
            LOG_ERROR("Cubemap tex failed to load at path: {}", faces[i]);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLCHECK
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLCHECK
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); GLCHECK
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); GLCHECK
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); GLCHECK

	b_IsSkyBox = true;

	return ID;
}

void OpenGLTexture::SetActiveTexture(int GlTexture)
{
	glActiveTexture(GlTexture); GLCHECK
}

void OpenGLTexture::Bind()
{	
	if(b_IsSkyBox)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID); GLCHECK
	}else{
		glBindTexture(GL_TEXTURE_2D, ID); GLCHECK
	}
}

void OpenGLTexture::UnBind()
{	
	if(b_IsSkyBox)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0); GLCHECK
	}else{
		glBindTexture(GL_TEXTURE_2D, 0); GLCHECK
	}
}

void OpenGLTexture::BindTextureSlot(uint32_t slot)
{
	glBindTextureUnit(slot, ID); GLCHECK
}

void OpenGLTexture::Delete()
{
	glDeleteTextures(1, &ID); GLCHECK
}