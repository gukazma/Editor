#include "OpenGLWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QTime>
#include <QtMath>
#include <QGridLayout>
#include "Scene/SceneManager.h"
#include "Scene/Camera.h"

#define GLEW_STATIC     

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , angularSpeed(0)
{
    fps = 60.0;
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

}
OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    doneCurrent();
    delete rubberBand;

}
void OpenGLWidget::mousePressEvent(QMouseEvent* e)
{
    if (SceneManager::EditorStatement::GetCurrentState() == false) {
        if (e->button() == Qt::RightButton) {
            isRightMousePress = GL_TRUE;
            update();
        }
        if (e->button() == Qt::LeftButton) {
            m_lastPoint1       = QVector2D(e->localPos());
            mousePressPosition = QVector2D(e->localPos());
            isLeftMousePress   = GL_TRUE;
            update();
        }
        m_lastPoint = QVector2D(e->localPos());
    }
    else {
        if (e->button() == Qt::LeftButton) {
            origin = e->pos();
            rubberBand->setGeometry(QRect(origin, QSize()));
            rubberBand->show();
        }
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* e)
{
    std::vector<QPoint> vertices;
    if (SceneManager::EditorStatement::GetCurrentState() == false) {
        if (e->button() == Qt::LeftButton || e->button() == Qt::RightButton) {
            if (e->button() == Qt::RightButton) {
                isRightMousePress = GL_FALSE;
            }
            if (e->button() == Qt::LeftButton) {
                isLeftMousePress = GL_FALSE;
            }
        }
    }
    else {
        //rubberBand->hide();
        if (e->button() == Qt::LeftButton && rubberBand) {
            QRect selectedRect = rubberBand->geometry();
            int    x1            = rubberBand->geometry().x();
            int    x2           = rubberBand->geometry().x()+rubberBand->geometry().width();
            int    y1            = rubberBand->geometry().y();
            int    y2            = rubberBand->geometry().y() + rubberBand->geometry().height();
            selectedRect.normalized();

            auto&& mesh         = g_sceneManager.m_meshs[0];
            auto&& m_mesh       = mesh.m_mesh;
                // 遍历点云集合，判断每个点是否在选择框内部
            for (auto& point : m_mesh.points()) {
                float f1=point.x(), f2=point.y(), f3=point.z();
                //if (selectedRect.contains(Point)) {
                //    // 将符合条件的点进行处理
                //    vertices.push_back(Point);
                //}
            }
        }

    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (SceneManager::EditorStatement::GetCurrentState() == false) {
        if (isRightMousePress) {
            LineMove(m_lastPoint, QVector2D(e->localPos()));
        }
        if (isLeftMousePress) {
            QVector2D diff = QVector2D(e->localPos()) - m_lastPoint1;
            QVector3D n    = QVector3D(diff.y(), diff.x(), 0.0).normalized();
            qreal     acc  = diff.length() / 80.0;
            m_rotationAxis = (m_rotationAxis * angularSpeed + n * acc).normalized();
            angularSpeed += acc;
        }
        m_lastPoint = QVector2D(e->localPos());
        update();
    }
    else {
        rubberBand->setGeometry(QRect(origin, e->pos()).normalized());
    }
}

void OpenGLWidget::LineMove(QVector2D posOrgin, QVector2D posEnd)
{
    float ratio   = 0.003f * sqrt(width() * height());
    float xoffset = posEnd.x() - posOrgin.x();
    float yoffset = posEnd.y() - posOrgin.y();

    m_lineMove.setX(m_lineMove.x() + xoffset * ratio);
    m_lineMove.setY(m_lineMove.y() - yoffset * ratio);
}
void OpenGLWidget::wheelEvent(QWheelEvent* e)
{
    int   offset = e->angleDelta().y() < 0 ? -1 : 1;
    qreal speed  = 0.9;
    //if (0 <= m_aspect + offset * speed && m_aspect + offset * speed <= 50) {
    //    m_aspect = m_aspect + offset * speed;
    //}
    m_aspect = m_aspect + offset * speed;
    if (m_aspect < 1) m_aspect = 1;
    e->accept();
    update();
}
void OpenGLWidget::timerEvent(QTimerEvent*)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.50;

    // Stop m_rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    }
    else {
        // Update m_rotation
        m_rotation = QQuaternion::fromAxisAndAngle(m_rotationAxis, angularSpeed) * m_rotation;
        // Request an update
        
        update();
    }
}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.9, 0.9, 0.9, 0);
    initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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
    //g_camera.resize(w, h);
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;
    m_projection.setToIdentity();
    m_projection.perspective(fov, aspect, zNear, zFar);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g_camera.bind(&program);
    for (auto&& mesh : g_sceneManager.m_meshs) {
        m_projection.setToIdentity();
        m_projection.perspective(45.0, width() / height(), c_near, c_far);

        QMatrix4x4 matrix;
        matrix.translate(0.0, 0.0, -10.0);
        matrix.rotate(m_rotation);
        matrix.scale(m_aspect);

        matrixView = QMatrix4x4();
        matrixView.lookAt(QVector3D(0, 0, 0), QVector3D(0.0, 0.0, -1), QVector3D(0.0, 1.0, 0.0));
        matrixView.translate(m_lineMove.x()/1000, m_lineMove.y()/1000, m_lineMove.z());

        program.setUniformValue("mvp_matrix", m_projection * matrixView * matrix);
        program.setUniformValue("u_LightPos", QVector3D(1.0f, 1.0f, 1.0f));
        program.setUniformValue("u_LightPos1", QVector3D(-1.0f, 1.0f, -1.0f));
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

