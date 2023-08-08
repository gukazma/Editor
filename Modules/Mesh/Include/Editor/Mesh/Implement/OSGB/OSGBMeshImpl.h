#pragma once
#include <Editor/Mesh/Implement/MeshImplBase.h>
namespace Editor {
class OSGBMeshImpleMesh : public MeshImplBase
{
public:
    virtual void read(const boost::filesystem::path& path_) override;
    virtual void write(const boost::filesystem::path& path_) override;
};
}   // namespace Editor