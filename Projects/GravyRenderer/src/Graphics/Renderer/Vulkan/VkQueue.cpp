#include "Graphics\Renderer\Vulkan\VkQueue.h"

namespace VkCore
{
    LOG_Queue::LOG_Queue()
    {
    }
    
    LOG_Queue::~LOG_Queue()
    {
    }

    void LOG_Queue::CreateSemaphores()
    {
        m_RenderComplete = CreateSemaphore(m_Device);
        m_PresentComplete = CreateSemaphore(m_Device);
    }

    void LOG_Queue::Init(VkDevice device, VkSwapchainKHR swapchain, uint32_t queueFamily, uint32_t queueFamilyIndex)
    {
        m_Device = device;
        m_SwapChain = swapchain;

        vkGetDeviceQueue(device, queueFamily, queueFamilyIndex, &m_Queue);

        LOG_DEBUG("Queue created.");
        CreateSemaphores();
    }

    void LOG_Queue::WaitIdle()
    {
        vkQueueWaitIdle(m_Queue);
    }

    void LOG_Queue::Destroy()
    {
        LOG_DEBUG("Destroying queue...");
        vkDestroySemaphore(m_Device, m_RenderComplete, nullptr);
        vkDestroySemaphore(m_Device, m_PresentComplete, nullptr);
    }

    uint32_t LOG_Queue::AcquireNextImage()
    {
        uint32_t imageIndex = 0;
        VkResult res = vkAcquireNextImageKHR(m_Device, m_SwapChain, UINT64_MAX, m_PresentComplete, nullptr, &imageIndex);
        CHECK_VK_RESULT(res, "Failed to acquire next image!");
        if (res != VK_SUCCESS) {
            return -1;
        }
        return imageIndex;
    }

    void LOG_Queue::SubmitSync(VkCommandBuffer cmdBuf)
    {
        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = VK_NULL_HANDLE,
            .pWaitDstStageMask = VK_NULL_HANDLE,
            .commandBufferCount = 1,
            .pCommandBuffers = &cmdBuf,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = VK_NULL_HANDLE
        };

        VkResult res = vkQueueSubmit(m_Queue, 1, &submitInfo, VK_NULL_HANDLE);
        CHECK_VK_RESULT(res, "Failed to submit command buffer!");
        
    }

    void LOG_Queue::SubmitAsync(VkCommandBuffer cmdBuf, VkPipelineStageFlags waitStages)
    {
        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        
        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_PresentComplete,
            .pWaitDstStageMask = &waitFlags,
            .commandBufferCount = 1,
            .pCommandBuffers = &cmdBuf,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &m_RenderComplete
        };

        VkResult res = vkQueueSubmit(m_Queue, 1, &submitInfo, VK_NULL_HANDLE);
        CHECK_VK_RESULT(res, "Failed to submit command buffer!");
    }

    void LOG_Queue::Present(uint32_t imageIndex)
    {
        VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_RenderComplete,
            .swapchainCount = 1,
            .pSwapchains = &m_SwapChain,
            .pImageIndices = &imageIndex
        };

        VkResult res = vkQueuePresentKHR(m_Queue, &presentInfo);
        CHECK_VK_RESULT(res, "Failed to present image!");

        WaitIdle();
    }
}