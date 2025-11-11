#include "Graphics/Renderer/Vulkan/VkWrapper.h"

namespace VkCore
{
    void BeginCommandBuffer(VkCommandBuffer cmdBuf, VkCommandBufferUsageFlagBits flags)
    {
        VkCommandBufferBeginInfo BeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = (VkCommandBufferUsageFlags)flags,
            .pInheritanceInfo = nullptr
        };

        VkResult res = vkBeginCommandBuffer(cmdBuf, &BeginInfo);
        if (res != VK_SUCCESS) {
            LOG_ERROR("Failed to reset command buffer!");
            return;
        }
    }

    VkSemaphore CreateSemaphore(VkDevice device)
    {
        VkSemaphoreCreateInfo semaphoreInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        };

        VkSemaphore semaphore;
        VkResult res = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore);
        CHECK_VK_RESULT(res, "Failed to create semaphore!");
        if (res != VK_SUCCESS) {
            LOG_ERROR("Failed to create semaphore!");
            return VK_NULL_HANDLE;
        }

        LOG_DEBUG("Semaphore created.");

        return semaphore;
    }
}