#include "Types.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix>
#include <QVector2D>
#include <QVector3D>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <memory>
namespace Editor {
class Mesh;
}
namespace soarscape {
class Mesh :  protected QOpenGLFunctions
{
public:
    struct VertexData
    {
        QVector3D position;
        QVector3D normal;

    };
    using Vec3f = std::array<float, 3>;
    Mesh();
    ~Mesh();
    void draw(QOpenGLShaderProgram* VPMatrix_);
    void updateRenderData();
    void read(const boost::filesystem::path& );

private:
    QOpenGLBuffer m_arrayBuf;
    QOpenGLBuffer m_indexBuf;
    std::shared_ptr<Editor::Mesh> m_mesh;
    size_t                        m_indiceSize;
};
}   // namespace soarscape
