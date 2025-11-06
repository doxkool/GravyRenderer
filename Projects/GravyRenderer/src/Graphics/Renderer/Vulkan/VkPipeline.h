#pragma once

#include "Core/Base.h"

#include "Graphics\Renderer\Vulkan\VkUtils.h"
#include "Graphics\Renderer\Vulkan\VkLogger.h"
#include "Graphics\Renderer\Vulkan\VkMesh.h"

#include "Window/GLFW/glfw.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    class VkCore_Pipeline
    {
    private:
        void CreateDescriptorSets(Mesh* pMesh, uint32_t NumberImages);
        void CreateDescriptorPool(uint32_t NumberImages);
        void CreateDescriptorSetLayout();
        void AllocateDescriptorSets(uint32_t NumberImages);
        void UpdateDescriptorSets(Mesh* pMesh, uint32_t NumberImages);

        VkPipeline m_Pipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
        std::vector<VkDescriptorSet> m_DescriptorSets;
        VkDevice m_Device = VK_NULL_HANDLE;
    public:
        VkCore_Pipeline(VkDevice device, GLFWwindow* pWindow, VkRenderPass renderPass,
            VkShaderModule vs, VkShaderModule fs,
            Mesh* pMesh, uint32_t NumberImages);

        ~VkCore_Pipeline();

        void Bind(VkCommandBuffer CmdBuf, int32_t ImageIndex);
    };
    
}