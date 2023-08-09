#pragma once
#include <Editor/Mesh/Types.h>
#include <boost/filesystem/path.hpp>
using Vec3f                                             = std::array<float, 3>;
namespace std {
template<> struct hash<Vec3f>
{
    size_t operator()(Vec3f const& vertex) const
    {
        return ((hash<float>()(vertex[0]) ^ (hash<float>()(vertex[2]) << 1)) >> 1) ^
               (hash<float>()(vertex[1]) << 1);
    }
};
}   // namespace std

namespace Editor {
class MeshImplBase
{
public:
    
    MeshImplBase()                                          = default;
    virtual void read(const boost::filesystem::path& path_);
    virtual void write(const boost::filesystem::path& path_);
    void         simpilify(float percent_);
    void         possionDiskSample(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals,
                                   float radius_);
    
    MyMesh&     getNativeMesh();
    void        updateData();
    std::vector<Vec3f>  m_vertices;
    std::vector<Vec3f>  m_normals;
    std::vector<size_t> m_indices;

protected:
    MyMesh m_nativeMesh;
    
};
}   // namespace Editor

