#pragma once

#include "vertex.h"

#include <filesystem>
#include <vector>

namespace jipu
{

struct Polygon
{
    std::vector<Vertex> vertices{};
    std::vector<uint16_t> indices{};
};

Polygon loadOBJ(const std::filesystem::path& path);
Polygon loadOBJ(void* buf, uint64_t len);
Polygon loadGLTF(const std::filesystem::path& path);
Polygon loadGLTF(void* buf, uint64_t len, const std::filesystem::path& baseDir);

} // namespace jipu