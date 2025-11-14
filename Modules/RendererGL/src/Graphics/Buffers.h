#pragma once

#include "Graphics/Renderer/glvao.h"
#include "Graphics/Renderer/glvbo.h"
#include "Graphics/Renderer/glebo.h"
#include "Graphics/Renderer/glibo.h"
#include "Graphics/Renderer/glfbo.h"

namespace Renderer
{
    class VertexArray : public glvao {};
    class VertexBuffer : public glvbo {};
    class ElementBuffer : public glebo {};
    class IndexBuffer : public glibo {};
    class FrameBuffer : public glfbo {};
}
