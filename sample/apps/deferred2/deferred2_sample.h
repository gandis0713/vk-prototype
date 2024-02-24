#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "file.h"
#include "im_gui.h"
#include "light.h"
#include "model.h"
#include "sample.h"

#include "jipu/buffer.h"
#include "jipu/command_buffer.h"
#include "jipu/command_encoder.h"
#include "jipu/device.h"
#include "jipu/driver.h"
#include "jipu/physical_device.h"
#include "jipu/pipeline_layout.h"
#include "jipu/queue.h"
#include "jipu/render_pass_encoder.h"
#include "jipu/surface.h"
#include "jipu/swapchain.h"

#include "vulkan_pipeline_group.h"

#include "khronos_texture.h"

namespace jipu
{

class Deferred2Sample : public Sample, public Im_Gui
{
public:
    Deferred2Sample() = delete;
    Deferred2Sample(const SampleDescriptor& descriptor);
    ~Deferred2Sample() override;

public:
    void init() override;
    void update() override;
    void draw() override;

private:
    void updateImGui() override;

private:
    void createDriver();
    void getPhysicalDevices();
    void createSurface();
    void createDevice();
    void createSwapchain();

    void createOffscreenPositionColorAttachmentTexture();
    void createOffscreenPositionColorAttachmentTextureView();
    void createOffscreenNormalColorAttachmentTexture();
    void createOffscreenNormalColorAttachmentTextureView();
    void createOffscreenAlbedoColorAttachmentTexture();
    void createOffscreenAlbedoColorAttachmentTextureView();
    void createOffscreenColorMapTexture();
    void createOffscreenColorMapTextureView();
    void createOffscreenNormalMapTexture();
    void createOffscreenNormalMapTextureView();
    void createOffscreenCamera();
    void createOffscreenUniformBuffer();
    void createOffscreenVertexBuffer();
    void createOffscreenBindingGroupLayout();
    void createOffscreenBindingGroup();
    void createOffscreenPipelineLayout();
    RenderPipelineDescriptor createOffscreenRenderPipelineDescriptor();
    void updateOffscreenUniformBuffer();

    void createCompositionBindingGroupLayout();
    void createCompositionBindingGroup();
    void createCompositionPipelineLayout();
    RenderPipelineDescriptor createCompositionRenderPipelineDescriptor();
    void createCompositionUniformBuffer();
    void createCompositionVertexBuffer();
    void updateCompositionUniformBuffer();

    void createDepthStencilTexture();
    void createDepthStencilTextureView();
    void createRenderPipelineGroup();

    void createCommandBuffer();
    void createQueue();

private:
    std::unique_ptr<Driver> m_driver = nullptr;
    std::vector<std::unique_ptr<PhysicalDevice>> m_physicalDevices{};
    std::unique_ptr<Surface> m_surface = nullptr;
    std::unique_ptr<Device> m_device = nullptr;
    std::unique_ptr<Swapchain> m_swapchain = nullptr;

    struct CompositionUBO
    {
        struct Light
        {
            alignas(16) glm::vec3 position;
            alignas(16) glm::vec3 color;
        };

        std::vector<CompositionUBO::Light> lights{};
        alignas(16) glm::vec3 cameraPosition;
        int lightCount = 8;
        int padding1;
        int padding2;
        int padding3;
    };

    struct MVP
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } m_mvp;

    struct
    {
        std::unique_ptr<Texture> positionColorAttachmentTexture = nullptr;
        std::unique_ptr<TextureView> positionColorAttachmentTextureView = nullptr;
        std::unique_ptr<Texture> normalColorAttachmentTexture = nullptr;
        std::unique_ptr<TextureView> normalColorAttachmentTextureView = nullptr;
        std::unique_ptr<Texture> albedoColorAttachmentTexture = nullptr;
        std::unique_ptr<TextureView> albedoColorAttachmentTextureView = nullptr;
        std::unique_ptr<Texture> colorMapTexture = nullptr;
        std::unique_ptr<TextureView> colorMapTextureView = nullptr;
        std::unique_ptr<Texture> normalMapTexture = nullptr;
        std::unique_ptr<TextureView> normalMapTextureView = nullptr;
        std::unique_ptr<Sampler> colorMapSampler = nullptr;
        std::unique_ptr<Sampler> normalMapSampler = nullptr;
        std::unique_ptr<Buffer> uniformBuffer = nullptr;
        std::unique_ptr<Buffer> vertexBuffer = nullptr;
        std::unique_ptr<Buffer> indexBuffer = nullptr;
        std::vector<std::unique_ptr<BindingGroupLayout>> bindingGroupLayouts{};
        std::vector<std::unique_ptr<BindingGroup>> bindingGroups{};
        std::unique_ptr<ShaderModule> vertexShaderModule = nullptr;
        std::unique_ptr<ShaderModule> fragmentShaderModule = nullptr;
        std::unique_ptr<PipelineLayout> pipelineLayout = nullptr;
        std::unique_ptr<Camera> camera = nullptr;
        Polygon polygon{};
    } m_offscreen;

    struct CompositionVertex
    {
        glm::vec3 position;
        glm::vec2 textureCoordinate;
    };
    struct
    {
        std::unique_ptr<BindingGroupLayout> bindingGroupLayout = nullptr;
        std::unique_ptr<BindingGroup> bindingGroup = nullptr;
        std::unique_ptr<Sampler> positionSampler = nullptr;
        std::unique_ptr<Sampler> normalSampler = nullptr;
        std::unique_ptr<Sampler> albedoSampler = nullptr;
        std::unique_ptr<ShaderModule> vertexShaderModule = nullptr;
        std::unique_ptr<ShaderModule> fragmentShaderModule = nullptr;
        std::unique_ptr<PipelineLayout> pipelineLayout = nullptr;
        std::unique_ptr<Buffer> uniformBuffer = nullptr;
        std::unique_ptr<Buffer> vertexBuffer = nullptr;
        CompositionUBO ubo{};
        std::vector<CompositionVertex> vertices{
            { { -1.0, -1.0, 0.0 }, { 0.0, 0.0 } },
            { { -1.0, 1.0, 0.0 }, { 0.0, 1.0 } },
            { { 1.0, -1.0, 0.0 }, { 1.0, 0.0 } },
            { { 1.0, -1.0, 0.0 }, { 1.0, 0.0 } },
            { { -1.0, 1.0, 0.0 }, { 0.0, 1.0 } },
            { { 1.0, 1.0, 0.0 }, { 1.0, 1.0 } },
        };
    } m_composition;

    std::unique_ptr<CommandBuffer> m_commandBuffer = nullptr;
    std::unique_ptr<Queue> m_queue = nullptr;
    std::unique_ptr<VulkanRenderPipelineGroup> m_renderPipelineGroup = nullptr;
    std::unique_ptr<Texture> m_depthStencilTexture = nullptr;
    std::unique_ptr<TextureView> m_depthStencilTextureView = nullptr;

    uint32_t m_sampleCount = 1;
    int m_lightMax = 10000;
};

} // namespace jipu