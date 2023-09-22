#include "vkt/gpu/texture.h"
#include "vkt/gpu/device.h"

namespace vkt
{

Texture::Texture(Device* device, TextureDescriptor descriptor)
    : m_device(device)
    , m_descriptor(descriptor)
{
}

Device* Texture::getDevice() const
{
    return m_device;
}

TextureType Texture::getType() const
{
    return m_descriptor.type;
}

TextureFormat Texture::getFormat() const
{
    return m_descriptor.format;
}

TextureUsageFlags Texture::getUsage() const
{
    return m_descriptor.usage;
}

uint32_t Texture::getWidth() const
{
    return m_descriptor.width;
}
uint32_t Texture::getHeight() const
{
    return m_descriptor.height;
}

uint32_t Texture::getMipLevels() const
{
    return m_descriptor.mipLevels;
}

uint32_t Texture::getSampleCount() const
{
    return m_descriptor.sampleCount;
}

} // namespace vkt
