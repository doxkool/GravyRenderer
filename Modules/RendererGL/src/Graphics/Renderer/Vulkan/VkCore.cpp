#include "VkCore.h"

#include "Graphics\Renderer\Vulkan\VkUtils.h"

namespace VkCore
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
        VkDebugUtilsMessageTypeFlagsEXT Type,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        printf("\n---- Debug callback ----\n");
        printf("%s\n", pCallbackData->pMessage);
	    printf("  Severity: %s\n", GetDebugSeverityStr(Severity));
	    printf("  Type: %s\n", GetDebugType(Type));
	    printf("  Objects: ");

        for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
            #ifdef _WIN32
                    printf("%llux ", pCallbackData->pObjects[i].objectHandle);
            #else
                    printf("%lux ", pCallbackData->pObjects[i].objectHandle);
            #endif
                }
            
        printf("\n");

        return VK_FALSE;  // The calling function should not be aborted
    }

    static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& SurfaceFormats)
    {
        for (const auto& Format : SurfaceFormats) {
            if (Format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && Format.format == VK_FORMAT_B8G8R8A8_UNORM) {
                return Format;
            }
        }

        return SurfaceFormats[0];
    }

    static VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& PresentModes)
    {
        for (const auto& Mode : PresentModes) {
            if (Mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return Mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    static uint32_t ChooseNumImages(const VkSurfaceCapabilitiesKHR& SurfaceCapabilities)
    {
        uint32_t NumImages = SurfaceCapabilities.minImageCount + 1;

        int FinalNumImages = 0;

        if ((SurfaceCapabilities.maxImageCount > 0) && (NumImages > SurfaceCapabilities.maxImageCount)) {
            FinalNumImages = SurfaceCapabilities.maxImageCount;
        }else{
            FinalNumImages = NumImages;
        }

        return FinalNumImages;
    }

    VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectMask, VkImageViewType viewType, uint32_t layerCount, uint32_t mipLevels)
    {
        VkImageViewCreateInfo CreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = viewType,
            .format = format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_G,
                .b = VK_COMPONENT_SWIZZLE_B,
                .a = VK_COMPONENT_SWIZZLE_A
            },
            .subresourceRange = {
                .aspectMask = aspectMask,
                .baseMipLevel = 0,
                .levelCount = mipLevels,
                .baseArrayLayer = 0,
                .layerCount = layerCount
            }
        };

        VkImageView imageView;
        VkResult res = vkCreateImageView(device, &CreateInfo, nullptr, &imageView);
        CHECK_VK_RESULT(res, "Failed to create image view!");

        return imageView;
    }

    Instance::Instance()
    {
    }
    
    Instance::~Instance()
    {
        VKCORE_DEBUG("---- Shutting down Vulkan instance ----");

        for (size_t i = 0; i < m_FrameBuffers.size(); i++) {
            vkDestroyFramebuffer(m_Device, m_FrameBuffers[i], nullptr);
        }
        m_FrameBuffers.clear();

        m_Queue.Destroy();

        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &m_CopyCommandBuf);

	    vkDestroyCommandPool(m_Device, m_CommandPool, NULL);

        for (size_t i = 0; i < m_ImageViews.size(); i++) {
            vkDestroyImageView(m_Device, m_ImageViews[i], nullptr);
        }
        m_ImageViews.clear();
        m_Images.clear();

        vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
        m_SwapChain = VK_NULL_HANDLE;

        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        m_Surface = VK_NULL_HANDLE;

        vkDeviceWaitIdle(m_Device);

        vkDestroyDevice(m_Device, nullptr);
        
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessenger = VK_NULL_HANDLE;
	    vkDestroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
	    if (!vkDestroyDebugUtilsMessenger) 
        {
	    	VKCORE_ERROR("Cannot find address of vkDestroyDebugUtilsMessengerEXT!");
	    	exit(1);
	    }else{
            vkDestroyDebugUtilsMessenger(m_Instance, m_DebugMessenger, NULL);
	        VKCORE_DEBUG("Debug callback destroyed.");
        }

        vkDestroyInstance(m_Instance, NULL);
        VKCORE_DEBUG("Vulkan instance has been destroyed.");
    }
    
    int Instance::Init(const char* pAppName, const char* pEngineName, GLFWwindow* pWindow)
    {
        m_pWindow = pWindow;

        Logger::Init();
        CreateInstance(pAppName, pEngineName);
        CreateDebugCallback();
        CreateSurface(pWindow);
        
        m_PhysicalDevice.Init(m_Instance, m_Surface);
        m_PhysicalDeviceQueueFamily = m_PhysicalDevice.SelectPhysicalDevice(VK_QUEUE_GRAPHICS_BIT, true);

        CreateDevice();

        CreateSwapChain();
        CreateCommandPool();

        m_Queue.Init(m_Device, m_SwapChain, m_PhysicalDeviceQueueFamily, 0);

        CreateCommandBuffers(1, &m_CopyCommandBuf);

        return 0;
    }

    int Instance::CreateInstance(const char* pAppName, const char* pEngineName)
    {
        std::vector<const char*> Layers = {
            "VK_LAYER_KHRONOS_validation"
        };
        
        std::vector<const char*> Extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,

            #if defined (_WIN32)
                    "VK_KHR_win32_surface",
            #endif
            #if defined (__APPLE__)
                    "VK_MVK_macos_surface",
            #endif
            #if defined (__linux__)
                    "VK_KHR_wayland_surface",
                    "VK_KHR_xlib_surface",
                    "VK_KHR_xcb_surface",
            #endif

            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        VkApplicationInfo AppInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = pAppName,
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = pEngineName,
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_3
        };
    
        VkInstanceCreateInfo CreateInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,				// reserved for future use. Must be zero
            .pApplicationInfo = &AppInfo,
            .enabledLayerCount = (uint32_t)(Layers.size()),
            .ppEnabledLayerNames = Layers.data(),
            .enabledExtensionCount = (uint32_t)(Extensions.size()),
            .ppEnabledExtensionNames = Extensions.data()
        };

        VkResult res = vkCreateInstance(&CreateInfo, nullptr, &m_Instance);
        CHECK_VK_RESULT(res, "Create instance");

        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to create Vulkan instance!");
            return -1;
        }

        VKCORE_DEBUG("Vulkan instance created.");
        return 0;
    }

    const VkImage& Instance::GetImage(int Index) const
{
	if (Index >= m_Images.size()) {
		VKCORE_ERROR("Invalid image index {}", Index);
		exit(1);
	}

	return m_Images[Index];
}

    int Instance::CreateDebugCallback()
    {
        VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = NULL,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = &DebugCallback,
            .pUserData = NULL
        };
    
        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessenger = VK_NULL_HANDLE;
        vkCreateDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
        if (!vkCreateDebugUtilsMessenger) {
            VKCORE_ERROR("Cannot find address of vkCreateDebugUtilsMessenger");
            exit(1);
        }
    
        VkResult res = vkCreateDebugUtilsMessenger(m_Instance, &MessengerCreateInfo, NULL, &m_DebugMessenger);
        CHECK_VK_RESULT(res, "debug utils messenger");

        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to create debug utils messenger!");
            return -1;
        }
    
        VKCORE_DEBUG("Debug utils messenger created");
        return 0;
    }

    int Instance::CreateSurface(GLFWwindow* pWindow)
    {
        // Create surface
        VkResult res = glfwCreateWindowSurface(m_Instance, pWindow, NULL, &m_Surface);
        CHECK_VK_RESULT(res, "Failed to create window surface!");

        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to create window surface!");
            return -1;
        }

        VKCORE_DEBUG("Window surface created.");
        return 0;
    }

    int Instance::CreateDevice()
    {
        float queuePriority[] = { 1.0f };

        VkDeviceQueueCreateInfo QueueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = m_PhysicalDeviceQueueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority[0]
        };

        std::vector<const char*> DeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
        };

        if (m_PhysicalDevice.GetDevice().m_PhysicalDeviceFeatures.geometryShader == VK_FALSE) {
            VKCORE_ERROR("Geometry shader not supported!");
            return -1;
        }
        
        if (m_PhysicalDevice.GetDevice().m_PhysicalDeviceFeatures.tessellationShader == VK_FALSE) {
            VKCORE_ERROR("Tessellation shader not supported!");
            return -1;
        }

        VkPhysicalDeviceFeatures DeviceFeatures = {
            .geometryShader = VK_TRUE,
            .tessellationShader = VK_TRUE
        };

        VkDeviceCreateInfo DeviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &QueueCreateInfo,
            .enabledLayerCount = 0,             // DEPRECATED
            .ppEnabledLayerNames = nullptr,     // DEPRECATED
            .enabledExtensionCount = (uint32_t)(DeviceExtensions.size()),
            .ppEnabledExtensionNames = DeviceExtensions.data(),
            .pEnabledFeatures = &DeviceFeatures
        };

        VkResult res = vkCreateDevice(m_PhysicalDevice.GetDevice().m_PhysicalDevice, &DeviceCreateInfo, nullptr, &m_Device);
        CHECK_VK_RESULT(res, "Failed to create logical device!");

        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to create logical device!");
            return -1;
        }

        VKCORE_DEBUG("Logical device created.");
        return 0;
    }

    int Instance::CreateSwapChain()
    {
        const VkSurfaceCapabilitiesKHR SurfaceCapabilities = m_PhysicalDevice.GetDevice().m_SurfaceCapabilities;

        uint32_t NumImages = ChooseNumImages(SurfaceCapabilities);

        const std::vector<VkPresentModeKHR> PresentModes = m_PhysicalDevice.GetDevice().m_PresentModes;
        VkPresentModeKHR PresentMode = ChoosePresentMode(PresentModes);

        m_SwapChainSurfaceFormat = ChooseSurfaceFormat(m_PhysicalDevice.GetDevice().m_SurfaceFormats);

        VkSwapchainCreateInfoKHR SwapChainCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = m_Surface,
            .minImageCount = NumImages,
            .imageFormat = m_SwapChainSurfaceFormat.format,
            .imageColorSpace = m_SwapChainSurfaceFormat.colorSpace,
            .imageExtent = SurfaceCapabilities.currentExtent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &m_PhysicalDeviceQueueFamily,
            .preTransform = SurfaceCapabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = PresentMode,
            .clipped = VK_TRUE
        };

        VkResult res = vkCreateSwapchainKHR(m_Device, &SwapChainCreateInfo, nullptr, &m_SwapChain);
        CHECK_VK_RESULT(res, "Failed to create swap chain!");

        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to create swap chain!");
            return -1;
        }

        VKCORE_DEBUG("Swap chain created.");

        uint32_t NumSwapChainImages = 0;
        res = vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &NumSwapChainImages, nullptr);
        CHECK_VK_RESULT(res, "Failed to get swap chain images!");
        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to get swap chain images!");
            return -1;
        }

        m_Images.resize(NumSwapChainImages);
        m_ImageViews.resize(NumSwapChainImages);
        res = vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &NumSwapChainImages, m_Images.data());
        CHECK_VK_RESULT(res, "Failed to get swap chain images!");
        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to get swap chain images!");
            return -1;
        }

        uint32_t LayerCount = 1;
        uint32_t MipLevels = 1;
        for (uint32_t i = 0; i < NumSwapChainImages; i++) {
            m_ImageViews[i] = CreateImageView(m_Device, m_Images[i], m_SwapChainSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, LayerCount, MipLevels);
        }

        return 0;
    }

    int Instance::CreateCommandPool()
    {
        VkCommandPoolCreateInfo CommandPoolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = m_PhysicalDeviceQueueFamily
        };

        VkResult res = vkCreateCommandPool(m_Device, &CommandPoolCreateInfo, nullptr, &m_CommandPool);
        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to create command pool!");
            return -1;
        }
        VKCORE_DEBUG("Command pool created.");
        return 0;
    }

    int Instance::CreateCommandBuffers(uint32_t count, VkCommandBuffer* pCmdBufs)
    {
        VkCommandBufferAllocateInfo cmdBufAllocInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = NULL,
            .commandPool = m_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = count
        };
    
        VkResult res = vkAllocateCommandBuffers(m_Device, &cmdBufAllocInfo, pCmdBufs);
        CHECK_VK_RESULT(res, "vkAllocateCommandBuffers\n");
    
        printf("%d command buffers created\n", count);
        
        return 0;
    }

    VkRenderPass Instance::CreateSimpleRenderPass()
    {
        VkAttachmentDescription AttachmentDescription = {
            .flags = 0,
            .format = m_SwapChainSurfaceFormat.format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

        VkAttachmentReference ColorAttachmentRef = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription SubpassDescription = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &ColorAttachmentRef,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };

        VkRenderPassCreateInfo RenderPassCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount = 1,
            .pAttachments = &AttachmentDescription,
            .subpassCount = 1,
            .pSubpasses = &SubpassDescription,
            .dependencyCount = 0,
            .pDependencies = nullptr
        };

        VkRenderPass RenderPass;

        VkResult res = vkCreateRenderPass(m_Device, &RenderPassCreateInfo, nullptr, &RenderPass);
        if (res != VK_SUCCESS) {
            VKCORE_ERROR("Failed to create render pass!");
            return RenderPass;
        }

        VKCORE_DEBUG("Render pass created.");
        return RenderPass;
    }

    std::vector<VkFramebuffer> Instance::CreateFrameBuffers(VkRenderPass renderPass)
    {
        m_FrameBuffers.resize(m_Images.size());

        int WinddowWidth, WindowHeight;
        glfwGetWindowSize(m_pWindow, &WinddowWidth, &WindowHeight);

        for (size_t i = 0; i < m_Images.size(); i++) {
            VkFramebufferCreateInfo FrameBufferCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = renderPass,
                .attachmentCount = 1,
                .pAttachments = &m_ImageViews[i],
                .width = (uint32_t)WinddowWidth,
                .height = (uint32_t)WindowHeight,
                .layers = 1
            };

            VkResult res = vkCreateFramebuffer(m_Device, &FrameBufferCreateInfo, nullptr, &m_FrameBuffers[i]);
            CHECK_VK_RESULT(res, "Failed to create framebuffer!");

            if (res != VK_SUCCESS) {
                VKCORE_ERROR("Failed to create framebuffer!");
                return {};
            }
        }
        return m_FrameBuffers;
    }

    uint32_t Instance::GetMemoryTypeIndex(uint32_t MemTypeBitsMask, VkMemoryPropertyFlags ReqMemPropFlags)
    {
    	const VkPhysicalDeviceMemoryProperties& MemProps = m_PhysicalDevice.GetDevice().m_PhysicalDeviceMemoryProperties;
    	for (uint i = 0; i < MemProps.memoryTypeCount; i++) {
    		const VkMemoryType& MemType = MemProps.memoryTypes[i];
    		uint CurBitmask = (1 << i);
    		bool IsCurMemTypeSupported = (MemTypeBitsMask & CurBitmask);
    		bool HasRequiredMemProps = ((MemType.propertyFlags & ReqMemPropFlags) == ReqMemPropFlags);

    		if (IsCurMemTypeSupported && HasRequiredMemProps) {
    			return i;
    		}
    	}

    	VKCORE_CRITICAL("Cannot find memory type for type %x requested mem props %x\n", MemTypeBitsMask, ReqMemPropFlags);
    	exit(1);
    	return -1;
    }

    BufferAndMemory Instance::CreateBuffer(VkDeviceSize Size, VkBufferUsageFlags Usage, VkMemoryPropertyFlags Properties)
    {
        VkBufferCreateInfo vbCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = Size,
            .usage = Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        BufferAndMemory Buf;

        // Step 1: create a buffer
        VkResult res = vkCreateBuffer(m_Device, &vbCreateInfo, NULL, &Buf.m_buffer);
        CHECK_VK_RESULT(res, "vkCreateBuffer\n");
        VKCORE_DEBUG("Buffer created\n");

        // Step 2: get the buffer memory requirements
        VkMemoryRequirements MemReqs = { 0 };
        vkGetBufferMemoryRequirements(m_Device, Buf.m_buffer, &MemReqs);
        VKCORE_TRACE("Buffer requires {} bytes.", (int)MemReqs.size);

        Buf.m_allocationSize = MemReqs.size;

        // Step 3: get the memory type index
        uint32_t MemoryTypeIndex = GetMemoryTypeIndex(MemReqs.memoryTypeBits, Properties);
        VKCORE_TRACE("Memory type index {}.", MemoryTypeIndex);

        // Step 4: allocate memory
        VkMemoryAllocateInfo MemAllocInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = NULL,
            .allocationSize = MemReqs.size,
            .memoryTypeIndex = MemoryTypeIndex
        };

        res = vkAllocateMemory(m_Device, &MemAllocInfo, NULL, &Buf.m_mem);
        CHECK_VK_RESULT(res, "vkAllocateMemory error %d\n");

        // Step 5: bind memory
        res = vkBindBufferMemory(m_Device, Buf.m_buffer, Buf.m_mem, 0);
        CHECK_VK_RESULT(res, "vkBindBufferMemory error %d\n");

        return Buf;
    }

    void Instance::SubmitCopyCommand()
    {
        vkEndCommandBuffer(m_CopyCommandBuf);
        m_Queue.SubmitSync(m_CopyCommandBuf);
        m_Queue.WaitIdle();
    }

    void Instance::CopyBuffer(VkBuffer Dst, VkBuffer Src, VkDeviceSize Size)
    {
    	BeginCommandBuffer(m_CopyCommandBuf, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    	VkBufferCopy BufferCopy = {
    		.srcOffset = 0,
    		.dstOffset = 0,
    		.size = Size
    	};

    	vkCmdCopyBuffer(m_CopyCommandBuf, Src, Dst, 1, &BufferCopy);

    	SubmitCopyCommand();
    }

    BufferAndMemory Instance::CreateVertexBuffer(const void* pVertices, size_t size)
    {
        // Step 1: create the staging buffer
	    VkBufferUsageFlags Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	    VkMemoryPropertyFlags MemProps = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	    								 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	    BufferAndMemory StagingVB = CreateBuffer(size, Usage, MemProps);

	    // Step 2: map the memory of the stage buffer
	    void* pMem = nullptr;
	    VkDeviceSize Offset = 0;
	    VkMemoryMapFlags Flags = 0;
	    VkResult res = vkMapMemory(m_Device, StagingVB.m_mem, Offset, 
	    	                       StagingVB.m_allocationSize, Flags, &pMem);
	    CHECK_VK_RESULT(res, "vkMapMemory\n");

	    // Step 3: copy the vertices to the staging buffer
        pMem = malloc(size);
	    memcpy(pMem, pVertices, size);

	    // Step 4: unmap/release the mapped memory
	    vkUnmapMemory(m_Device, StagingVB.m_mem);

	    // Step 5: create the final buffer
	    Usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	    MemProps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	    BufferAndMemory VB = CreateBuffer(size, Usage, MemProps);

	    // Step 6: copy the staging buffer to the final buffer
	    CopyBuffer(VB.m_buffer, StagingVB.m_buffer, size);

	    // Step 7: release the resources of the staging buffer
	    StagingVB.Destroy(m_Device);

	    return VB;
    }

    void Instance::Render()
    {
        
    }
}