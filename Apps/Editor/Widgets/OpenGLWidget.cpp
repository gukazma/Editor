#include "OpenGLWidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTime>
#include <QtMath>
#include "Scene/SceneManager.h"
#include "Scene/Camera.h"
OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , angularSpeed(0)
{
    fps = 60.0;
}
OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    doneCurrent();
}
void OpenGLWidget::mousePressEvent(QMouseEvent* e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}
void OpenGLWidget::mouseReleaseEvent(QMouseEvent* e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    g_camera.rotate(rotation);
    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent* e) {
    auto delta = e->delta();
    g_camera.moveForward(delta);
    update();
}


void OpenGLWidget::timerEvent(QTimerEvent*)
{
    //// Decrease angular speed (friction)
    //angularSpeed *= 0.99;

    //// Stop rotation when speed goes below threshold
    //if (angularSpeed < 0.01) {
    //    angularSpeed = 0.0;
    //}
    //else {
    //    // Update rotation
    //    
    //    // Request an update
    //}
}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.9529411764705882, 0.9529411764705882, 0.9529411764705882, 1);

    initShaders();

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);

    startTimer(12);
}
void OpenGLWidget::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vshader.glsl")) {
        qDebug() << __FILE__ << __FUNCTION__ << " add vertex shader file failed.";
        close();
    }
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fshader.glsl")) {
        qDebug() << __FILE__ << __FUNCTION__ << " add fragment shader file failed.";
        close();
    }

    if (!program.link()) {
        qDebug() << __FILE__ << __LINE__ << "program link failed";
        close();
    }
    if (!program.bind()) {
        qDebug() << __FILE__ << __LINE__ << "program bind failed";
        close();
    }
}

void OpenGLWidget::resizeGL(int w, int h)
{
    g_camera.resize(w, h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_camera.bind(&program);
    for (auto&& mesh : g_sceneManager.m_meshs) {
        mesh.draw(&program);
    }
    calcFPS();
    paintFPS();
}

void OpenGLWidget::calcFPS()
{
    static QTime time;
    static int   once = [=]() {
        time.start();
        return 0;
    }();
    Q_UNUSED(once)
    static int frame = 0;
    if (frame++ > 100) {
        qreal elasped = time.elapsed();
        updateFPS(frame / elasped * 1000);
        time.restart();
        frame = 0;
    }
}
void OpenGLWidget::updateFPS(qreal v)
{
    fps = v;
}
void OpenGLWidget::paintFPS()
{
    //    QString str = QString("FPS:%1").arg(QString::number(fps, 'f', 3));
    //    this->setWindowTitle(str);
}
