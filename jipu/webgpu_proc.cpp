#include <webgpu.h>

namespace jipu
{

extern WGPUProc procGetProcAddress(WGPUDevice device, char const* procName);
extern WGPUInstance procCreateInstance(WGPUInstanceDescriptor const* wgpuDescriptor);
extern void procInstanceRequestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options, WGPURequestAdapterCallback callback, void* userdata);
extern WGPUSurface procInstanceCreateSurface(WGPUInstance instance, WGPUSurfaceDescriptor const* descriptor);
extern void procAdapterRequestDevice(WGPUAdapter adapter, WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor, WGPURequestDeviceCallback callback, void* userdata);
extern WGPUQueue procDeviceGetQueue(WGPUDevice device);
extern WGPUStatus procSurfaceGetCapabilities(WGPUSurface surface, WGPUAdapter adapter, WGPUSurfaceCapabilities* capabilities);

} // namespace jipu

extern "C"
{
    using namespace jipu;

    WGPU_EXPORT WGPUProc wgpuGetProcAddress(WGPUDevice device, char const* procName) WGPU_FUNCTION_ATTRIBUTE
    {
        return procGetProcAddress(device, procName);
    }

    WGPU_EXPORT WGPUInstance wgpuCreateInstance(WGPU_NULLABLE WGPUInstanceDescriptor const* descriptor) WGPU_FUNCTION_ATTRIBUTE
    {
        return procCreateInstance(descriptor);
    }

    WGPU_EXPORT void wgpuInstanceRequestAdapter(WGPUInstance instance, WGPU_NULLABLE WGPURequestAdapterOptions const* options, WGPURequestAdapterCallback callback, WGPU_NULLABLE void* userdata) WGPU_FUNCTION_ATTRIBUTE
    {
        return procInstanceRequestAdapter(instance, options, callback, userdata);
    }

    WGPU_EXPORT WGPUSurface wgpuInstanceCreateSurface(WGPUInstance instance, WGPUSurfaceDescriptor const* descriptor) WGPU_FUNCTION_ATTRIBUTE
    {
        return procInstanceCreateSurface(instance, descriptor);
    }

    WGPU_EXPORT void wgpuAdapterRequestDevice(WGPUAdapter adapter, WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor, WGPURequestDeviceCallback callback, void* userdata) WGPU_FUNCTION_ATTRIBUTE
    {
        return procAdapterRequestDevice(adapter, descriptor, callback, userdata);
    }

    WGPU_EXPORT WGPUQueue wgpuDeviceGetQueue(WGPUDevice device) WGPU_FUNCTION_ATTRIBUTE
    {
        return procDeviceGetQueue(device);
    }

    WGPU_EXPORT WGPUStatus wgpuSurfaceGetCapabilities(WGPUSurface surface, WGPUAdapter adapter, WGPUSurfaceCapabilities* capabilities) WGPU_FUNCTION_ATTRIBUTE
    {
        return procSurfaceGetCapabilities(surface, adapter, capabilities);
    }
}