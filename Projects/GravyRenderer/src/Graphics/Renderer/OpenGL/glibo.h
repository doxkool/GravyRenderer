#pragma once

#include "Core/Base.h"

#include <GL/glew.h>

namespace Renderer
{
	class glibo
	{
	public:
		GLuint Create(uint32_t* indices, uint32_t count);

		void Bind();
		void UnBind();
		void Delete();

		uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}