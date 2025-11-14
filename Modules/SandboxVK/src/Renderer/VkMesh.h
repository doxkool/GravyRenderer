#pragma once

#include "Graphics/Renderer/Vulkan/VkUtils.h"
#include "Graphics/Renderer/Vulkan/VkBufferMem.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    struct Mesh
    {
        BufferAndMemory m_VertexBuffer;
	    size_t m_VertexBufferSize = 0;
	    //VulkanTexture* m_pTex = NULL;

	    void Destroy(VkDevice Device)
	    {
	    	m_VertexBuffer.Destroy(Device);

	    	//if (m_pTex) {
	    	//	m_pTex->Destroy(Device);
	    	//	delete m_pTex;
	    	//}
	    }
    };
}