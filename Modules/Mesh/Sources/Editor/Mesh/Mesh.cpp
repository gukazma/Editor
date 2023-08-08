#include <Editor/Mesh/Implement/OSGB/OSGBMeshImpl.h>
#include <Editor/Mesh/Mesh.h>
namespace Editor {
Mesh::Mesh(const boost::filesystem::path& path_, DataType type)
{
    switch (type) {
    case Editor::Mesh::OSGB: m_meshImplement = std::make_shared<OSGBMeshImpleMesh>(); break;
    case Editor::Mesh::GLTF: break;
    default: break;
    }
    m_meshImplement->read(path_);
}

void Mesh::write(const boost::filesystem::path& path_)
{
    m_meshImplement->write(path_);
}

void Mesh::simpilify(float percent_)
{
    m_meshImplement->simpilify(percent_);
}

void Mesh::possionDiskSample(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals,
                             float radius_)
{
    m_meshImplement->possionDiskSample(vertices, normals, radius_);
}

void Mesh::getVertices(std::vector<Vec3f>& vertices) {
    m_meshImplement->getVertices(vertices);
}

void Mesh::getNormals(std::vector<Vec3f>& normals) {
    m_meshImplement->getNormals(normals);
}

void Mesh::getIndices(std::vector<size_t>& indices) {
    m_meshImplement->getIndices(indices);
}


MyMesh& Mesh::getNativMesh()
{
    return m_meshImplement->getNativeMesh();
}

}   // namespace Editor