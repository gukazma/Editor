#include "Camera.h"
#include <QOpenGLShaderProgram>
#include <algorithm>
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
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_position, m_focalPoint, QVector3D(0.0, 1.0, 0.0));
    program->setUniformValue("mvp_matrix", m_projectMatrix * m_viewMatrix);
}

void Camera::resize(qreal w, qreal h) {
    m_aspect = w / h;
    m_projectMatrix.setToIdentity();
    m_projectMatrix.perspective(m_fov, m_aspect, m_zNear, m_zFar);
}

void Camera::rotate(const QQuaternion& rotate) {
    QMatrix4x4 rotateMatrix;
    rotateMatrix.setToIdentity();
    rotateMatrix.rotate(rotate);
    m_position = rotateMatrix * m_position;
}

void Camera::moveForward(qreal delta) {

    QVector3D forward = m_focalPoint - m_position;
    if (forward.length() > m_maxDistance) {
        delta=-delta;
    }
    float     normalizedDistance = forward.length() / m_maxDistance;   // 将距离归一化到 0-1 范围内
    float     speed = (1 - normalizedDistance) * m_maxSpeed;   // 根据归一化距离计算速度
    m_position -= forward.normalized() * delta / std::abs(delta) * speed;
}


