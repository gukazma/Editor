#pragma once
#include <Editor/Mesh/Types.h>
#include <array>
#include <boost/filesystem/path.hpp>
#include <memory>
#include <vector>
namespace Editor {
class MeshImplBase;
class Mesh
{
public:
    enum DataType
    {
        OSGB = 0,
        GLTF,
    };

    using Vec3f = std::array<float, 3>;
    explicit Mesh(const boost::filesystem::path& path_, DataType type = OSGB);
    void    write(const boost::filesystem::path& path_);
    void    simpilify(float percent_);
    void    possionDiskSample(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals,
                              float radius_);
    void    updateData();
    MyMesh& getNativMesh();
    std::vector<Vec3f>  m_vertices;
    std::vector<Vec3f>  m_normals;
    std::vector<size_t> m_indices;

private:
    std::shared_ptr<MeshImplBase> m_meshImplement;
};
}   // namespace Editor