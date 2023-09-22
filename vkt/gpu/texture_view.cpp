#include "vkt/gpu/texture_view.h"
#include "vkt/gpu/texture.h"

namespace vkt
{

TextureView::TextureView(Texture* texture, TextureViewDescriptor descriptor)
    : m_texture(texture)
    , m_descriptor(descriptor)
{
}

TextureViewType TextureView::getType() const
{
    return m_descriptor.type;
}

TextureFormat TextureView::getFormat() const
{
    return m_texture->getFormat();
}

uint32_t TextureView::getWidth() const
{
    return m_texture->getWidth();
}

uint32_t TextureView::getHeight() const
{
    return m_texture->getHeight();
}

uint32_t TextureView::getMipLevels() const
{
    return m_texture->getMipLevels();
}

uint32_t TextureView::getSampleCount() const
{
    return m_texture->getSampleCount();
}

} // namespace vkt
