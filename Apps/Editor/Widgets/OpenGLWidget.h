#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget* parent);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
