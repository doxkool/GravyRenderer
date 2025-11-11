#pragma once

#include "Logger.h"
#include "Graphics/Renderer/Vulkan/VkUtils.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    void BeginCommandBuffer(VkCommandBuffer cmdBuf, VkCommandBufferUsageFlagBits flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    
    VkSemaphore CreateSemaphore(VkDevice device);
}