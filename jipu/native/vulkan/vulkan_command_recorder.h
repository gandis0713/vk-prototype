#pragma once

#include "vulkan_api.h"
#include "vulkan_command.h"
#include "vulkan_command_encoder.h"
#include "vulkan_command_resource_synchronizer.h"
#include "vulkan_export.h"

namespace jipu
{

class VulkanPipeline;
class VulkanCommandBuffer;
class VulkanRenderPipeline;
class VulkanComputePipeline;

struct VulkanCommandRecordResult
{
    std::vector<std::unique_ptr<Command>> commands{};
    ResourceSyncResult resourceSyncResult{};
};

struct VulkanCommandRecorderDescriptor
{
    CommandEncodingResult commandEncodingResult{};
};

class VULKAN_EXPORT VulkanCommandRecorder final
{
public:
    VulkanCommandRecorder() = delete;
    VulkanCommandRecorder(VulkanCommandBuffer* commandBuffer, VulkanCommandRecorderDescriptor descriptor);
    ~VulkanCommandRecorder();

    VulkanCommandRecorder(const VulkanCommandRecorder&) = delete;
    VulkanCommandRecorder& operator=(const VulkanCommandRecorder&) = delete;

    VulkanCommandRecordResult record();

public:
    VulkanCommandBuffer* getCommandBuffer() const;

private:
    void beginRecord();
    VulkanCommandRecordResult endRecord();

    // compute pass
    void beginComputePass(BeginComputePassCommand* command);
    void setComputePipeline(SetComputePipelineCommand* command);
    void setComputeBindGroup(SetBindGroupCommand* command);
    void dispatch(DispatchCommand* command);
    void dispatchIndirect(DispatchIndirectCommand* command);
    void endComputePass(EndComputePassCommand* command);

    // render pass
    void beginRenderPass(BeginRenderPassCommand* command);
    void setRenderPipeline(SetRenderPipelineCommand* command);
    void setRenderBindGroup(SetBindGroupCommand* command);
    void setVertexBuffer(SetVertexBufferCommand* command);
    void setIndexBuffer(SetIndexBufferCommand* command);
    void setViewport(SetViewportCommand* command);
    void setScissor(SetScissorCommand* command);
    void setBlendConstant(SetBlendConstantCommand* command);
    void draw(DrawCommand* command);
    void drawIndexed(DrawIndexedCommand* command);
    void beginOcclusionQuery(BeginOcclusionQueryCommand* command);
    void endOcclusionQuery(EndOcclusionQueryCommand* command);
    void executeBundle(ExecuteBundleCommand* command);
    void endRenderPass(EndRenderPassCommand* command);

    // copy
    void copyBufferToBuffer(CopyBufferToBufferCommand* command);
    void copyBufferToTexture(CopyBufferToTextureCommand* command);
    void copyTextureToBuffer(CopyTextureToBufferCommand* command);
    void copyTextureToTexture(CopyTextureToTextureCommand* command);

    // query
    void resolveQuerySet(ResolveQuerySetCommand* command);

private:
    VulkanCommandBuffer* m_commandBuffer = nullptr;
    VulkanCommandRecorderDescriptor m_descriptor{};
    VulkanCommandResourceSynchronizer m_commandResourceSyncronizer{};

private:
    VulkanRenderPipeline* m_renderPipeline = nullptr;
    VulkanComputePipeline* m_computePipeline = nullptr;

private:
    // Render Pass Information
    std::shared_ptr<VulkanRenderPass> m_renderPass{};
    std::shared_ptr<VulkanFramebuffer> m_framebuffer{};

    bool m_isUseSecondaryBuffer = false;
};

// Generator
VkPipelineStageFlags generatePipelineStageFlags(Command* cmd);
VkAccessFlags generateBufferAccessFlags(BufferUsageFlags usage);
VkAccessFlags generateTextureAccessFlags(TextureUsageFlags usage);

} // namespace jipu