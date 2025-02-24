#include "jipu/webgpu/webgpu_header.h"

#include "webgpu/webgpu_adapter.h"
#include "webgpu/webgpu_bind_group.h"
#include "webgpu/webgpu_bind_group_layout.h"
#include "webgpu/webgpu_buffer.h"
#include "webgpu/webgpu_command_buffer.h"
#include "webgpu/webgpu_command_encoder.h"
#include "webgpu/webgpu_compute_pass_encoder.h"
#include "webgpu/webgpu_compute_pipeline.h"
#include "webgpu/webgpu_device.h"
#include "webgpu/webgpu_instance.h"
#include "webgpu/webgpu_pipeline_layout.h"
#include "webgpu/webgpu_queue.h"
#include "webgpu/webgpu_render_bundle.h"
#include "webgpu/webgpu_render_bundle_encoder.h"
#include "webgpu/webgpu_render_pass_encoder.h"
#include "webgpu/webgpu_render_pipeline.h"
#include "webgpu/webgpu_sampler.h"
#include "webgpu/webgpu_shader_module.h"
#include "webgpu/webgpu_surface.h"
#include "webgpu/webgpu_texture.h"
#include "webgpu/webgpu_texture_view.h"

#include <unordered_map>

namespace jipu
{

WGPUInstance procCreateInstance(WGPUInstanceDescriptor const* wgpuDescriptor)
{
    return reinterpret_cast<WGPUInstance>(WebGPUInstance::create(wgpuDescriptor));
}

WGPUFuture procInstanceRequestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options, WGPURequestAdapterCallbackInfo2 callbackInfo)
{
    WebGPUInstance* webgpuInstance = reinterpret_cast<WebGPUInstance*>(instance);
    return webgpuInstance->requestAdapter(options, callbackInfo);
}

WGPUSurface procInstanceCreateSurface(WGPUInstance instance, WGPUSurfaceDescriptor const* descriptor)
{
    WebGPUInstance* webgpuInstance = reinterpret_cast<WebGPUInstance*>(instance);
    return reinterpret_cast<WGPUSurface>(webgpuInstance->createSurface(descriptor));
}

WGPUFuture procAdapterRequestDevice(WGPUAdapter adapter, WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor, WGPURequestDeviceCallbackInfo2 callbackInfo)
{
    WebGPUAdapter* webgpuAdapter = reinterpret_cast<WebGPUAdapter*>(adapter);
    return webgpuAdapter->requestDevice(descriptor, callbackInfo);
}

WGPUQueue procDeviceGetQueue(WGPUDevice device)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUQueue>(webgpuDevice->getQueue());
}

WGPUStatus procSurfaceGetCapabilities(WGPUSurface surface, WGPUAdapter adapter, WGPUSurfaceCapabilities* capabilities)
{
    WebGPUSurface* webgpuSurface = reinterpret_cast<WebGPUSurface*>(surface);
    WebGPUAdapter* webgpuAdapter = reinterpret_cast<WebGPUAdapter*>(adapter);
    return webgpuSurface->getCapabilities(webgpuAdapter, capabilities);
}

void procSurfaceConfigure(WGPUSurface surface, WGPUSurfaceConfiguration const* config)
{
    WebGPUSurface* webgpuSurface = reinterpret_cast<WebGPUSurface*>(surface);
    return webgpuSurface->configure(config);
}

WGPUBindGroup procDeviceCreateBindGroup(WGPUDevice device, WGPUBindGroupDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUBindGroup>(webgpuDevice->createBindGroup(descriptor));
}

WGPUBindGroupLayout procDeviceCreateBindGroupLayout(WGPUDevice device, WGPUBindGroupLayoutDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUBindGroupLayout>(webgpuDevice->createBindGroupLayout(descriptor));
}

WGPUPipelineLayout procDeviceCreatePipelineLayout(WGPUDevice device, WGPUPipelineLayoutDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUPipelineLayout>(webgpuDevice->createPipelineLayout(descriptor));
}

WGPURenderPipeline procDeviceCreateRenderPipeline(WGPUDevice device, WGPURenderPipelineDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPURenderPipeline>(webgpuDevice->createRenderPipeline(descriptor));
}

WGPUShaderModule procDeviceCreateShaderModule(WGPUDevice device, WGPUShaderModuleDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUShaderModule>(webgpuDevice->createShaderModule(descriptor));
}

void procSurfaceGetCurrentTexture(WGPUSurface surface, WGPUSurfaceTexture* surfaceTexture)
{
    WebGPUSurface* webgpuSurface = reinterpret_cast<WebGPUSurface*>(surface);
    return webgpuSurface->getCurrentTexture(surfaceTexture);
}

WGPUTextureView procTextureCreateView(WGPUTexture texture, WGPU_NULLABLE WGPUTextureViewDescriptor const* descriptor)
{
    WebGPUTexture* webgpuTexture = reinterpret_cast<WebGPUTexture*>(texture);
    return reinterpret_cast<WGPUTextureView>(webgpuTexture->createView(descriptor));
}

WGPUCommandEncoder procDeviceCreateCommandEncoder(WGPUDevice device, WGPU_NULLABLE WGPUCommandEncoderDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUCommandEncoder>(webgpuDevice->createCommandEncoder(descriptor));
}

WGPURenderPassEncoder procCommandEncoderBeginRenderPass(WGPUCommandEncoder commandEncoder, WGPURenderPassDescriptor const* descriptor)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return reinterpret_cast<WGPURenderPassEncoder>(webgpuCommandEncoder->beginRenderPass(descriptor));
}

void procRenderPassEncoderSetPipeline(WGPURenderPassEncoder renderPassEncoder, WGPURenderPipeline pipeline)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->setPipeline(reinterpret_cast<WebGPURenderPipeline*>(pipeline));
}

void procRenderPassEncoderDraw(WGPURenderPassEncoder renderPassEncoder, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void procRenderPassEncoderEnd(WGPURenderPassEncoder renderPassEncoder)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->end();
}

void procRenderPassEncoderRelease(WGPURenderPassEncoder renderPassEncoder)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->release();
}

WGPUCommandBuffer procCommandEncoderFinish(WGPUCommandEncoder commandEncoder, WGPU_NULLABLE WGPUCommandBufferDescriptor const* descriptor)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return reinterpret_cast<WGPUCommandBuffer>(webgpuCommandEncoder->finish(descriptor));
}

void procQueueSubmit(WGPUQueue queue, size_t commandCount, WGPUCommandBuffer const* commands)
{
    WebGPUQueue* webgpuQueue = reinterpret_cast<WebGPUQueue*>(queue);
    return webgpuQueue->submit(commandCount, commands);
}

void procSurfacePresent(WGPUSurface surface)
{
    WebGPUSurface* webgpuSurface = reinterpret_cast<WebGPUSurface*>(surface);
    return webgpuSurface->present();
}

void procCommandBufferRelease(WGPUCommandBuffer commandBuffer)
{
    WebGPUCommandBuffer* webgpuCommandBuffer = reinterpret_cast<WebGPUCommandBuffer*>(commandBuffer);
    return webgpuCommandBuffer->release();
}

void procCommandEncoderRelease(WGPUCommandEncoder commandEncoder)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return webgpuCommandEncoder->release();
}

void procTextureViewRelease(WGPUTextureView textureView)
{
    WebGPUTextureView* webgpuTextureView = reinterpret_cast<WebGPUTextureView*>(textureView);
    return webgpuTextureView->release();
}

void procTextureRelease(WGPUTexture texture)
{
    WebGPUTexture* webgpuTexture = reinterpret_cast<WebGPUTexture*>(texture);
    return webgpuTexture->release();
}

void procRenderPipelineRelease(WGPURenderPipeline renderPipeline)
{
    WebGPURenderPipeline* webgpuRenderPipeline = reinterpret_cast<WebGPURenderPipeline*>(renderPipeline);
    return webgpuRenderPipeline->release();
}

void procPipelineLayoutRelease(WGPUPipelineLayout pipelineLayout)
{
    WebGPUPipelineLayout* webgpuPipelineLayout = reinterpret_cast<WebGPUPipelineLayout*>(pipelineLayout);
    return webgpuPipelineLayout->release();
}

void procShaderModuleRelease(WGPUShaderModule shaderModule)
{
    WebGPUShaderModule* webgpuShaderModule = reinterpret_cast<WebGPUShaderModule*>(shaderModule);
    return webgpuShaderModule->release();
}

void procQueueRelease(WGPUQueue queue)
{
    WebGPUQueue* webgpuQueue = reinterpret_cast<WebGPUQueue*>(queue);
    return webgpuQueue->release();
}

void procDeviceDestroy(WGPUDevice device)
{
    // TODO
}

void procDeviceRelease(WGPUDevice device)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return webgpuDevice->release();
}

void procAdapterRelease(WGPUAdapter adapter)
{
    WebGPUAdapter* webgpuAdapter = reinterpret_cast<WebGPUAdapter*>(adapter);
    return webgpuAdapter->release();
}

void procSurfaceRelease(WGPUSurface surface)
{
    WebGPUSurface* webgpuSurface = reinterpret_cast<WebGPUSurface*>(surface);
    return webgpuSurface->release();
}

void procInstanceRelease(WGPUInstance instance)
{
    WebGPUInstance* webgpuInstance = reinterpret_cast<WebGPUInstance*>(instance);
    return webgpuInstance->release();
}

WGPUTexture procDeviceCreateTexture(WGPUDevice device, WGPUTextureDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUTexture>(webgpuDevice->createTexture(descriptor));
}

WGPUBuffer procDeviceCreateBuffer(WGPUDevice device, WGPUBufferDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUBuffer>(webgpuDevice->createBuffer(descriptor));
}

void* procBufferGetMappedRange(WGPUBuffer buffer, size_t offset, size_t size)
{
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    return webgpuBuffer->getMappedRange(offset, size);
}

void procBufferUnmap(WGPUBuffer buffer)
{
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    return webgpuBuffer->unmap();
}

void procRenderPassEncoderSetVertexBuffer(WGPURenderPassEncoder renderPassEncoder, uint32_t slot, WGPU_NULLABLE WGPUBuffer buffer, uint64_t offset, uint64_t size)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    return webgpuRenderPassEncoder->setVertexBuffer(slot, webgpuBuffer, offset, size);
}

void procRenderPassEncoderSetIndexBuffer(WGPURenderPassEncoder renderPassEncoder, WGPUBuffer buffer, WGPUIndexFormat format, uint64_t offset, uint64_t size)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    return webgpuRenderPassEncoder->setIndexBuffer(webgpuBuffer, format, offset, size);
}

void procRenderPassEncoderDrawIndexed(WGPURenderPassEncoder renderPassEncoder, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->drawIndexed(indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void procBufferDestroy(WGPUBuffer buffer)
{
    // TODO

    // WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    // return webgpuBuffer->destroy();
}

void procBufferRelease(WGPUBuffer buffer)
{
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    return webgpuBuffer->release();
}

void procRenderPassEncoderSetViewport(WGPURenderPassEncoder renderPassEncoder, float x, float y, float width, float height, float minDepth, float maxDepth)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->setViewport(x, y, width, height, minDepth, maxDepth);
}

void procRenderPassEncoderSetScissorRect(WGPURenderPassEncoder renderPassEncoder, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->setScissorRect(x, y, width, height);
}

void procQueueWriteBuffer(WGPUQueue queue, WGPUBuffer buffer, uint64_t bufferOffset, void const* data, size_t size)
{
    WebGPUQueue* webgpuQueue = reinterpret_cast<WebGPUQueue*>(queue);
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    return webgpuQueue->writeBuffer(webgpuBuffer, bufferOffset, data, size);
}

void procQueueWriteTexture(WGPUQueue queue, WGPUImageCopyTexture const* destination, void const* data, size_t dataSize, WGPUTextureDataLayout const* dataLayout, WGPUExtent3D const* writeSize)
{
    WebGPUQueue* webgpuQueue = reinterpret_cast<WebGPUQueue*>(queue);
    return webgpuQueue->writeTexture(destination, data, dataSize, dataLayout, writeSize);
}

void procRenderPassEncoderSetBindGroup(WGPURenderPassEncoder renderPassEncoder, uint32_t groupIndex, WGPU_NULLABLE WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    WebGPUBindGroup* webgpuBindGroup = reinterpret_cast<WebGPUBindGroup*>(group);
    return webgpuRenderPassEncoder->setBindGroup(groupIndex, webgpuBindGroup, dynamicOffsetCount, dynamicOffsets);
}

void procBindGroupRelease(WGPUBindGroup bindGroup)
{
    WebGPUBindGroup* webgpuBindGroup = reinterpret_cast<WebGPUBindGroup*>(bindGroup);
    return webgpuBindGroup->release();
}

void procBindGroupLayoutRelease(WGPUBindGroupLayout bindGroupLayout)
{
    WebGPUBindGroupLayout* webgpuBindGroupLayout = reinterpret_cast<WebGPUBindGroupLayout*>(bindGroupLayout);
    return webgpuBindGroupLayout->release();
}

WGPUSampler procDeviceCreateSampler(WGPUDevice device, WGPU_NULLABLE WGPUSamplerDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUSampler>(webgpuDevice->createSampler(descriptor));
}

void procSamplerRelease(WGPUSampler sampler)
{
    WebGPUSampler* webgpuSampler = reinterpret_cast<WebGPUSampler*>(sampler);
    return webgpuSampler->release();
}

WGPUWaitStatus procInstanceWaitAny(WGPUInstance instance, size_t futureCount, WGPUFutureWaitInfo* futures, uint64_t timeoutNS)
{
    WebGPUInstance* webgpuInstance = reinterpret_cast<WebGPUInstance*>(instance);
    return webgpuInstance->waitAny(futureCount, futures, timeoutNS);
}

void procInstanceProcessEvents(WGPUInstance instance)
{
    WebGPUInstance* webgpuInstance = reinterpret_cast<WebGPUInstance*>(instance);
    return webgpuInstance->processEvents();
}

WGPUFuture procQueueOnSubmittedWorkDone(WGPUQueue queue, WGPUQueueWorkDoneCallbackInfo2 callbackInfo)
{
    WebGPUQueue* webgpuQueue = reinterpret_cast<WebGPUQueue*>(queue);
    return webgpuQueue->onSubmittedWorkDone(callbackInfo);
}

uint64_t procBufferGetSize(WGPUBuffer buffer)
{
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);
    return webgpuBuffer->getSize();
}

WGPURenderBundleEncoder procDeviceCreateRenderBundleEncoder(WGPUDevice device, WGPURenderBundleEncoderDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPURenderBundleEncoder>(webgpuDevice->createRenderBundleEncoder(descriptor));
}

WGPURenderBundle procRenderBundleEncoderFinish(WGPURenderBundleEncoder renderBundleEncoder, WGPU_NULLABLE WGPURenderBundleDescriptor const* descriptor)
{
    WebGPURenderBundleEncoder* webgpuRenderBundleEncoder = reinterpret_cast<WebGPURenderBundleEncoder*>(renderBundleEncoder);
    return reinterpret_cast<WGPURenderBundle>(webgpuRenderBundleEncoder->finish(descriptor));
}

void procRenderBundleRelease(WGPURenderBundle renderBundle)
{
    WebGPURenderBundle* webgpuRenderBundle = reinterpret_cast<WebGPURenderBundle*>(renderBundle);
    return webgpuRenderBundle->release();
}

void procRenderBundleEncoderDraw(WGPURenderBundleEncoder renderBundleEncoder, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    WebGPURenderBundleEncoder* webgpuRenderBundleEncoder = reinterpret_cast<WebGPURenderBundleEncoder*>(renderBundleEncoder);
    return webgpuRenderBundleEncoder->draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void procRenderBundleEncoderDrawIndexed(WGPURenderBundleEncoder renderBundleEncoder, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance)
{
    WebGPURenderBundleEncoder* webgpuRenderBundleEncoder = reinterpret_cast<WebGPURenderBundleEncoder*>(renderBundleEncoder);
    return webgpuRenderBundleEncoder->drawIndexed(indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void procRenderBundleEncoderSetBindGroup(WGPURenderBundleEncoder renderBundleEncoder, uint32_t groupIndex, WGPU_NULLABLE WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets)
{
    WebGPURenderBundleEncoder* webgpuRenderBundleEncoder = reinterpret_cast<WebGPURenderBundleEncoder*>(renderBundleEncoder);
    WebGPUBindGroup* webgpuBindGroup = reinterpret_cast<WebGPUBindGroup*>(group);

    return webgpuRenderBundleEncoder->setBindGroup(groupIndex, webgpuBindGroup, dynamicOffsetCount, dynamicOffsets);
}

void procRenderBundleEncoderSetIndexBuffer(WGPURenderBundleEncoder renderBundleEncoder, WGPUBuffer buffer, WGPUIndexFormat format, uint64_t offset, uint64_t size)
{
    WebGPURenderBundleEncoder* webgpuRenderBundleEncoder = reinterpret_cast<WebGPURenderBundleEncoder*>(renderBundleEncoder);
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);

    return webgpuRenderBundleEncoder->setIndexBuffer(webgpuBuffer, format, offset, size);
}

void procRenderBundleEncoderSetPipeline(WGPURenderBundleEncoder renderBundleEncoder, WGPURenderPipeline pipeline)
{
    WebGPURenderBundleEncoder* webgpuRenderBundleEncoder = reinterpret_cast<WebGPURenderBundleEncoder*>(renderBundleEncoder);
    WebGPURenderPipeline* webgpuRenderPipeline = reinterpret_cast<WebGPURenderPipeline*>(pipeline);

    return webgpuRenderBundleEncoder->setPipeline(webgpuRenderPipeline);
}

void procRenderBundleEncoderSetVertexBuffer(WGPURenderBundleEncoder renderBundleEncoder, uint32_t slot, WGPU_NULLABLE WGPUBuffer buffer, uint64_t offset, uint64_t size)
{
    WebGPURenderBundleEncoder* webgpuRenderBundleEncoder = reinterpret_cast<WebGPURenderBundleEncoder*>(renderBundleEncoder);
    WebGPUBuffer* webgpuBuffer = reinterpret_cast<WebGPUBuffer*>(buffer);

    return webgpuRenderBundleEncoder->setVertexBuffer(slot, webgpuBuffer, offset, size);
}

void procRenderPassEncoderExecuteBundles(WGPURenderPassEncoder renderPassEncoder, size_t bundleCount, WGPURenderBundle const* bundles)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->executeBundles(bundleCount, bundles);
}

void procCommandEncoderCopyBufferToBuffer(WGPUCommandEncoder commandEncoder, WGPUBuffer source, uint64_t sourceOffset, WGPUBuffer destination, uint64_t destinationOffset, uint64_t size)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return webgpuCommandEncoder->copyBufferToBuffer(source, sourceOffset, destination, destinationOffset, size);
}

void procCommandEncoderCopyBufferToTexture(WGPUCommandEncoder commandEncoder, WGPUImageCopyBuffer const* source, WGPUImageCopyTexture const* destination, WGPUExtent3D const* copySize)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return webgpuCommandEncoder->copyBufferToTexture(source, destination, copySize);
}

void procCommandEncoderCopyTextureToBuffer(WGPUCommandEncoder commandEncoder, WGPUImageCopyTexture const* source, WGPUImageCopyBuffer const* destination, WGPUExtent3D const* copySize)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return webgpuCommandEncoder->copyTextureToBuffer(source, destination, copySize);
}

void procCommandEncoderCopyTextureToTexture(WGPUCommandEncoder commandEncoder, WGPUImageCopyTexture const* source, WGPUImageCopyTexture const* destination, WGPUExtent3D const* copySize)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return webgpuCommandEncoder->copyTextureToTexture(source, destination, copySize);
}

void procRenderPassEncoderSetBlendConstant(WGPURenderPassEncoder renderPassEncoder, WGPUColor const* color)
{
    WebGPURenderPassEncoder* webgpuRenderPassEncoder = reinterpret_cast<WebGPURenderPassEncoder*>(renderPassEncoder);
    return webgpuRenderPassEncoder->setBlendConstant(color);
}

WGPUComputePassEncoder procCommandEncoderBeginComputePass(WGPUCommandEncoder commandEncoder, WGPU_NULLABLE WGPUComputePassDescriptor const* descriptor)
{
    WebGPUCommandEncoder* webgpuCommandEncoder = reinterpret_cast<WebGPUCommandEncoder*>(commandEncoder);
    return reinterpret_cast<WGPUComputePassEncoder>(webgpuCommandEncoder->beginComputePass(descriptor));
}

void procComputePassEncoderDispatchWorkgroups(WGPUComputePassEncoder computePassEncoder, uint32_t workgroupCountX, uint32_t workgroupCountY, uint32_t workgroupCountZ)
{
    WebGPUComputePassEncoder* webgpuComputePassEncoder = reinterpret_cast<WebGPUComputePassEncoder*>(computePassEncoder);
    return webgpuComputePassEncoder->dispatchWorkgroups(workgroupCountX, workgroupCountY, workgroupCountZ);
}

void procComputePassEncoderEnd(WGPUComputePassEncoder computePassEncoder)
{
    WebGPUComputePassEncoder* webgpuComputePassEncoder = reinterpret_cast<WebGPUComputePassEncoder*>(computePassEncoder);
    return webgpuComputePassEncoder->end();
}

void procComputePassEncoderSetBindGroup(WGPUComputePassEncoder computePassEncoder, uint32_t groupIndex, WGPU_NULLABLE WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets)
{
    WebGPUComputePassEncoder* webgpuComputePassEncoder = reinterpret_cast<WebGPUComputePassEncoder*>(computePassEncoder);
    return webgpuComputePassEncoder->setBindGroup(groupIndex, group, dynamicOffsetCount, dynamicOffsets);
}

void procComputePassEncoderSetPipeline(WGPUComputePassEncoder computePassEncoder, WGPUComputePipeline pipeline)
{
    WebGPUComputePassEncoder* webgpuComputePassEncoder = reinterpret_cast<WebGPUComputePassEncoder*>(computePassEncoder);
    return webgpuComputePassEncoder->setPipeline(pipeline);
}

void procComputePassEncoderRelease(WGPUComputePassEncoder computePassEncoder)
{
    WebGPUComputePassEncoder* webgpuComputePassEncoder = reinterpret_cast<WebGPUComputePassEncoder*>(computePassEncoder);
    return webgpuComputePassEncoder->release();
}

WGPUComputePipeline procDeviceCreateComputePipeline(WGPUDevice device, WGPUComputePipelineDescriptor const* descriptor)
{
    WebGPUDevice* webgpuDevice = reinterpret_cast<WebGPUDevice*>(device);
    return reinterpret_cast<WGPUComputePipeline>(webgpuDevice->createComputePipeline(descriptor));
}

void procComputePipelineRelease(WGPUComputePipeline computePipeline)
{
    WebGPUComputePipeline* webgpuComputePipeline = reinterpret_cast<WebGPUComputePipeline*>(computePipeline);
    return webgpuComputePipeline->release();
}

namespace
{

std::unordered_map<std::string, WGPUProc> sProcMap{
    { "wgpuCreateInstance", reinterpret_cast<WGPUProc>(procCreateInstance) },
    { "wgpuInstanceRequestAdapter2", reinterpret_cast<WGPUProc>(procInstanceRequestAdapter) },
    { "wgpuInstanceCreateSurface", reinterpret_cast<WGPUProc>(procInstanceCreateSurface) },
    { "wgpuAdapterRequestDevice2", reinterpret_cast<WGPUProc>(procAdapterRequestDevice) },
    { "wgpuDeviceGetQueue", reinterpret_cast<WGPUProc>(procDeviceGetQueue) },
    { "wgpuSurfaceGetCapabilities", reinterpret_cast<WGPUProc>(procSurfaceGetCapabilities) },
    { "wgpuSurfaceConfigure", reinterpret_cast<WGPUProc>(procSurfaceConfigure) },
    { "wgpuDeviceCreateBindGroup", reinterpret_cast<WGPUProc>(procDeviceCreateBindGroup) },
    { "wgpuDeviceCreateBindGroupLayout", reinterpret_cast<WGPUProc>(procDeviceCreateBindGroupLayout) },
    { "wgpuDeviceCreatePipelineLayout", reinterpret_cast<WGPUProc>(procDeviceCreatePipelineLayout) },
    { "wgpuDeviceCreateRenderPipeline", reinterpret_cast<WGPUProc>(procDeviceCreateRenderPipeline) },
    { "wgpuDeviceCreateShaderModule", reinterpret_cast<WGPUProc>(procDeviceCreateShaderModule) },
    { "wgpuSurfaceGetCurrentTexture", reinterpret_cast<WGPUProc>(procSurfaceGetCurrentTexture) },
    { "wgpuTextureCreateView", reinterpret_cast<WGPUProc>(procTextureCreateView) },
    { "wgpuDeviceCreateCommandEncoder", reinterpret_cast<WGPUProc>(procDeviceCreateCommandEncoder) },
    { "wgpuCommandEncoderBeginRenderPass", reinterpret_cast<WGPUProc>(procCommandEncoderBeginRenderPass) },
    { "wgpuRenderPassEncoderSetPipeline", reinterpret_cast<WGPUProc>(procRenderPassEncoderSetPipeline) },
    { "wgpuRenderPassEncoderDraw", reinterpret_cast<WGPUProc>(procRenderPassEncoderDraw) },
    { "wgpuRenderPassEncoderEnd", reinterpret_cast<WGPUProc>(procRenderPassEncoderEnd) },
    { "wgpuRenderPassEncoderRelease", reinterpret_cast<WGPUProc>(procRenderPassEncoderRelease) },
    { "wgpuCommandEncoderFinish", reinterpret_cast<WGPUProc>(procCommandEncoderFinish) },
    { "wgpuQueueSubmit", reinterpret_cast<WGPUProc>(procQueueSubmit) },
    { "wgpuSurfacePresent", reinterpret_cast<WGPUProc>(procSurfacePresent) },
    { "wgpuCommandBufferRelease", reinterpret_cast<WGPUProc>(procCommandBufferRelease) },
    { "wgpuCommandEncoderRelease", reinterpret_cast<WGPUProc>(procCommandEncoderRelease) },
    { "wgpuTextureViewRelease", reinterpret_cast<WGPUProc>(procTextureViewRelease) },
    { "wgpuTextureRelease", reinterpret_cast<WGPUProc>(procTextureRelease) },
    { "wgpuRenderPipelineRelease", reinterpret_cast<WGPUProc>(procRenderPipelineRelease) },
    { "wgpuPipelineLayoutRelease", reinterpret_cast<WGPUProc>(procPipelineLayoutRelease) },
    { "wgpuShaderModuleRelease", reinterpret_cast<WGPUProc>(procShaderModuleRelease) },
    { "wgpuQueueRelease", reinterpret_cast<WGPUProc>(procQueueRelease) },
    { "wgpuDeviceDestroy", reinterpret_cast<WGPUProc>(procDeviceDestroy) },
    { "wgpuDeviceRelease", reinterpret_cast<WGPUProc>(procDeviceRelease) },
    { "wgpuAdapterRelease", reinterpret_cast<WGPUProc>(procAdapterRelease) },
    { "wgpuSurfaceRelease", reinterpret_cast<WGPUProc>(procSurfaceRelease) },
    { "wgpuInstanceRelease", reinterpret_cast<WGPUProc>(procInstanceRelease) },
    { "wgpuDeviceCreateTexture", reinterpret_cast<WGPUProc>(procDeviceCreateTexture) },
    { "wgpuDeviceCreateBuffer", reinterpret_cast<WGPUProc>(procDeviceCreateBuffer) },
    { "wgpuBufferGetMappedRange", reinterpret_cast<WGPUProc>(procBufferGetMappedRange) },
    { "wgpuBufferUnmap", reinterpret_cast<WGPUProc>(procBufferUnmap) },
    { "wgpuRenderPassEncoderSetVertexBuffer", reinterpret_cast<WGPUProc>(procRenderPassEncoderSetVertexBuffer) },
    { "wgpuRenderPassEncoderSetIndexBuffer", reinterpret_cast<WGPUProc>(procRenderPassEncoderSetIndexBuffer) },
    { "wgpuRenderPassEncoderDrawIndexed", reinterpret_cast<WGPUProc>(procRenderPassEncoderDrawIndexed) },
    { "wgpuBufferDestroy", reinterpret_cast<WGPUProc>(procBufferDestroy) },
    { "wgpuBufferRelease", reinterpret_cast<WGPUProc>(procBufferRelease) },
    { "wgpuRenderPassEncoderSetViewport", reinterpret_cast<WGPUProc>(procRenderPassEncoderSetViewport) },
    { "wgpuRenderPassEncoderSetScissorRect", reinterpret_cast<WGPUProc>(procRenderPassEncoderSetScissorRect) },
    { "wgpuQueueWriteBuffer", reinterpret_cast<WGPUProc>(procQueueWriteBuffer) },
    { "wgpuQueueWriteTexture", reinterpret_cast<WGPUProc>(procQueueWriteTexture) },
    { "wgpuRenderPassEncoderSetBindGroup", reinterpret_cast<WGPUProc>(procRenderPassEncoderSetBindGroup) },
    { "wgpuBindGroupRelease", reinterpret_cast<WGPUProc>(procBindGroupRelease) },
    { "wgpuBindGroupLayoutRelease", reinterpret_cast<WGPUProc>(procBindGroupLayoutRelease) },
    { "wgpuDeviceCreateSampler", reinterpret_cast<WGPUProc>(procDeviceCreateSampler) },
    { "wgpuSamplerRelease", reinterpret_cast<WGPUProc>(procSamplerRelease) },
    { "wgpuInstanceWaitAny", reinterpret_cast<WGPUProc>(procInstanceWaitAny) },
    { "wgpuInstanceProcessEvents", reinterpret_cast<WGPUProc>(procInstanceProcessEvents) },
    { "wgpuQueueOnSubmittedWorkDone2", reinterpret_cast<WGPUProc>(procQueueOnSubmittedWorkDone) },
    { "wgpuBufferGetSize", reinterpret_cast<WGPUProc>(procBufferGetSize) },
    { "wgpuDeviceCreateRenderBundleEncoder", reinterpret_cast<WGPUProc>(procDeviceCreateRenderBundleEncoder) },
    { "wgpuRenderBundleEncoderFinish", reinterpret_cast<WGPUProc>(procRenderBundleEncoderFinish) },
    { "wgpuRenderBundleRelease", reinterpret_cast<WGPUProc>(procRenderBundleRelease) },
    { "wgpuRenderBundleEncoderDraw", reinterpret_cast<WGPUProc>(procRenderBundleEncoderDraw) },
    { "wgpuRenderBundleEncoderDrawIndexed", reinterpret_cast<WGPUProc>(procRenderBundleEncoderDrawIndexed) },
    { "wgpuRenderBundleEncoderSetBindGroup", reinterpret_cast<WGPUProc>(procRenderBundleEncoderSetBindGroup) },
    { "wgpuRenderBundleEncoderSetIndexBuffer", reinterpret_cast<WGPUProc>(procRenderBundleEncoderSetIndexBuffer) },
    { "wgpuRenderBundleEncoderSetPipeline", reinterpret_cast<WGPUProc>(procRenderBundleEncoderSetPipeline) },
    { "wgpuRenderBundleEncoderSetVertexBuffer", reinterpret_cast<WGPUProc>(procRenderBundleEncoderSetVertexBuffer) },
    { "wgpuRenderPassEncoderExecuteBundles", reinterpret_cast<WGPUProc>(procRenderPassEncoderExecuteBundles) },
    { "wgpuCommandEncoderCopyBufferToBuffer", reinterpret_cast<WGPUProc>(procCommandEncoderCopyBufferToBuffer) },
    { "wgpuCommandEncoderCopyBufferToTexture", reinterpret_cast<WGPUProc>(procCommandEncoderCopyBufferToTexture) },
    { "wgpuCommandEncoderCopyTextureToBuffer", reinterpret_cast<WGPUProc>(procCommandEncoderCopyTextureToBuffer) },
    { "wgpuCommandEncoderCopyTextureToTexture", reinterpret_cast<WGPUProc>(procCommandEncoderCopyTextureToTexture) },
    { "wgpuRenderPassEncoderSetBlendConstant", reinterpret_cast<WGPUProc>(procRenderPassEncoderSetBlendConstant) },
    { "wgpuCommandEncoderBeginComputePass", reinterpret_cast<WGPUProc>(procCommandEncoderBeginComputePass) },
    { "wgpuComputePassEncoderDispatchWorkgroups", reinterpret_cast<WGPUProc>(procComputePassEncoderDispatchWorkgroups) },
    { "wgpuComputePassEncoderEnd", reinterpret_cast<WGPUProc>(procComputePassEncoderEnd) },
    { "wgpuComputePassEncoderSetBindGroup", reinterpret_cast<WGPUProc>(procComputePassEncoderSetBindGroup) },
    { "wgpuComputePassEncoderSetPipeline", reinterpret_cast<WGPUProc>(procComputePassEncoderSetPipeline) },
    { "wgpuComputePassEncoderRelease", reinterpret_cast<WGPUProc>(procComputePassEncoderRelease) },
    { "wgpuDeviceCreateComputePipeline", reinterpret_cast<WGPUProc>(procDeviceCreateComputePipeline) },
    { "wgpuComputePipelineRelease", reinterpret_cast<WGPUProc>(procComputePipelineRelease) },
};

} // namespace

WGPUProc procGetProcAddress(WGPUStringView procName)
{
    if (procName.data == nullptr)
    {
        return nullptr;
    }

    std::string key(procName.data, procName.length);
    if (sProcMap.contains(key))
    {
        return sProcMap[key];
    }

    return nullptr;
}

} // namespace jipu