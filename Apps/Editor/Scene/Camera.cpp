#include "Camera.h"
#include <QOpenGLShaderProgram>
Camera g_camera;
Camera::Camera(qreal fov, qreal aspect, qreal zNear, qreal zFar)
{
    m_projectMatrix.setToIdentity();
    m_projectMatrix.perspective(fov, aspect, zNear, zFar);

    m_viewMatrix.setToIdentity();
    m_position = m_focalPoint - QVector3D(0.0, 0.0, m_focalDistance);
    m_viewMatrix.translate(m_position);
}

Camera::~Camera() {}

void Camera::bind(QOpenGLShaderProgram* program) {
    program->setUniformValue("mvp_matrix", m_projectMatrix * m_viewMatrix);
}

void Camera::resize(qreal w, qreal h) {
    m_aspect = w / h;
    m_projectMatrix.setToIdentity();
    m_projectMatrix.perspective(m_fov, m_aspect, m_zNear, m_zFar);
}


