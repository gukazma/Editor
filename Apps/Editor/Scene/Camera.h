#pragma once
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>
class QOpenGLShaderProgram;
class Camera : protected QOpenGLFunctions
{
public:
    Camera(qreal fov = 45.0, qreal aspect = 1024.0/720.0, qreal zNear = 0.1, qreal zFar = 100.0);
    ~Camera();
    void bind(QOpenGLShaderProgram* program);
    void resize(qreal w, qreal h);
    void rotate(const QQuaternion& rotate);
    void moveForward(qreal distance = 1.0);

private:
    QVector3D  m_focalPoint    = {0, 0, 0};
    QVector3D  m_position    = {0, 0, 0};
    qreal  m_focalDistance = 5.0;
    qreal      m_maxDistance   = 50.0;
    qreal      m_maxSpeed      = 1.0;
    QMatrix4x4 m_projectMatrix;
    QMatrix4x4 m_viewMatrix;
    qreal m_aspect = 0;
    qreal m_zNear = 0.1, m_zFar = 100.0, m_fov = 45.0;
};

extern Camera g_camera;