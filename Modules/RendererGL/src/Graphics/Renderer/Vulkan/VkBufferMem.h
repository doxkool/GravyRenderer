#pragma once

#include "Graphics\Renderer\Vulkan\VkUtils.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    class BufferAndMemory 
    {
        public:
        BufferAndMemory() {}

        VkBuffer m_buffer = nullptr;
        VkDeviceMemory m_mem = nullptr;
        VkDeviceSize m_allocationSize = 0;

        void Update(VkDevice Device, const void* pData, size_t Size)
        {
            void* pMem = NULL;
            VkResult res = vkMapMemory(Device, m_mem, 0, Size, 0, &pMem);
            CHECK_VK_RESULT(res, "vkMapMemory");
            memcpy(pMem, pData, Size);
            vkUnmapMemory(Device, m_mem);
        }

        void Destroy(VkDevice Device)
        {
            if (m_mem) {
                vkFreeMemory(Device, m_mem, NULL);
            }
            if (m_buffer) {
                vkDestroyBuffer(Device, m_buffer, NULL);
            }
        }
    };
}