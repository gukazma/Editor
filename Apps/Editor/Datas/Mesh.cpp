#include "Mesh.h"
#include <vector>
#include <array>
#include <Editor/Mesh/Mesh.h>
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
    int vertexLocation = program->attributeLocation("a_Position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D) * 2);

    vertexLocation = program->attributeLocation("in_Normal");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, sizeof(QVector3D), 3, sizeof(QVector3D) * 2);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
}

void soarscape::Mesh::updateRenderData() {
    if (!m_mesh) {
        return;
    }
    
    std::vector<Vec3f>      vertices, normals;
    std::vector<size_t>     indices;
    
    m_mesh->getVertices(vertices);
    m_mesh->getNormals(normals);
    m_mesh->getIndices(indices);

    for (size_t i = 0; i < vertices.size(); i++) {
        VertexData v;
        auto       p = vertices[i];
        auto       n = normals[i];
        v.position   = {p[0], p[1], p[2]};
        v.normal     = {n[0], n[1], n[2]};
        m_vertices.push_back(v);
    }

    for (size_t i = 0; i < indices.size(); i++) {
        m_indices.push_back(indices[i]);
    }

    m_arrayBuf.bind();
    m_arrayBuf.allocate(vertices.data(), vertices.size()*sizeof(VertexData));
    m_indexBuf.bind();
    m_indexBuf.allocate(indices.data(), indices.size() * sizeof(GLushort));
}

void soarscape::Mesh::read(const boost::filesystem::path& path_) {
    m_mesh = std::make_shared<Editor::Mesh>(path_);
}
