#pragma once

#include "utils/cast.h"
#include "vkt/gpu/texture_view.h"
#include "vulkan_api.h"

namespace vkt
{

class VulkanTexture;
class VKT_EXPORT VulkanTextureView : public TextureView
{
public:
    VulkanTextureView() = delete;
    VulkanTextureView(VulkanTexture* texture, TextureViewDescriptor descriptor);
    ~VulkanTextureView() override;

    VkImageView getImageView() const;

private:
    VkImageView m_imageView = VK_NULL_HANDLE;
};

DOWN_CAST(VulkanTextureView, TextureView);

} // namespace vkt
