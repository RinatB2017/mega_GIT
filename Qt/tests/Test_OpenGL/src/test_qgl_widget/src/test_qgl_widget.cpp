/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
// http://www.firststeps.ru/mfc/opengl/opengl1.html
//--------------------------------------------------------------------------------
#include <QtGui>
#include <QtOpenGL>
//--------------------------------------------------------------------------------
#include <GL/glu.h>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_qgl_widget.hpp"
//--------------------------------------------------------------------------------
Test_QGLWidget::Test_QGLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    cylinder  = gluNewQuadric();
    cylinder2 = gluNewQuadric();

    sphere = gluNewQuadric();

    rotationX = 0.0; //-21.0;
    rotationY = 90.0; //-57.0;
    rotationZ = 0.0;

    faceColors[0] = Qt::red;
    faceColors[1] = Qt::green;
    faceColors[2] = Qt::blue;
    faceColors[3] = Qt::yellow;
}
//--------------------------------------------------------------------------------
Test_QGLWidget::~Test_QGLWidget()
{
    gluDeleteQuadric(cylinder);
    gluDeleteQuadric(cylinder2);
    gluDeleteQuadric(sphere);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::initializeGL()
{
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -1.0, +1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //draw();
    //draw_sphere();
    //draw_cylinder();
    draw_object();
    //draw_cube();
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if (event->buttons() & Qt::LeftButton)
    {
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        updateGL();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        rotationX += 180 * dy;
        rotationZ += 180 * dx;
        updateGL();
    }
    lastPos = event->pos();
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        z += 0.1;
        qDebug() << "up" << z;
        break;

    case Qt::Key_Down:
        z -= 0.1;
        qDebug() << "down" << z;
        break;

    case Qt::Key_Left:
        qDebug() << "left";
        break;

    case Qt::Key_Right:
        qDebug() << "right";
        break;

    case Qt::Key_Escape:
        qApp->quit();
        break;

    default:
        break;
    }
    updateGL();
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    int face = faceAtPosition(event->pos());
    if (face != -1)
    {
        QColor color = QColorDialog::getColor(faceColors[face], this);
        if (color.isValid())
        {
            faceColors[face] = color;
            updateGL();
        }
    }
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw(void)
{
    static const GLfloat P1[3] = { 0.0, -1.0, +2.0 };
    static const GLfloat P2[3] = { +1.73205081, -1.0, -1.0 };
    static const GLfloat P3[3] = { -1.73205081, -1.0, -1.0 };
    static const GLfloat P4[3] = { 0.0, +2.0, 0.0 };

    static const GLfloat * const coords[4][3] = {
        { P1, P2, P3 }, { P1, P3, P4 }, { P1, P4, P2 }, { P2, P4, P3 }
    };

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    for (int i = 0; i < 4; ++i)
    {
        glLoadName(i);
        glBegin(GL_TRIANGLES);
        qglColor(faceColors[i]);
        for (int j = 0; j < 3; ++j)
        {
            glVertex3f(coords[i][j][0],
                    coords[i][j][1],
                    coords[i][j][2]);
        }
        glEnd();
    }
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_sphere(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3d(1.0, 0.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    gluSphere(sphere, 1.0f, 10, 10);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_cylinder(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3d(1.0, 0.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //---
    glTranslatef(0.0, 0.0, -10.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    gluCylinder(cylinder, 0.1, 0.1, 2.0, 10, 20);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_object(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3d(1.0, 0.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //---
    glTranslatef(0.0, 0.0, -10.0);

    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
        glVertex3f (0, 0, 0);
        glVertex3f (0, 0, 100);
    glEnd();
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
        glVertex3f (0, 0, 0);
        glVertex3f (0, 100, 0);
    glEnd();
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
        glVertex3f (0, 0, 0);
        glVertex3f (100, 0, 0);
    glEnd();
    //---

    //---
    //glTranslatef(0.0, 0.0, -10.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    gluCylinder(cylinder, 0.1, 0.1, 2.0, 10, 20);
    //---

    //---
    glColor3d(0.0, 1.0, 0.0);

    glTranslatef(0.0, 0.0, -2.0);

    //glRotatef(rotationX, 1.0, 0.0, 0.0);
    //glRotatef(rotationY, 0.0, 1.0, 0.0);
    //glRotatef(rotationZ, 0.0, 0.0, 1.0);

    gluCylinder(cylinder2, 0.1, 0.1, 2.0, 10, 20);
    //---

    //---
    glColor3d(0.0, 0.0, 1.0);

    //glTranslatef(0.0, 0.0, -3.0);

    gluSphere(sphere, 0.5, 20, 20);
    //---

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(130, 1, 50, 0);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(150, 150, 150, 0, 0, 0, 0, 100, 0);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_cube(void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3d(1.0, 0.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -10.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    //---

    glBegin (GL_QUADS);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f (1.0, 1.0, 1.0);
        glVertex3f (-1.0, 1.0, 1.0);
        glVertex3f (-1.0, -1.0, 1.0);
        glVertex3f (1.0, -1.0, 1.0);
    glEnd();

    glBegin (GL_QUADS);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f (1.0, 1.0, -1.0);
        glVertex3f (1.0, -1.0, -1.0);
        glVertex3f (-1.0, -1.0, -1.0);
        glVertex3f (-1.0, 1.0, -1.0);
    glEnd();

    glBegin (GL_QUADS);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f (-1.0, 1.0, 1.0);
        glVertex3f (-1.0, 1.0, -1.0);
        glVertex3f (-1.0, -1.0, -1.0);
        glVertex3f (-1.0, -1.0, 1.0);
    glEnd();

    glBegin (GL_QUADS);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f (1.0, 1.0, 1.0);
        glVertex3f (1.0, -1.0, 1.0);
        glVertex3f (1.0, -1.0, -1.0);
        glVertex3f (1.0, 1.0, -1.0);
    glEnd();

    glBegin (GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f (-1.0, 1.0, -1.0);
        glVertex3f (-1.0, 1.0, 1.0);
        glVertex3f (1.0, 1.0, 1.0);
        glVertex3f (1.0, 1.0, -1.0);
    glEnd();

    glBegin(GL_QUADS);
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f (-1.0, -1.0, -1.0);
        glVertex3f (1.0, -1.0, -1.0);
        glVertex3f (1.0, -1.0, 1.0);
        glVertex3f (-1.0, -1.0, 1.0);
    glEnd();
}
//--------------------------------------------------------------------------------
int Test_QGLWidget::faceAtPosition(const QPoint &pos)
{
    const int MaxSize = 512;
    GLuint buffer[MaxSize];
    GLint viewport[4];

    makeCurrent();

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(MaxSize, buffer);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(GLdouble(pos.x()), GLdouble(viewport[3] - pos.y()),
            5.0, 5.0, viewport);
    GLfloat x = GLfloat(width()) / height();
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    draw();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    if (!glRenderMode(GL_RENDER))
        return -1;
    return buffer[3];
}
//--------------------------------------------------------------------------------
