#pragma once

#include "Logger.h"
#include "Graphics/Renderer/Vulkan/VkUtils.h"
#include "Graphics/Renderer/Vulkan/VkDevice.h"
#include "Graphics/Renderer/Vulkan/VkWrapper.h"
#include "Graphics/Renderer/Vulkan/VkQueue.h"
#include "Graphics/Renderer/Vulkan/VkShader.h"
#include "Graphics/Renderer/Vulkan/VkPipeline.h"
#include "Graphics/Renderer/Vulkan/VkMesh.h"
#include "Graphics/Renderer/Vulkan/VkBufferMem.h"

#include "Window/GLFW/glfw.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    class Instance
    {
    private:
        int CreateInstance(const char* pAppName, const char* pEngineName);
        int CreateDebugCallback();
        int CreateSurface(GLFWwindow* pWindow);
        int CreateDevice();
        int CreateSwapChain();
        int CreateCommandPool();
        BufferAndMemory CreateBuffer(VkDeviceSize Size, VkBufferUsageFlags Usage, VkMemoryPropertyFlags Properties);
        void CopyBuffer(VkBuffer Dst, VkBuffer Src, VkDeviceSize Size);
        void SubmitCopyCommand();

        GLFWwindow* m_pWindow = nullptr;

        VkInstance m_Instance = nullptr;
        VkSurfaceKHR m_Surface = nullptr;
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;
        VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
        VkCommandPool m_CommandPool = VK_NULL_HANDLE;
        VkCommandBuffer m_CopyCommandBuf = VK_NULL_HANDLE;
        VkSurfaceFormatKHR m_SwapChainSurfaceFormat = {};
        
        std::vector<VkImage> m_Images;
	    std::vector<VkImageView> m_ImageViews;
        std::vector<VkFramebuffer> m_FrameBuffers;

        LOG_Device m_PhysicalDevice;
        LOG_Queue m_Queue;
        uint32_t m_PhysicalDeviceQueueFamily = 0;
    public:
        Instance();
        ~Instance();

        int Init(const char* appName, const char* pEngineName, GLFWwindow* pWindow);
        int CreateCommandBuffers(uint32_t count, VkCommandBuffer* pCmdBufs);
        VkRenderPass CreateSimpleRenderPass();
        std::vector<VkFramebuffer> CreateFrameBuffers(VkRenderPass renderPass);
        BufferAndMemory CreateVertexBuffer(const void* pVertices, size_t size);

        LOG_Queue* GetQueue() { return &m_Queue; }
        const VkImage& GetImage(int Index) const;
        uint32_t GetSwapChainImagesCount() const { return static_cast<uint32_t>(m_Images.size()); }
        std::vector<VkImage> GetSwapChainImages() const { return m_Images; }
        VkDevice& GetDevice() { return m_Device; }
        uint32_t GetMemoryTypeIndex(uint32_t MemTypeBitsMask, VkMemoryPropertyFlags ReqMemPropFlags);

        void Render();
    };
}