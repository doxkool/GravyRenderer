#include "glfbo.h"
#include "Graphics/API/OpenGL/OpenGL.h"

GLuint glfbo::Create(float width, float height, bool shadowMap, ColorSpace colorSpace)
{
	if(shadowMap)
	{
		glGenFramebuffers(1, &FBO); GLCHECK
		glBindFramebuffer(GL_FRAMEBUFFER, FBO); GLCHECK
		
		glGenTextures(1, &texture_id); GLCHECK
		glBindTexture(GL_TEXTURE_2D, texture_id); GLCHECK
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); GLCHECK
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0); GLCHECK
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GLCHECK
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GLCHECK
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); GLCHECK
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); GLCHECK
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); GLCHECK
    	// attach depth texture as FBO's depth buffer
    	glBindFramebuffer(GL_FRAMEBUFFER, FBO); GLCHECK
    	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0); GLCHECK
    	glDrawBuffer(GL_NONE); GLCHECK
    	glReadBuffer(GL_NONE); GLCHECK
    	glBindFramebuffer(GL_FRAMEBUFFER, 0); GLCHECK
	}else{
		glGenFramebuffers(1, &FBO); GLCHECK
		glBindFramebuffer(GL_FRAMEBUFFER, FBO); GLCHECK

		glGenTextures(1, &texture_id); GLCHECK
		glBindTexture(GL_TEXTURE_2D, texture_id); GLCHECK

		if (colorSpace == ColorSpace::SRGB){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); GLCHECK
		}else{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); GLCHECK
		}
			
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0); GLCHECK
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLCHECK
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLCHECK
		glGenRenderbuffers(1, &RBO); GLCHECK
		glBindRenderbuffer(GL_RENDERBUFFER, RBO); GLCHECK
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); GLCHECK
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); GLCHECK

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!"); GLCHECK

		glBindFramebuffer(GL_FRAMEBUFFER, 0); GLCHECK
		glBindTexture(GL_TEXTURE_2D, 0); GLCHECK
		glBindRenderbuffer(GL_RENDERBUFFER, 0); GLCHECK
	}
	

    return FBO;
}


// here we bind our framebuffer
void glfbo::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO); GLCHECK
}

// here we unbind our framebuffer
void glfbo::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); GLCHECK
}

void glfbo::Delete()
{
	glDeleteBuffers(1, &FBO); GLCHECK
}

void glfbo::Rescale(glm::vec2 resolution)
{
	Rescale(resolution.x, resolution.y);
}

void glfbo::Rescale(float width, float height)
{
	glm::vec2 currentFbRes = OpenGL::GetFrameBufferRes();
	glm::vec2 newFbRes = { width, height };

	if (currentFbRes != newFbRes)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO); GLCHECK
		glBindTexture(GL_TEXTURE_2D, texture_id); GLCHECK
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); GLCHECK
		glBindFramebuffer(GL_FRAMEBUFFER, 0); GLCHECK

		OpenGL::SetFrameBufferRes(newFbRes);
		return;
	}
}

GLuint glfbo::GetTexture()
{
	return texture_id;
}