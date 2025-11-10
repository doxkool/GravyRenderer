#pragma once

#include "Graphics\Renderer\Vulkan\VkLogger.h"
#include "Graphics\Renderer\Vulkan\VkUtils.h"

#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>

#include <vulkan/vulkan.h>

namespace VkCore
{
    VkShaderModule CreateFromBinary(VkDevice& device, const char* pShaderPath);
    VkShaderModule CreateFromFile(VkDevice& device, const char* pShaderPath);
    void Destroy();

    VkShaderModule GetShaderModule();
}