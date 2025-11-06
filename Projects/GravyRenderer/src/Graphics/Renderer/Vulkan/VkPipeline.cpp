#include "Graphics\Renderer\Vulkan\VkPipeline.h"

namespace VkCore
{    
    VkCore_Pipeline::VkCore_Pipeline(VkDevice device, GLFWwindow* pWindow, VkRenderPass renderPass,
        VkShaderModule vs, VkShaderModule fs,
        Mesh* pMesh, uint32_t NumberImages)
    {
        m_Device = device;

        if (pMesh) {
            CreateDescriptorSets(pMesh, NumberImages);
        }

        VkPipelineShaderStageCreateInfo shaderStagesInfo[2] = {
            {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vs,
            .pName = "main",
            },
            {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fs,
            .pName = "main",
            }
        };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        };

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        };

        int WindowWidth, WindowHeight;
	    glfwGetWindowSize(pWindow, &WindowWidth, &WindowHeight);

        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)WindowWidth,
            .height = (float)WindowHeight,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        VkRect2D scissor = {
            .offset = { 0, 0 },
            .extent = { (uint32_t)WindowWidth, (uint32_t)WindowHeight }
        };

        VkPipelineViewportStateCreateInfo viewportStateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor
        };

        VkPipelineRasterizationStateCreateInfo rasterizerInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0f,
            .depthBiasSlopeFactor = 0.0f,
            .lineWidth = 1.0f
        };

        VkPipelineMultisampleStateCreateInfo multisamplingInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE
        };

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {
            .blendEnable = VK_FALSE,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
        };

        VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment,
            .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
        };

        VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .dynamicStateCount = 0,
            .pDynamicStates = nullptr
        };

        VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthTestEnable = VK_FALSE,
            .depthWriteEnable = VK_FALSE,
            .depthCompareOp = VK_COMPARE_OP_ALWAYS,
            .depthBoundsTestEnable = VK_FALSE,
            .stencilTestEnable = VK_FALSE,
            .front = {},
            .back = {},
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f
        };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
        };

        if (pMesh && pMesh->m_VertexBuffer.m_buffer)
        {
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
        }else{
            pipelineLayoutInfo.setLayoutCount = 0;
            pipelineLayoutInfo.pSetLayouts = nullptr;
        }

        VkResult res = vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);
        CHECK_VK_RESULT(res, "vkCreatePipelineLayout\n");

        VkGraphicsPipelineCreateInfo pipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stageCount = 2,
            .pStages = &shaderStagesInfo[0],
            .pVertexInputState = &vertexInputInfo,
            .pInputAssemblyState = &inputAssemblyInfo,
            .pTessellationState = nullptr,
            .pViewportState = &viewportStateInfo,
            .pRasterizationState = &rasterizerInfo,
            .pMultisampleState = &multisamplingInfo,
            //.pDepthStencilState = &depthStencilInfo,
            .pColorBlendState = &colorBlendingInfo,
            //.pDynamicState = &dynamicStateInfo,
            .layout = m_PipelineLayout,
            .renderPass = renderPass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1
        };

        res = vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline);
        CHECK_VK_RESULT(res, "vkCreateGraphicsPipelines\n");
        if (res != VK_SUCCESS)
        {
            VKCORE_ERROR("Failed to create graphics pipeline!");
            return;
        }
        else
        {
            VKCORE_DEBUG("Created graphics pipeline.");
        }
    }
    
    VkCore_Pipeline::~VkCore_Pipeline()
    {
        vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, NULL);
	    vkDestroyPipelineLayout(m_Device, m_PipelineLayout, NULL);
	    vkDestroyDescriptorPool(m_Device, m_DescriptorPool, NULL);
	    vkDestroyPipeline(m_Device, m_Pipeline, NULL);
    }

    void VkCore_Pipeline::CreateDescriptorSets(Mesh* pMesh, uint32_t NumberImages)
    {
        CreateDescriptorPool(NumberImages);
        CreateDescriptorSetLayout();
        AllocateDescriptorSets(NumberImages);
        UpdateDescriptorSets(pMesh, NumberImages);
    }

    void VkCore_Pipeline::CreateDescriptorPool(uint32_t NumberImages)
    {
        VkDescriptorPoolSize poolSize = {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = NumberImages
        };

        VkDescriptorPoolCreateInfo poolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = NumberImages,
            .poolSizeCount = 1,
            .pPoolSizes = &poolSize
        };

        VkResult res = vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool);
        CHECK_VK_RESULT(res, "vkCreateDescriptorPool\n");
        if (res != VK_SUCCESS)
        {
            VKCORE_ERROR("Failed to create descriptor pool!");
            return;
        }
        else
        {
            VKCORE_DEBUG("Created descriptor pool.");
        }
    }

    void VkCore_Pipeline::CreateDescriptorSetLayout()
    {
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

        VkDescriptorSetLayoutBinding layoutBinding = {
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .pImmutableSamplers = nullptr
        };

        layoutBindings.push_back(layoutBinding);

        VkDescriptorSetLayoutCreateInfo layoutInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = (uint32_t)layoutBindings.size(),
            .pBindings = layoutBindings.data()
        };

        VkResult res = vkCreateDescriptorSetLayout(m_Device, &layoutInfo, nullptr, &m_DescriptorSetLayout);
        CHECK_VK_RESULT(res, "vkCreateDescriptorSetLayout\n");
        if (res != VK_SUCCESS)
        {
            VKCORE_ERROR("Failed to create descriptor set layout!");
            return;
        }
        else
        {
            VKCORE_DEBUG("Created descriptor set layout.");
        }
    }
    
    void VkCore_Pipeline::AllocateDescriptorSets(uint32_t NumberImages)
    {
        std::vector<VkDescriptorSetLayout> layouts(NumberImages, m_DescriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = m_DescriptorPool,
            .descriptorSetCount = NumberImages,
            .pSetLayouts = layouts.data()
        };

        m_DescriptorSets.resize(NumberImages);

        VkResult res = vkAllocateDescriptorSets(m_Device, &allocInfo, m_DescriptorSets.data());
        CHECK_VK_RESULT(res, "vkAllocateDescriptorSets\n");
        if (res != VK_SUCCESS)
        {
            VKCORE_ERROR("Failed to allocate descriptor sets!");
            return;
        }
        else
        {
            VKCORE_DEBUG("Allocated descriptor sets.");
        }
    }
    
    void VkCore_Pipeline::UpdateDescriptorSets(Mesh* pMesh, uint32_t NumberImages)
    {
        for (size_t i = 0; i < NumberImages; i++)
        {
            VkDescriptorBufferInfo bufferInfo = {
                .buffer = pMesh->m_VertexBuffer.m_buffer,
                .offset = 0,
                .range = VK_WHOLE_SIZE
            };

            std::vector<VkWriteDescriptorSet> descriptorWrites;

            descriptorWrites.push_back(
                VkWriteDescriptorSet {
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext = nullptr,
                    .dstSet = m_DescriptorSets[i],
                    .dstBinding = 0,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .pImageInfo = nullptr,
                    .pBufferInfo = &bufferInfo,
                    .pTexelBufferView = nullptr
                }
            );

            vkUpdateDescriptorSets(m_Device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        }
    }
    

    void VkCore_Pipeline::Bind(VkCommandBuffer CmdBuf, int32_t ImageIndex)
    {
        vkCmdBindPipeline(CmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

        if (m_DescriptorSetLayout)
        {
            vkCmdBindDescriptorSets(CmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[ImageIndex], 0, nullptr);
        }
    }
}