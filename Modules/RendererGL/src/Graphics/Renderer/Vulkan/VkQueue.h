#pragma once

#include "Graphics\Renderer\Vulkan\VkLogger.h"
#include "Graphics\Renderer\Vulkan\VkWrapper.h"
#include "Graphics\Renderer\Vulkan\VkUtils.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    class VkCore_Queue
    {
    private:
        void CreateSemaphores();

        VkDevice m_Device = nullptr;
        VkSwapchainKHR m_SwapChain = nullptr;
        VkQueue m_Queue = nullptr;
        VkSemaphore m_RenderComplete = nullptr;
        VkSemaphore m_PresentComplete = nullptr;
    public:
        VkCore_Queue();
        ~VkCore_Queue();

        void Init(VkDevice device, VkSwapchainKHR swapchain, uint32_t queueFamily, uint32_t queueFamilyIndex);
        void Destroy();
        uint32_t AcquireNextImage();
        void SubmitSync(VkCommandBuffer cmdBuf);
        void SubmitAsync(VkCommandBuffer cmdBuf, VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        void Present(uint32_t imageIndex);
        void WaitIdle();
    };    
}