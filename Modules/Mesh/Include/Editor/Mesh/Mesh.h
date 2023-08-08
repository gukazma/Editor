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
    void    getVertices(std::vector<Vec3f>& vertices);
    void    getNormals(std::vector<Vec3f>& normals);
    void    getIndices(std::vector<size_t>& indices);
    MyMesh& getNativMesh();

private:
    std::shared_ptr<MeshImplBase> m_meshImplement;
};
}   // namespace Editor