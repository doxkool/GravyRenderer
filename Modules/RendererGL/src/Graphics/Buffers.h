#pragma once

#include "Core/Base.h"

#include "Graphics/Renderer/OpenGL/glvao.h"
#include "Graphics/Renderer/OpenGL/glvbo.h"
#include "Graphics/Renderer/OpenGL/glebo.h"
#include "Graphics/Renderer/OpenGL/glibo.h"
#include "Graphics/Renderer/OpenGL/glfbo.h"

namespace Renderer
{
    class VertexArray : public glvao {};
    class VertexBuffer : public glvbo {};
    class ElementBuffer : public glebo {};
    class IndexBuffer : public glibo {};
    class FrameBuffer : public glfbo {};
}
