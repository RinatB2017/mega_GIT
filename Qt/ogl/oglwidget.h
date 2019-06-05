#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
//#include <gl/GLU.h>
//#include <gl/GL.h>

#include <GL/glu.h>
#include <GL/gl.h>

class OGLWidget : public QOpenGLWidget
{
public:
    OGLWidget(QWidget *parent = nullptr);
    ~OGLWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // OGLWIDGET_H
