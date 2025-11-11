#include "Graphics/Renderer/Vulkan/VkDevice.h"

namespace VkCore
{
    static void PrintImageUsageFlags(const VkImageUsageFlags& flags)
    {
        if (flags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
            printf("Image usage transfer src is supported\n");
        }
    
        if (flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
            printf("Image usage transfer dest is supported\n");
        }
    
        if (flags & VK_IMAGE_USAGE_SAMPLED_BIT) {
            printf("Image usage sampled is supported\n");
        }
    
        if (flags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
            printf("Image usage color attachment is supported\n");
        }
    
        if (flags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            printf("Image usage depth stencil attachment is supported\n");
        }
    
        if (flags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
            printf("Image usage transient attachment is supported\n");
        }
    
        if (flags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) {
            printf("Image usage input attachment is supported\n");
        }
    }
    
    
    static void PrintMemoryProperty(VkMemoryPropertyFlags PropertyFlags)
    {
        if (PropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
            printf("DEVICE LOCAL ");
        }
    
        if (PropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            printf("HOST VISIBLE ");
        }
    
        if (PropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
            printf("HOST COHERENT ");
        }
    
        if (PropertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
            printf("HOST CACHED ");
        }
    
        if (PropertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) {
            printf("LAZILY ALLOCATED ");
        }
    
        if (PropertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT) {
            printf("PROTECTED ");
        }
    }

    void LOG_Device::Init(const VkInstance& vkInstance, const VkSurfaceKHR& vkSurface)
    {
        uint32_t deviceCount = 0;
        VkResult res = vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
        CHECK_VK_RESULT(res, "Failed to enumerate physical devices!");

        printf("\nNumber of physical devices: %d\n", deviceCount);
        PhysicalDevices.resize(deviceCount);

        std::vector<VkPhysicalDevice> devices(deviceCount);

        res = vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());
        CHECK_VK_RESULT(res, "Failed to enumerate physical devices!");

        for (uint32_t i = 0; i < deviceCount; i++)
        {
            PhysicalDevices[i].m_PhysicalDevice = devices[i];

            vkGetPhysicalDeviceProperties(devices[i], &PhysicalDevices[i].m_PhysicalDeviceProperties);

            printf("Device name: %s\n", PhysicalDevices[i].m_PhysicalDeviceProperties.deviceName);
            
            uint32_t apiVer = PhysicalDevices[i].m_PhysicalDeviceProperties.apiVersion;
            printf("    API version: %d.%d.%d.%d\n", 
                VK_API_VERSION_VARIANT(apiVer), 
                VK_API_VERSION_MAJOR(apiVer), 
                VK_API_VERSION_MINOR(apiVer), 
                VK_API_VERSION_PATCH(apiVer));

            vkGetPhysicalDeviceMemoryProperties(devices[i], &PhysicalDevices[i].m_PhysicalDeviceMemoryProperties);

            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, nullptr);
            printf("    Number of queue families: %d\n", queueFamilyCount);

            PhysicalDevices[i].m_QueueFamilyProperties.resize(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, PhysicalDevices[i].m_QueueFamilyProperties.data());

            PhysicalDevices[i].m_QueueFamilyPresentSupport.resize(queueFamilyCount);
            for (uint32_t j = 0; j < queueFamilyCount; j++)
            {
                const VkQueueFamilyProperties& queueFamilyProperties = PhysicalDevices[i].m_QueueFamilyProperties[j];
                printf("    Family %d Num queues: %d ", j, queueFamilyProperties.queueCount);
                VkQueueFlags queueFlags = queueFamilyProperties.queueFlags;
                printf("    GFX %s, Compute %s, Transfer %s, Sparse binding %s\n",
                    (queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No",
                    (queueFlags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No",
                    (queueFlags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No",
                    (queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No");

                res = vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], j, vkSurface, &PhysicalDevices[i].m_QueueFamilyPresentSupport[j]);
                CHECK_VK_RESULT(res, "Failed to get physical device surface support!");
            }

            uint32_t formatCount = 0;
            res = vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], vkSurface, &formatCount, nullptr);
            CHECK_VK_RESULT(res, "Failed to get physical device surface formats!");

            PhysicalDevices[i].m_SurfaceFormats.resize(formatCount);

            res = vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], vkSurface, &formatCount, PhysicalDevices[i].m_SurfaceFormats.data());
            CHECK_VK_RESULT(res, "Failed to get physical device surface formats!");

            for (uint32_t j = 0; j < formatCount; j++)
            {
                const VkSurfaceFormatKHR& surfaceFormat = PhysicalDevices[i].m_SurfaceFormats[j];
                printf("    Format %x color space %x\n", surfaceFormat.format, surfaceFormat.colorSpace);
            }

            uint32_t surfaceCapabilitiesCount = 0;
            res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[i], vkSurface, &PhysicalDevices[i].m_SurfaceCapabilities);
            CHECK_VK_RESULT(res, "Failed to get physical device surface capabilities!");
            
            PrintImageUsageFlags(PhysicalDevices[i].m_SurfaceCapabilities.supportedUsageFlags);

            uint32_t presentModeCount = 0;
            res = vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], vkSurface, &presentModeCount, nullptr);
            CHECK_VK_RESULT(res, "Failed to get physical device surface present modes!");

            PhysicalDevices[i].m_PresentModes.resize(presentModeCount);

            res = vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], vkSurface, &presentModeCount, PhysicalDevices[i].m_PresentModes.data());
            CHECK_VK_RESULT(res, "Failed to get physical device surface present modes!");

            for (uint32_t j = 0; j < presentModeCount; j++)
            {
                const VkPresentModeKHR& presentMode = PhysicalDevices[i].m_PresentModes[j];
                LOG_TRACE("Present mode {}:\n  Present mode = {}", j, (int)presentMode);
            }

            printf("Num memory types %d\n", PhysicalDevices[i].m_PhysicalDeviceMemoryProperties.memoryTypeCount);
            for (uint32_t j = 0; j < PhysicalDevices[i].m_PhysicalDeviceMemoryProperties.memoryTypeCount; j++)
            {
                const VkMemoryType& memoryType = PhysicalDevices[i].m_PhysicalDeviceMemoryProperties.memoryTypes[j];
                printf("%d: flags %x heap %d ", j, 
                    memoryType.propertyFlags,
                    memoryType.heapIndex);

                PrintMemoryProperty(PhysicalDevices[i].m_PhysicalDeviceMemoryProperties.memoryTypes[j].propertyFlags);

                printf("\n");
            }

            printf("Num heap types %d\n", PhysicalDevices[i].m_PhysicalDeviceMemoryProperties.memoryHeapCount);
            printf("\n");

            vkGetPhysicalDeviceFeatures(devices[i], &PhysicalDevices[i].m_PhysicalDeviceFeatures);
            
            LOG_DEBUG("Initialized physical device: {} (index: {}).", PhysicalDevices[i].m_PhysicalDeviceProperties.deviceName, i);
        }
    }

    uint32_t LOG_Device::SelectPhysicalDevice(VkQueueFlags RequiredQueueType, bool SupportsPresent)
    {
        for (uint32_t i = 0; i < PhysicalDevices.size(); i++)
        {
            const PhysicalDevice& physicalDevice = PhysicalDevices[i];
            const VkPhysicalDeviceProperties& properties = physicalDevice.m_PhysicalDeviceProperties;

            if (physicalDevice.m_QueueFamilyPresentSupport[0] == SupportsPresent)
            {
                for (uint32_t j = 0; j < physicalDevice.m_QueueFamilyProperties.size(); j++)
                {
                    const VkQueueFamilyProperties& queueFamilyProperties = physicalDevice.m_QueueFamilyProperties[j];
                    if ((queueFamilyProperties.queueFlags & RequiredQueueType) == RequiredQueueType)
                    {
                        m_DeviceIndex = i;
                        LOG_DEBUG("Selected physical device: {} (index: {})", properties.deviceName, m_DeviceIndex);
                        return m_DeviceIndex;
                    }
                }
            }
        }
        LOG_ERROR("No suitable physical device found!");
        return -1;
    }

    const PhysicalDevice& LOG_Device::GetDevice() const
    {
        if (m_DeviceIndex == -1)
        {
            LOG_ERROR("No physical device selected!");
            exit(1);
        }
        return PhysicalDevices[m_DeviceIndex];
    }
}