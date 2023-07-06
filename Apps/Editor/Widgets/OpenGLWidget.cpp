#include "OpenGLWidget.h"
#include <QOpenGLFunctions>
#include <QSurfaceFormat>
OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.9529411764705882, 0.9529411764705882, 0.9529411764705882, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h) {

}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
}
