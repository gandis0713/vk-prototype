#pragma once

#include "fps.h"
#include "hpc_watcher.h"
#include "native_imgui.h"
#include "window.h"

#include <deque>
#include <filesystem>
#include <optional>
#include <unordered_set>

#include <jipu/native/adapter.h>
#include <jipu/native/device.h>
#include <jipu/native/instance.h>
#include <jipu/native/physical_device.h>
#include <jipu/native/queue.h>
#include <jipu/native/surface.h>
#include <jipu/native/swapchain.h>

#include "hpc/instance.h"

namespace jipu
{

struct SampleDescriptor
{
    WindowDescriptor windowDescriptor;
    std::filesystem::path path;
};

class NativeSample : public Window
{
public:
    NativeSample() = delete;
    NativeSample(const SampleDescriptor& descriptor);
    virtual ~NativeSample();

public:
    virtual void createInstance();
    virtual void createAdapter();
    virtual void getPhysicalDevices();
    virtual void createSurface();
    virtual void createDevice();
    virtual void createSwapchain();
    virtual void createQueue();

public:
    void init() override;
    void onUpdate() override;
    void onResize(uint32_t width, uint32_t height) override;

public:
    void recordImGui(std::vector<std::function<void()>> cmds);
    void windowImGui(const char* title, std::vector<std::function<void()>> uis);
    void drawImGui(CommandEncoder* commandEncoder, TextureView* renderView);

public:
    void onHPCListner(Values values);

protected:
    std::filesystem::path m_appPath;
    std::filesystem::path m_appDir;

    std::unique_ptr<Instance> m_instance = nullptr;
    std::unique_ptr<Adapter> m_adapter = nullptr;
    std::vector<std::unique_ptr<PhysicalDevice>> m_physicalDevices{};
    std::unique_ptr<Device> m_device = nullptr;
    std::unique_ptr<Surface> m_surface = nullptr;
    std::unique_ptr<Queue> m_queue = nullptr;
    std::unique_ptr<Swapchain> m_swapchain = nullptr;
    std::unique_ptr<CommandEncoder> m_commandEncoder = nullptr;
    TextureView* m_renderView = nullptr;

protected:
    std::optional<NativeImGui> m_imgui = std::nullopt;

protected:
    std::unique_ptr<HPCWatcher> m_hpcWatcher = nullptr;
    std::unique_ptr<hpc::Instance> m_hpcInstance = nullptr;

protected:
    void createHPCWatcher(const std::unordered_set<hpc::Counter>& counters = {});
    void drawPolyline(std::string title, std::deque<float> data, std::string unit = "");
    void profilingWindow();

private:
    FPS m_fps{};
    std::unordered_map<hpc::Counter, std::deque<float>> m_profiling{};
};

} // namespace jipu
