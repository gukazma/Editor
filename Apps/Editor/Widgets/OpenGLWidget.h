#ifndef WINDOW_H
#define WINDOW_H

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>
#include <QMatrix4x4>
#include <QPoint>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

protected:
    void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* e) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent* e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

    void initShaders();
    void LineMove(QVector2D posOrgin, QVector2D posEnd);

private:
    void calcFPS();
    void updateFPS(qreal);
    void paintFPS();

private:
    GLboolean            isRightMousePress = GL_FALSE;
    GLboolean            isLeftMousePress  = GL_FALSE;

    QBasicTimer          timer;
    QOpenGLShaderProgram program;
    QMatrix4x4           m_projection;
    QVector3D   m_rotationAxis;
    qreal       angularSpeed;
    QQuaternion m_rotation;
    qreal       fps;
    QVector2D            mousePressPosition;

    qreal       m_aspect = 10;
    const float c_near   = 0.1f;
    const float c_far    = 100.0f;

    QMatrix4x4 matrixView;
    QVector3D m_lineMove = {0.0, 0.0, -3.0};
    QVector2D m_lastPoint;
    QVector2D  m_lastPoint1;
};

#endif   // WINDOW_H
