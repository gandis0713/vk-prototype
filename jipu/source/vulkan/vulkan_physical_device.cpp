#include "vulkan_physical_device.h"
#include "vulkan_device.h"
#include "vulkan_driver.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace jipu
{

VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanDriver* vulkanDriver, const VulkanPhysicalDeviceDescriptor& descriptor)
    : PhysicalDevice(vulkanDriver)
{
    m_physicalDevice = descriptor.physicalDevice;

    // Gather device information.
    gatherPhysicalDeviceInfo();
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
    // doesn't need to destroy VkPhysicalDevice.
}

std::unique_ptr<Device> VulkanPhysicalDevice::createDevice(DeviceDescriptor descriptor)
{
    return std::make_unique<VulkanDevice>(this, descriptor);
}

PhysicalDeviceInfo VulkanPhysicalDevice::getInfo() const
{
    PhysicalDeviceInfo info{};
    info.deviceName = m_info.physicalDeviceProperties.deviceName;
    return info;
}

VkInstance VulkanPhysicalDevice::getVkInstance() const
{
    return downcast(m_driver)->getVkInstance();
}

VkPhysicalDevice VulkanPhysicalDevice::getVkPhysicalDevice() const
{
    return m_physicalDevice;
}

const VulkanPhysicalDeviceInfo& VulkanPhysicalDevice::getVulkanPhysicalDeviceInfo() const
{
    return m_info;
}

void VulkanPhysicalDevice::gatherPhysicalDeviceInfo()
{
    const VulkanAPI& vkAPI = downcast(m_driver)->vkAPI;

    // Gather physical device properties and features.
    vkAPI.GetPhysicalDeviceProperties(m_physicalDevice, &m_info.physicalDeviceProperties);
    m_info.apiVersion = m_info.physicalDeviceProperties.apiVersion;

    spdlog::info("Vulkan Device API Version: {}.{}.{}",
                 VK_API_VERSION_MAJOR(m_info.apiVersion),
                 VK_API_VERSION_MINOR(m_info.apiVersion),
                 VK_API_VERSION_PATCH(m_info.apiVersion));

    vkAPI.GetPhysicalDeviceFeatures(m_physicalDevice, &m_info.physicalDeviceFeatures);

    // Gather device memory properties.
    {
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        vkAPI.GetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryProperties);

        m_info.memoryTypes.assign(memoryProperties.memoryTypes, memoryProperties.memoryTypes + memoryProperties.memoryTypeCount);
        m_info.memoryHeaps.assign(memoryProperties.memoryHeaps, memoryProperties.memoryHeaps + memoryProperties.memoryHeapCount);
    }

    // Gather queue Family Properties.
    {
        uint32_t queueFamilyCount = 0;
        vkAPI.GetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

        m_info.queueFamilyProperties.resize(queueFamilyCount);
        vkAPI.GetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, m_info.queueFamilyProperties.data());
    }

    // Gather device layer properties.
    {
        uint32_t deviceLayerCount = 0;
        VkResult result = vkAPI.EnumerateDeviceLayerProperties(m_physicalDevice, &deviceLayerCount, nullptr);
        if (result != VK_SUCCESS && result != VK_INCOMPLETE)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceLayerProperties to get count. Error: {}", static_cast<int32_t>(result)));
        }

        m_info.layerProperties.resize(deviceLayerCount);
        result = vkAPI.EnumerateDeviceLayerProperties(m_physicalDevice, &deviceLayerCount, m_info.layerProperties.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceLayerProperties. Error: {}", static_cast<int32_t>(result)));
        }

        for (const auto& layerProperty : m_info.layerProperties)
        {
            spdlog::info("Device Layer Name: {}", layerProperty.layerName);
        }
    }

    // Gather device extension properties.
    {
        uint32_t deviceExtensionCount = 0;
        VkResult result = vkAPI.EnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &deviceExtensionCount, nullptr);
        if (result != VK_SUCCESS && result != VK_INCOMPLETE)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceExtensionProperties to get count. Error: {}", static_cast<int32_t>(result)));
        }

        m_info.extensionProperties.resize(deviceExtensionCount);
        result = vkAPI.EnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &deviceExtensionCount, m_info.extensionProperties.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error(fmt::format("Failure EnumerateDeviceExtensionProperties. Error: {}", static_cast<int32_t>(result)));
        }

        for (const auto& extensionProperty : m_info.extensionProperties)
        {
            spdlog::info("Device Extention Name: {}", extensionProperty.extensionName);

            // TODO: define "VK_KHR_portability_subset"
            if (strncmp(extensionProperty.extensionName, "VK_KHR_portability_subset", VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.portabilitySubset = true;
            }

            if (strncmp(extensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                m_info.swapchain = true;
            }
        }
    }
}

int VulkanPhysicalDevice::findMemoryTypeIndex(VkMemoryPropertyFlags flags) const
{
    int memoryTypeIndex = -1;
    for (int i = 0u; i < m_info.memoryTypes.size(); ++i)
    {
        const auto& memoryType = m_info.memoryTypes[i];
        if ((memoryType.propertyFlags & flags) == flags)
        {
            memoryTypeIndex = i;
            break;
        }
    }

    return memoryTypeIndex;
}

bool VulkanPhysicalDevice::isDepthStencilSupported(VkFormat format) const
{
    const VulkanAPI& vkAPI = downcast(m_driver)->vkAPI;

    VkFormatProperties formatProperties{};
    vkAPI.GetPhysicalDeviceFormatProperties(m_physicalDevice, format, &formatProperties);

    return formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
}

} // namespace jipu
