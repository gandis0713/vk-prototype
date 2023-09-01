#include "vulkan_queue.h"
#include "vulkan_device.h"
#include "vulkan_physical_device.h"
#include "vulkan_swapchain.h"

#include <spdlog/spdlog.h>
#include <stdexcept>

namespace vkt
{

VulkanQueue::VulkanQueue(VulkanDevice* device, const QueueDescriptor& descriptor) noexcept(false)
    : Queue(device, descriptor)
{
    VulkanPhysicalDevice* physicalDevice = downcast(m_device->getPhysicalDevice());

    const VulkanPhysicalDeviceInfo& deviceInfo = physicalDevice->getInfo();

    const uint64_t queueFamilyPropertiesSize = deviceInfo.queueFamilyProperties.size();
    if (queueFamilyPropertiesSize <= 0)
    {
        throw std::runtime_error("There is no queue family properties.");
    }

    for (uint64_t index = 0; index < queueFamilyPropertiesSize; ++index)
    {
        const auto& properties = deviceInfo.queueFamilyProperties[index];
        if (properties.queueFlags & ToVkQueueFlags(descriptor.flags))
        {
            m_index = static_cast<uint32_t>(index);
            m_properties = properties;
            break;
        }
    }

    device->vkAPI.GetDeviceQueue(device->getVkDevice(), m_index, 0, &m_queue);

    // create fence.
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = 0;
    // fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (device->vkAPI.CreateFence(device->getVkDevice(), &fenceCreateInfo, nullptr, &m_fence) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render queue fence.");
    }

    // create semaphore
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    if (device->vkAPI.CreateSemaphore(device->getVkDevice(), &semaphoreCreateInfo, nullptr, &m_renderSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render queue semephore.");
    }

    if (device->vkAPI.CreateSemaphore(device->getVkDevice(), &semaphoreCreateInfo, nullptr, &m_computeSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create compute queue semephore.");
    }
}

VulkanQueue::~VulkanQueue()
{
    auto vulkanDevice = downcast(m_device);
    const VulkanAPI& vkAPI = vulkanDevice->vkAPI;

    // wait idle state before destroy semaphore.
    vkAPI.QueueWaitIdle(m_queue);

    // Doesn't need to destroy VkQueue.
    vkAPI.DestroySemaphore(vulkanDevice->getVkDevice(), m_renderSemaphore, nullptr);
    vkAPI.DestroySemaphore(vulkanDevice->getVkDevice(), m_computeSemaphore, nullptr);
    vkAPI.DestroyFence(vulkanDevice->getVkDevice(), m_fence, nullptr);
}

void VulkanQueue::submit(std::vector<CommandBuffer*> commandBuffers)
{
    auto vulkanDevice = downcast(m_device);
    const VulkanAPI& vkAPI = vulkanDevice->vkAPI;

    // submit command buffer to a queue
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_computeSemaphore;

    std::vector<VkCommandBuffer> vulkanCommandBuffers{};
    vulkanCommandBuffers.resize(submitInfo.commandBufferCount);
    for (auto i = 0; i < vulkanCommandBuffers.size(); ++i)
    {
        vulkanCommandBuffers[i] = downcast(commandBuffers[i])->getVkCommandBuffer();
    }
    submitInfo.pCommandBuffers = vulkanCommandBuffers.data();

    if (vkAPI.QueueSubmit(m_queue, 1, &submitInfo, m_fence) != VK_SUCCESS)
    {
        spdlog::error("failed to submit command buffer.");
    }

    vkAPI.WaitForFences(vulkanDevice->getVkDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX);
    vkAPI.ResetFences(vulkanDevice->getVkDevice(), 1, &m_fence);
}

void VulkanQueue::submit(std::vector<CommandBuffer*> commandBuffers, Swapchain* swapchain)
{
    auto vulkanDevice = downcast(m_device);
    const VulkanAPI& vkAPI = vulkanDevice->vkAPI;

    std::vector<VkSemaphore> waitSemaphores{ m_computeSemaphore, downcast(swapchain)->getAcquireImageSemaphore() };
    downcast(swapchain)->injectSemaphore(m_renderSemaphore);

    // submit command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitPipelineStages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitPipelineStages;

    uint32_t commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    submitInfo.commandBufferCount = commandBufferCount;
    std::vector<VkCommandBuffer> vulkanCommandBuffers{};
    vulkanCommandBuffers.resize(commandBufferCount);
    for (auto i = 0; i < commandBufferCount; ++i)
    {
        vulkanCommandBuffers[i] = downcast(commandBuffers[i])->getVkCommandBuffer();
    }
    submitInfo.pCommandBuffers = vulkanCommandBuffers.data();

    VkSemaphore signalSemaphores[] = { m_renderSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkAPI.QueueSubmit(m_queue, 1, &submitInfo, m_fence) != VK_SUCCESS)
    {
        spdlog::error("failed to submit draw command buffer!");
    }

    vkAPI.WaitForFences(vulkanDevice->getVkDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX);
    vkAPI.ResetFences(vulkanDevice->getVkDevice(), 1, &m_fence);

    swapchain->present(this);
}

VkQueue VulkanQueue::getVkQueue() const
{
    return m_queue;
}

// Convert Helper
VkQueueFlags ToVkQueueFlags(QueueFlags flags)
{
    VkQueueFlags vkflags = 0u;

    if (flags & QueueFlagBits::kGraphics)
    {
        vkflags |= VK_QUEUE_GRAPHICS_BIT;
    }
    if (flags & QueueFlagBits::kCompute)
    {
        vkflags |= VK_QUEUE_COMPUTE_BIT;
    }
    if (flags & QueueFlagBits::kTransfer)
    {
        vkflags |= VK_QUEUE_TRANSFER_BIT;
    }

    return vkflags;
}

QueueFlags ToQueueFlags(VkQueueFlags vkflags)
{
    QueueFlags flags = 0u;

    if (vkflags & VK_QUEUE_GRAPHICS_BIT)
    {
        flags |= QueueFlagBits::kGraphics;
    }
    if (vkflags & VK_QUEUE_COMPUTE_BIT)
    {
        flags |= QueueFlagBits::kCompute;
    }
    if (vkflags & VK_QUEUE_TRANSFER_BIT)
    {
        flags |= QueueFlagBits::kTransfer;
    }

    return flags;
}

} // namespace vkt
