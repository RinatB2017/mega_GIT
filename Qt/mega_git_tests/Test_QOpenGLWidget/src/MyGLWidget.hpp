
#ifndef MYGLWIDGET_HPP
#define MYGLWIDGET_HPP

#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QMatrix4x4>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    MyGLWidget(QWidget *parent = nullptr);

private:
    QMatrix4x4 m_projection;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif
