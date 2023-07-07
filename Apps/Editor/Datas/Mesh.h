#include "Types.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix>
#include <QVector2D>
#include <QVector3D>
#include <fstream>
#include <boost/filesystem/path.hpp>
namespace soarscape {
class Mesh : public SurfaceMesh, protected QOpenGLFunctions
{
public:
    struct VertexData
    {
        QVector3D position;
    };

    Mesh();
    ~Mesh();
    void draw(QOpenGLShaderProgram* VPMatrix_);
    void updateRenderData();
    void read(const boost::filesystem::path& );

private:
    QOpenGLBuffer m_arrayBuf;
    QOpenGLBuffer m_indexBuf;
};
}   // namespace soarscape
