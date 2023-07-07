#include "OpenGLWidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTime>
#include <QtMath>
#include "Scene/SceneManager.h"
OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , cube(0)
    , texture(0)
    , angularSpeed(0)
{
    fps = 60.0;
}
OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    delete texture;
    delete cube;
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
}
void OpenGLWidget::timerEvent(QTimerEvent*)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    }
    else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
        // Request an update
        update();
    }
}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.9529411764705882, 0.9529411764705882, 0.9529411764705882, 1);

    initShaders();
    initTextures();

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);

    startTimer(12);
    //	timer.start(12, this);
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
void OpenGLWidget::initTextures()
{
    texture = new QOpenGLTexture(QImage(":/Images/cube.png").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();
    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -1.0);
    matrix.rotate(rotation);

    program.setUniformValue("mvp_matrix", projection * matrix);

    program.setUniformValue("texture", 0);

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
