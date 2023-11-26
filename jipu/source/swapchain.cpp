#include "jipu/swapchain.h"

#include "jipu/device.h"
#include "jipu/queue.h"
#include "jipu/surface.h"

namespace jipu
{

Swapchain::Swapchain(Device* device, const SwapchainDescriptor& descriptor) noexcept
    : m_device(device)
    , m_surface(descriptor.surface)
    , m_textureFormat(descriptor.textureFormat)
    , m_presentMode(descriptor.presentMode)
    , m_colorSpace(descriptor.colorSpace)
    , m_width(descriptor.width)
    , m_height(descriptor.height)
{
}

std::vector<Texture*> Swapchain::getTextures() const
{
    std::vector<Texture*> textures{};

    for (auto& texture : m_textures)
    {
        textures.push_back(texture.get());
    }

    return textures;
}

TextureView* Swapchain::getTextureView(uint32_t index)
{
    if (index >= m_textureViews.size())
        return nullptr;

    return m_textureViews[index].get();
}

std::vector<TextureView*> Swapchain::getTextureViews() const
{
    std::vector<TextureView*> textureViews{};

    for (auto& textureView : m_textureViews)
    {
        textureViews.push_back(textureView.get());
    }

    return textureViews;
}

TextureFormat Swapchain::getTextureFormat() const
{
    return m_textureFormat;
}

PresentMode Swapchain::getPresentMode() const
{
    return m_presentMode;
}

uint32_t Swapchain::getWidth() const
{
    return m_width;
}

uint32_t Swapchain::getHeight() const
{
    return m_height;
}

} // namespace jipu
