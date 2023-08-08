#pragma once

namespace Editor {
class Mesh;
struct VCGSimplify
{
    void operator()(Mesh* mesh_, float level_);
};
}   // namespace Editor