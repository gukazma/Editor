#include "Editor/Mesh/Implement/Simplifiy/VCGSimplify.h"
#include "Editor/Mesh/Mesh.h"
#include <vcg/complex/base.h>
namespace Editor {
void VCGSimplify::operator()(Mesh* mesh_, float level_)
{
    auto& nativemesh = mesh_->getNativMesh();
}
}   // namespace Editor