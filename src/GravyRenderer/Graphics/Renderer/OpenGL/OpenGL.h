#pragma once

#define GLEW_NO_GLU

#include <GL/glew.h>

#include "Core/Time.h"
#include "Core/Base.h"
#include "Core/Logger.h"
#include "Core/ConfigsDef.h"

#include "glshader.h"
#include "glvao.h"
#include "glvbo.h"
#include "glebo.h"
#include "glfbo.h"
#include "gltexture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CHECK_GL_ERRORS

#ifdef CHECK_GL_ERRORS
    #define GLCHECK GlCheckError(__FUNCTION__, __FILE__, __LINE__);
#else
    #define GLCHECK
#endif

void GlCheckError(const char *function, const char *file, int line);

namespace Gravy
{
    class OpenGL
    {
        private:

        public:
        OpenGL() {}
        ~OpenGL() {}

        static int Init(RendererSpec* confInit);

        static void SetClearColor(glm::vec4 color);

        static void ClearBuffer();

        static void WireframeRendering(bool enable);
        static void WireframeRendering(bool enable, float wireWidth);

        static void DrawArray(GLsizei count);
        static void DrawElements(GLsizei count);

        static void SetFrameBufferRes(glm::vec2 resolution);
        static void SetFrameBufferRes(int width, int height);
        
        static glm::vec2 GetFrameBufferRes();

        static float GetAspectRatio();
    };
}