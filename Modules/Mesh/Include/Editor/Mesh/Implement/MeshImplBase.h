#pragma once
#include <Editor/Mesh/Types.h>
#include <boost/filesystem/path.hpp>
#include <unordered_map>
namespace Editor {
class MeshImplBase
{
public:
    using Vec3f                                             = std::array<float, 3>;
    MeshImplBase()                                          = default;
    virtual void read(const boost::filesystem::path& path_) = 0;
    virtual void write(const boost::filesystem::path& path_);
    void         simpilify(float percent_);
    void         possionDiskSample(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals,
                                   float radius_);

    void    getVertices(std::vector<Vec3f>& vertices);
    void    getNormals(std::vector<Vec3f>& normals);
    void    getIndices(std::vector<size_t>& indices);
    MyMesh&      getNativeMesh();

private:
    size_t findIndex(MyVertex& v_, std::vector<Vec3f>& vertices);

protected:
    MyMesh m_nativeMesh;
};
}   // namespace Editor