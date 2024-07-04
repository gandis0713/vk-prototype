#include "adreno_instance.h"

#include "adreno_gpu.h"
#include "hpc/backend/handle.h"
#include "hpc/backend/instance.h"

#include <spdlog/spdlog.h>

namespace hpc
{
namespace adreno
{

std::vector<std::unique_ptr<GPU>> AdrenoInstance::gpus()
{
    // TODO: remove test code.
    auto handle = hpc::backend::Handle::create("/dev/kgsl-3d0");
    if (!handle)
    {
        spdlog::error("Failed to create device handle");
        return {};
    }

    m_instance = hpc::backend::Instance::create(std::move(handle));
    if (!m_instance)
    {
        spdlog::error("Failed to create device instance");
        return {};
    }

    return {};
}

} // namespace adreno
} // namespace hpc