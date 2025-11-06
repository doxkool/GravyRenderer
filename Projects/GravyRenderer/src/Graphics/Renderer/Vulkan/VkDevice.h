#pragma once

#include "Graphics\Renderer\Vulkan\VkLogger.h"
#include "Graphics\Renderer\Vulkan\VkUtils.h"

#include <vulkan/vulkan.h>

namespace VkCore
{
    struct PhysicalDevice
    {
        VkPhysicalDevice m_PhysicalDevice;
        VkPhysicalDeviceProperties m_PhysicalDeviceProperties;
        std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
        std::vector<VkBool32> m_QueueFamilyPresentSupport;
        std::vector<VkSurfaceFormatKHR> m_SurfaceFormats;
        VkSurfaceCapabilitiesKHR m_SurfaceCapabilities;
        VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;
        std::vector<VkPresentModeKHR> m_PresentModes;
        VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures;
        VkFormat m_DepthFormat;
    };

    class VkCore_Device
    {
    private:
        std::vector<PhysicalDevice> PhysicalDevices;
        int m_DeviceIndex = -1;
    public:
        VkCore_Device() {}
        ~VkCore_Device() {}

        void Init(const VkInstance& vkInstance, const VkSurfaceKHR& vkSurface);

        uint32_t SelectPhysicalDevice(VkQueueFlags RequiredQueueType, bool SupportsPresent);

        const PhysicalDevice& GetDevice() const;
    };     
}