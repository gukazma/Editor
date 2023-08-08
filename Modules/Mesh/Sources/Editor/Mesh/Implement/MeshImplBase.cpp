#include <Editor/Mesh/Implement/MeshImplBase.h>
#include <boost/range/algorithm.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
#include <vcg/complex/algorithms/point_sampling.h>
#include <wrap/io_trimesh/export.h>

namespace Editor {
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

size_t MeshImplBase::findIndex(MyVertex& v_, std::vector<Vec3f>& vertices)
{
    for (size_t i = 0; i < vertices.size(); i++) {
        Vec3f v = {v_.P()[0], v_.P()[1], v_.P()[2]};
        if (v == vertices[i]) return i;
    }
}
void    MeshImplBase::getIndices(std::vector<size_t>& indices) {
    std::vector<Vec3f> vertices;
    getVertices(vertices);
    for (size_t i = 0; i < m_nativeMesh.face.size(); i++) {
        auto& face = m_nativeMesh.face[i];
        if (face.IsD()) continue;
        indices.push_back(findIndex(*face.V(0), vertices));
        indices.push_back(findIndex(*face.V(1), vertices));
        indices.push_back(findIndex(*face.V(2), vertices));
    }
}
MyMesh& MeshImplBase::getNativeMesh()
{
    return m_nativeMesh;
}
}   // namespace Editor