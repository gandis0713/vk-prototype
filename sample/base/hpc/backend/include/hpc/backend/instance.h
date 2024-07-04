#pragma once

#include "export.h"
#include "handle.h"

#include <memory>

namespace hpc
{
namespace backend
{

enum class BackendType
{
    Mali,
    Adreno
};

class HPC_DEVICE_VISIBILITY Instance
{
public:
    static std::unique_ptr<Instance> create(const BackendType type);

    Instance() = default;
    virtual ~Instance() = default;

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;
};

} // namespace backend
} // namespace hpc