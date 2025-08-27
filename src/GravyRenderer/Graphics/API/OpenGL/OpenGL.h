#pragma once

#define GLEW_NO_GLU

#include <GL/glew.h>

#include "Core/Time.h"
#include "Core/Base.h"
#include "Core/Logger.h"

#include "glshader.h"
#include "glvao.h"
#include "glvbo.h"
#include "glebo.h"
#include "glfbo.h"
#include "gltexture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <imgui_internal.h>

#define CHECK_GL_ERRORS

#ifdef CHECK_GL_ERRORS
    #define GLCHECK GlCheckError(__FUNCTION__, __FILE__, __LINE__);
#else
    #define GLCHECK
#endif

void GlCheckError(const char *function, const char *file, int line);

namespace OpenGL
{
    void Init_OpenGL();

    void SetClearColor(glm::vec4 color);

    void ClearBuffer();

    void WireframeRendering(bool enable);
    void WireframeRendering(bool enable, float wireWidth);

    void DrawArray(GLsizei count);
    void DrawElements(GLsizei count);

    void SetFrameBufferRes(glm::vec2 resolution);
    void SetFrameBufferRes(int width, int height);
    
    glm::vec2 GetFrameBufferRes();

    float GetAspectRatio();
}