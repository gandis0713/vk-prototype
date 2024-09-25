#include "vulkan_resource_tracker.h"

#include "vulkan_binding_group.h"
#include "vulkan_binding_group_layout.h"
#include "vulkan_buffer.h"
#include "vulkan_command.h"
#include "vulkan_texture.h"

namespace jipu
{

void VulkanResourceTracker::beginComputePass(BeginComputePassCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::setComputePipeline(SetComputePipelineCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::setComputeBindingGroup(SetBindGroupCommand* command)
{
    // consumer
    if (false) // TODO
    {
        auto bufferBindings = command->bindingGroup->getBufferBindings();
        for (auto& bufferBinding : bufferBindings)
        {
            m_ongoingPassResourceInfo.consumer.buffers[bufferBinding.buffer] = BufferUsageInfo{
                .stageFlags = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                .accessFlags = VK_ACCESS_SHADER_READ_BIT,
            };
        }

        auto textureBindings = command->bindingGroup->getTextureBindings();
        for (auto& textureBinding : textureBindings)
        {
            m_ongoingPassResourceInfo.consumer.textures[textureBinding.textureView->getTexture()] = TextureUsageInfo{
                .stageFlags = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                .accessFlags = VK_ACCESS_SHADER_READ_BIT,
                .layout = VK_IMAGE_LAYOUT_GENERAL,
            };
        }
    }

    // producer
    {
        auto bufferBindings = command->bindingGroup->getBufferBindings();
        for (auto& bufferBinding : bufferBindings)
        {
            m_ongoingPassResourceInfo.producer.buffers[bufferBinding.buffer] = BufferUsageInfo{
                .stageFlags = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                .accessFlags = VK_ACCESS_SHADER_WRITE_BIT,
            };
        }

        auto textureBindings = command->bindingGroup->getTextureBindings();
        for (auto& textureBinding : textureBindings)
        {
            m_ongoingPassResourceInfo.producer.textures[textureBinding.textureView->getTexture()] = TextureUsageInfo{
                .stageFlags = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                .accessFlags = VK_ACCESS_SHADER_WRITE_BIT,
                .layout = VK_IMAGE_LAYOUT_GENERAL,
            };
        }
    }
}

void VulkanResourceTracker::dispatch(DispatchCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::dispatchIndirect(DispatchIndirectCommand* command)
{
    // TODO
}

void VulkanResourceTracker::endComputePass(EndComputePassCommand* command)
{
    m_passResourceInfos.push_back(std::move(m_ongoingPassResourceInfo));
    m_ongoingPassResourceInfo.clear();
}

void VulkanResourceTracker::beginRenderPass(BeginRenderPassCommand* command)
{
    // TODO
}

void VulkanResourceTracker::setRenderPipeline(SetRenderPipelineCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::setVertexBuffer(SetVertexBufferCommand* command)
{
    // consumer
    {
        m_ongoingPassResourceInfo.consumer.buffers[command->buffer] = BufferUsageInfo{
            .stageFlags = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
            .accessFlags = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
        };
    }
}

void VulkanResourceTracker::setIndexBuffer(SetIndexBufferCommand* command)
{
    // consumer
    {
        m_ongoingPassResourceInfo.consumer.buffers[command->buffer] = BufferUsageInfo{
            .stageFlags = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
            .accessFlags = VK_ACCESS_INDEX_READ_BIT,
        };
    }
}

void VulkanResourceTracker::setViewport(SetViewportCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::setScissor(SetScissorCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::setBlendConstant(SetBlendConstantCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::draw(DrawCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::drawIndexed(DrawIndexedCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::beginOcclusionQuery(BeginOcclusionQueryCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::endOcclusionQuery(EndOcclusionQueryCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::endRenderPass(EndRenderPassCommand* command)
{
    m_passResourceInfos.push_back(std::move(m_ongoingPassResourceInfo));
    m_ongoingPassResourceInfo.clear();
}

void VulkanResourceTracker::setRenderBindingGroup(SetBindGroupCommand* command)
{
    // consumer
    {
        auto bindingGroup = command->bindingGroup;
        auto bindingGroupLayout = command->bindingGroup->getLayout();

        auto bufferBindings = bindingGroup->getBufferBindings();
        auto bufferBindingLayouts = bindingGroupLayout->getBufferBindingLayouts();
        for (auto i = 0; i < bufferBindings.size(); ++i)
        {
            auto& bufferBinding = bufferBindings[i];
            auto& bufferBindingLayout = bufferBindingLayouts[i];

            auto bufferUsageInfo = BufferUsageInfo{ .stageFlags = VK_PIPELINE_STAGE_NONE,
                                                    .accessFlags = VK_ACCESS_NONE };

            if (bufferBindingLayout.stages & BindingStageFlagBits::kComputeStage)
            {
                bufferUsageInfo.stageFlags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            }
            if (bufferBindingLayout.stages & BindingStageFlagBits::kVertexStage)
            {
                bufferUsageInfo.stageFlags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            }
            if (bufferBindingLayout.stages & BindingStageFlagBits::kFragmentStage)
            {
                bufferUsageInfo.stageFlags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }

            switch (bufferBindingLayout.type)
            {
            case BufferBindingType::kUniform:
                bufferUsageInfo.accessFlags |= VK_ACCESS_UNIFORM_READ_BIT;
                break;
            case BufferBindingType::kStorage:
                bufferUsageInfo.accessFlags |= VK_ACCESS_SHADER_WRITE_BIT;
                break;
            case BufferBindingType::kReadOnlyStorage:
                bufferUsageInfo.accessFlags |= VK_ACCESS_SHADER_READ_BIT;
                break;
            default:
                bufferUsageInfo.accessFlags |= VK_ACCESS_SHADER_READ_BIT;
                break;
            }
        }

        auto textureBindings = bindingGroup->getTextureBindings();
        auto textureBindingLayouts = bindingGroupLayout->getTextureBindingLayouts();
        for (auto i = 0; i < textureBindings.size(); ++i)
        {
            auto& textureBinding = textureBindings[i];
            auto& textureBindingLayout = textureBindingLayouts[i];

            auto textureUsageInfo = TextureUsageInfo{ .stageFlags = VK_PIPELINE_STAGE_NONE,
                                                      .accessFlags = VK_ACCESS_NONE,
                                                      .layout = VK_IMAGE_LAYOUT_UNDEFINED };

            if (textureBindingLayout.stages & BindingStageFlagBits::kComputeStage)
            {
                textureUsageInfo.stageFlags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            }
            if (textureBindingLayout.stages & BindingStageFlagBits::kVertexStage)
            {
                textureUsageInfo.stageFlags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            }
            if (textureBindingLayout.stages & BindingStageFlagBits::kFragmentStage)
            {
                textureUsageInfo.stageFlags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }
            textureUsageInfo.accessFlags |= VK_ACCESS_SHADER_READ_BIT;
            textureUsageInfo.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
    }

    // producer
    {
        // TODO
    }
}

void VulkanResourceTracker::copyBufferToBuffer(CopyBufferToBufferCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::copyBufferToTexture(CopyBufferToTextureCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::copyTextureToBuffer(CopyTextureToBufferCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::copyTextureToTexture(CopyTextureToTextureCommand* command)
{
    // do nothing.
}

void VulkanResourceTracker::resolveQuerySet(ResolveQuerySetCommand* command)
{
    // do nothing.
}

std::vector<PassResourceInfo> VulkanResourceTracker::extractPassResourceInfos()
{
    auto movedPassResourceInfos = std::move(m_passResourceInfos);
    m_passResourceInfos.clear();

    return movedPassResourceInfos;
}

} // namespace jipu