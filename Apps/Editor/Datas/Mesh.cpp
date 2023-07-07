#include "Mesh.h"
#include <vector>
#include <array>
soarscape::Mesh::Mesh() 
    : m_arrayBuf(QOpenGLBuffer::VertexBuffer)
    , m_indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    m_arrayBuf.create();
    m_indexBuf.create();
}

soarscape::Mesh::~Mesh() {}

void soarscape::Mesh::draw(QOpenGLShaderProgram* program) {
    // Tell OpenGL which VBOs to use
    m_arrayBuf.bind();
    m_indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, number_of_faces()*3, GL_UNSIGNED_SHORT, 0);
}

void soarscape::Mesh::updateRenderData() {
    std::vector<VertexData> vertices;
    std::vector<GLushort>   indices;
    for (auto&& p : points()) {
        VertexData vertex;
        vertex.position = {p.x(), p.y(), p.z()};
        vertices.push_back(vertex);
    }

    for (auto&& f : faces()) {
        auto face_hf = halfedge(f);
        auto fvs =  vertices_around_face(face_hf);
        for (auto&& v : fvs) {
            indices.push_back(v);
        }
    }
    m_arrayBuf.bind();
    m_arrayBuf.allocate(vertices.data(), vertices.size()*sizeof(VertexData));
    m_indexBuf.bind();
    m_indexBuf.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void soarscape::Mesh::read(const boost::filesystem::path& path_) {
    CGAL::read_mesh(*this, path_.generic_string());
}
