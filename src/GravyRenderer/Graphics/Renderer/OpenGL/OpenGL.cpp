#include "Graphics/Renderer/OpenGL/OpenGL.h"

//#define GLMESSAGECALLBACK_ENABLED
#define GLCHECKERROR_ENABLED

unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int FrameBuffer_Width;
unsigned int FrameBuffer_Height;

bool b_WireframeRendering = false;

void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar *message,
                     const void *userParam)
{
    #ifdef GLMESSAGECALLBACK_ENABLED
        if(id == 131185) { return; } // Ignore event : "will use video memory..."

        std::string source_str = "";
        std::string type_str = "";

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:               source_str = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     source_str = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   source_str = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       source_str = "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:       source_str = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:             source_str = "Other"; break;
        default: break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               type_str = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "Undefined"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         type_str = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         type_str = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              type_str = "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          type_str = "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           type_str = "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               type_str = "Other"; break;
        default: break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:    LOG_TRACE("OPENGL DEBUG :: Source: {} | Type: {} | Severity: Info | Message: {}", source_str, type_str, message); break;
        case GL_DEBUG_SEVERITY_LOW:             LOG_DEBUG("OPENGL DEBUG :: Source: {} | Type: {} | Severity: Low | Message: {}", source_str, type_str, message); break;
        case GL_DEBUG_SEVERITY_MEDIUM:          LOG_ERROR("OPENGL DEBUG :: Source: {} | Type: {} | Severity: Medium | Message: {}", source_str, type_str, message); break;
        case GL_DEBUG_SEVERITY_HIGH:            LOG_ERROR("OPENGL DEBUG :: Source: {} | Type: {} | Severity: High | Message: {}", source_str, type_str, message); break;
        default: break;
        }
    #endif
}

void GlCheckError(const char *function, const char *file, int line)
{
    #ifdef GLCHECKERROR_ENABLED
        GLenum error = 0;
        bool is_error = false;
        while ((error = glGetError()) != GL_NO_ERROR)
        {
            is_error = true;
            switch (error)
            {
            case GL_INVALID_ENUM:                   LOG_ERROR("  GL_INVALID_ENUM :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_INVALID_VALUE:                  LOG_ERROR("  GL_INVALID_VALUE :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_INVALID_OPERATION:              LOG_ERROR("  GL_INVALID_OPERATION :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_STACK_OVERFLOW:                 LOG_ERROR("  GL_STACK_OVERFLOW :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_STACK_UNDERFLOW:                LOG_ERROR("  GL_STACK_UNDERFLOW :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_OUT_OF_MEMORY:                  LOG_ERROR("  GL_OUT_OF_MEMORY :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  LOG_ERROR("  GL_INVALID_FRAMEBUFFER_OPERATION :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_CONTEXT_LOST:                   LOG_ERROR("  GL_CONTEXT_LOST :: encountered at function {} ({}:{})", function, file, line); break;
            case GL_TABLE_TOO_LARGE:                LOG_ERROR("  GL_TABLE_TOO_LARGE :: encountered at function {} ({}:{})", function, file, line); break;
            default:                                LOG_ERROR("  Unknown error code {}", error); break;
            }
        }
    #endif
}

namespace Gravy
{
    int OpenGL::Init(GrvConfInit* confInit)
    {
        glewExperimental = GL_TRUE;

        GLenum err = glewInit();

        if (GLEW_OK != err)
        {
            auto errChar = (const char*)glewGetErrorString(err);
            LOG_CRITICAL("ERROR! GLEW_INIT_FAILED :: {}", errChar);
            return -1;
        }
        else
        {
            LOG_DEBUG("GLEW_INIT_SUCCESS");

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            if(confInit->apiEnableMessageCallBack)
            {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(MessageCallback, 0);
            }

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            std::string gl_vendor = std::string((const char *)glGetString(GL_VENDOR));
            std::string gl_renderer = std::string((const char *)glGetString(GL_RENDERER));
            std::string gl_version = std::string((const char *)glGetString(GL_VERSION));

            LOG_INFO("OpenGL_API Information :\n    {} \n    {}\n    {}\n", gl_vendor, gl_renderer, gl_version);
        }
        return 0;
    }

    void OpenGL::SetClearColor(glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a); GLCHECK
    }

    void OpenGL::ClearBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); GLCHECK
    }

    void OpenGL::SetFrameBufferRes(glm::vec2 resolution)
    {
        SetFrameBufferRes(resolution.x, resolution.y);
    }

    void OpenGL::SetFrameBufferRes(int width, int height)
    {
        glViewport(0, 0, width, height); GLCHECK
        FrameBuffer_Height = height;
        FrameBuffer_Width = width;
    }

    glm::vec2 OpenGL::GetFrameBufferRes()
    {
        return glm::vec2(FrameBuffer_Width, FrameBuffer_Height);
    }

    float OpenGL::GetAspectRatio()
    {
        glm::vec2 res = GetFrameBufferRes();
        return res.x / res.y;
    }

    void OpenGL::WireframeRendering(bool enable)
    {
        glLineWidth(2.0f);

        if (enable)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); GLCHECK
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); GLCHECK
        }
    }

    void OpenGL::WireframeRendering(bool enable, float wireWidth)
    {
        glLineWidth(wireWidth);

        if (enable)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); GLCHECK
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); GLCHECK
        }
    }

    void OpenGL::DrawArray(GLsizei count)
    {
        glDrawArrays(GL_TRIANGLES, 0, count); GLCHECK
    }

    void OpenGL::DrawElements(GLsizei count)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); GLCHECK
    }
}