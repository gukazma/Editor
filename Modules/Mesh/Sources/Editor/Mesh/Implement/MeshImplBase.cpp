#include <Editor/Mesh/Implement/MeshImplBase.h>
#include <boost/range/algorithm.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
#include <vcg/complex/algorithms/point_sampling.h>
#include <wrap/io_trimesh/export.h>
#include <wrap/io_trimesh/import.h>

namespace Editor {
void MeshImplBase::read(const boost::filesystem::path& path_) {
    vcg::tri::io::Importer<MyMesh>::Open(m_nativeMesh, path_.generic_string().c_str());
}
void MeshImplBase::write(const boost::filesystem::path& path_)
{
    vcg::tri::io::Exporter<MyMesh>::Save(m_nativeMesh, path_.generic_string().c_str());
}
void MeshImplBase::simpilify(float percent_)
{
    int                                       FinalSize = m_nativeMesh.FN() * percent_;
    vcg::tri::TriEdgeCollapseQuadricParameter qparams;
    qparams.QualityThr       = .3;
    qparams.PreserveBoundary = false;
    double TargetError       = std::numeric_limits<double>::max();
    bool   CleaningFlag      = false;

    vcg::tri::UpdateBounding<MyMesh>::Box(m_nativeMesh);

    // decimator initialization
    vcg::LocalOptimization<MyMesh> DeciSession(m_nativeMesh, &qparams);

    int t1 = clock();
    DeciSession.Init<MyTriEdgeCollapse>();
    int t2 = clock();
    printf("Initial Heap Size %i\n", int(DeciSession.h.size()));

    DeciSession.SetTargetSimplices(FinalSize);
    DeciSession.SetTimeBudget(0.5f);
    DeciSession.SetTargetOperations(100000);
    if (TargetError < std::numeric_limits<float>::max()) DeciSession.SetTargetMetric(TargetError);

    while (DeciSession.DoOptimization() && m_nativeMesh.fn > FinalSize &&
           DeciSession.currMetric < TargetError)
        printf("Current Mesh size %7i heap sz %9i err %9g \n",
               m_nativeMesh.fn,
               int(DeciSession.h.size()),
               DeciSession.currMetric);

    int t3 = clock();
    printf("mesh  %d %d Error %g \n", m_nativeMesh.vn, m_nativeMesh.fn, DeciSession.currMetric);
    printf("\nCompleted in (%5.3f+%5.3f) sec\n",
           float(t2 - t1) / CLOCKS_PER_SEC,
           float(t3 - t2) / CLOCKS_PER_SEC);
    int dup   = vcg::tri::Clean<MyMesh>::RemoveDuplicateVertex(m_nativeMesh);
    int unref = vcg::tri::Clean<MyMesh>::RemoveUnreferencedVertex(m_nativeMesh);
    printf("Removed %i duplicate and %i unreferenced vertices from mesh \n", dup, unref);
}
void MeshImplBase::possionDiskSample(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals,
                                     float radius_)
{
    //----------------------------------------------------------------------
    // Basic Sample of a mesh surface
    // Build a point cloud with points with a plain poisson disk distribution

    int                       t0 = clock();
    std::vector<vcg::Point3f> pointVec;
    vcg::tri::PoissonSampling<MyMesh>(m_nativeMesh, pointVec, 0, radius_);
    int    t1 = clock();
    MyMesh BasicPoissonMesh;
    vcg::tri::BuildMeshFromCoordVector(BasicPoissonMesh, pointVec);

    vcg::tri::UpdateNormal<MyMesh>::PerFace(BasicPoissonMesh);
    for (size_t i = 0; i < BasicPoissonMesh.vert.size(); i++) {
        auto& v = BasicPoissonMesh.vert[i];
        Vec3f p = {v.P()[0], v.P()[1], v.P()[2]};
        Vec3f n = {v.N()[0], v.N()[1], v.N()[2]};
        vertices.push_back(p);
        normals.push_back(n);
    }
}
void MeshImplBase::updateVIMap() {
    int n = 0;
    for (size_t i = 0; i < m_nativeMesh.vert.size(); i++) {
        auto v = m_nativeMesh.vert[i];
        if (v.IsD()) continue;
        Vec3f p = {v.P()[0], v.P()[1], v.P()[2]};
        vimap[p] = n;
        n++;
    }
}
void    MeshImplBase::getVertices(std::vector<Vec3f>& vertices) {
    for (size_t i = 0; i < m_nativeMesh.vert.size(); i++) {
        auto v = m_nativeMesh.vert[i];
        if (v.IsD()) continue;

        Vec3f p = {v.P()[0], v.P()[1], v.P()[2]};
        vertices.push_back(p);
    }
}
void    MeshImplBase::getNormals(std::vector<Vec3f>& normals) {
    vcg::tri::UpdateNormal<MyMesh>::PerVertex(m_nativeMesh);
    for (size_t i = 0; i < m_nativeMesh.vert.size(); i++) {
        auto v = m_nativeMesh.vert[i];
        if (v.IsD()) continue;

        Vec3f n = {v.N()[0], v.N()[1], v.N()[2]};
        normals.push_back(n);
    }
}

void    MeshImplBase::getIndices(std::vector<size_t>& indices) {
    updateVIMap();
    indices.resize(m_nativeMesh.face.size()*3);

    for (size_t i = 0; i < m_nativeMesh.face.size(); i+=3) {
        auto& face = m_nativeMesh.face[i];
        if (face.IsD()) continue;
        Vec3f v0 = {face.V(0)->P()[0], face.V(0)->P()[1], face.V(0)->P()[2]};
        Vec3f v1 = {face.V(1)->P()[0], face.V(1)->P()[1], face.V(1)->P()[2]};
        Vec3f v2 = {face.V(2)->P()[0], face.V(2)->P()[1], face.V(2)->P()[2]};
        indices[i] = vimap[v0];
        indices[i+1] = vimap[v1];
        indices[i+2] = vimap[v2];
    }
}
MyMesh& MeshImplBase::getNativeMesh()
{
    return m_nativeMesh;
}
}   // namespace Editor