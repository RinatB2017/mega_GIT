#include "MyGLWidget.hpp"

MyGLWidget::MyGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

}

void MyGLWidget::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//    glShadeModel(GL_FLAT);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
}

void MyGLWidget::resizeGL(int w, int h)
{
    // Update projection matrix and other size related settings:
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / GLfloat(h), 0.01f, 100.0f);

}

void MyGLWidget::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

#if 0
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif

#if 1
    glBegin (GL_QUADS);
        glColor3d(1,0,0);     // красный цвет
        //glLineWidth(1);       // ширину линии устанавливаем 1

        //glNormal3f(0.0, 0.0, 1.0);
        glVertex3f (1.0, 1.0, 1.0);
        glVertex3f (-1.0, 1.0, 1.0);
        glVertex3f (-1.0, -1.0, 1.0);
        glVertex3f (1.0, -1.0, 1.0);
    glEnd();
#endif

#if 0
    glLineWidth(1);       // ширину линии
                          // устанавливаем 1
    glBegin(GL_LINES);
        glColor3d(1,0,0);     // красный цвет
        glVertex3d(-4.5,3,0); // первая линия
        glVertex3d(-3,3,0);
        glColor3d(0,1,0);     // зеленый
        glVertex3d(-3,3.3,0); // вторая линия
        glVertex3d(-4,3.4,0);
    glEnd();
#endif
}
