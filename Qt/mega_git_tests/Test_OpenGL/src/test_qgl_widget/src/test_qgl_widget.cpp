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
    l_obj.append(cylinder);
    l_obj.append(cylinder2);

    sphere = gluNewQuadric();
    sphere2 = gluNewQuadric();
    l_obj.append(sphere);
    l_obj.append(sphere2);

    sphere_0 = gluNewQuadric();
    sphere_1 = gluNewQuadric();
    sphere_2 = gluNewQuadric();
    sphere_3 = gluNewQuadric();
    sphere_4 = gluNewQuadric();
    sphere_5 = gluNewQuadric();
    sphere_6 = gluNewQuadric();
    sphere_7 = gluNewQuadric();
    l_obj.append(sphere_0);
    l_obj.append(sphere_1);
    l_obj.append(sphere_2);
    l_obj.append(sphere_3);
    l_obj.append(sphere_4);
    l_obj.append(sphere_5);
    l_obj.append(sphere_6);
    l_obj.append(sphere_7);

    cylinder_00 = gluNewQuadric();
    cylinder_01 = gluNewQuadric();
    cylinder_02 = gluNewQuadric();
    cylinder_03 = gluNewQuadric();
    cylinder_04 = gluNewQuadric();
    cylinder_05 = gluNewQuadric();
    cylinder_06 = gluNewQuadric();
    cylinder_07 = gluNewQuadric();
    cylinder_08 = gluNewQuadric();
    cylinder_09 = gluNewQuadric();
    cylinder_10 = gluNewQuadric();
    cylinder_11 = gluNewQuadric();
    l_obj.append(cylinder_00);
    l_obj.append(cylinder_01);
    l_obj.append(cylinder_02);
    l_obj.append(cylinder_03);
    l_obj.append(cylinder_04);
    l_obj.append(cylinder_05);
    l_obj.append(cylinder_06);
    l_obj.append(cylinder_07);
    l_obj.append(cylinder_08);
    l_obj.append(cylinder_09);
    l_obj.append(cylinder_10);
    l_obj.append(cylinder_11);

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
    foreach (GLUquadricObj *obj, l_obj)
    {
        gluDeleteQuadric(obj);
    }
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(x, y, z);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    add_grid();

    if(flag_o0) draw();
    if(flag_o1) draw_sphere();
    if(flag_o2) draw_cylinder();
    if(flag_o3) draw_object();
    if(flag_o4) draw_object2();
    if(flag_o5) draw_cube();
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::show_object(bool o0,
                                 bool o1,
                                 bool o2,
                                 bool o3,
                                 bool o4,
                                 bool o5)
{
    flag_o0 = o0;
    flag_o1 = o1;
    flag_o2 = o2;
    flag_o3 = o3;
    flag_o4 = o4;
    flag_o5 = o5;

    emit info(QString("flag_o0 %1").arg(flag_o0));
    emit info(QString("flag_o1 %1").arg(flag_o1));
    emit info(QString("flag_o2 %1").arg(flag_o2));
    emit info(QString("flag_o3 %1").arg(flag_o3));
    emit info(QString("flag_o4 %1").arg(flag_o4));
    emit info(QString("flag_o5 %1").arg(flag_o5));
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
void Test_QGLWidget::add_grid(void)
{
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

    // R z
    // G y
    // B x
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
    gluSphere(sphere, 1.0f, 10, 10);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_cylinder(void)
{
    gluCylinder(cylinder, 0.1, 0.1, 2.0, 10, 20);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_object(void)
{
    //---
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

    //---
    glColor3d(1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 4.0);
    gluSphere(sphere2, 0.5, 20, 20);
    //---
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_object2(void)
{
    glColor3f(1, 0, 0);

    qreal radius = 0.1;
    GLint slices = 20;
    GLint stacks = 20;
    //---
    glTranslatef(0.0, 0.0, 0.0);
    gluSphere(sphere_0, radius, slices, stacks);
    //---
    glTranslatef(-1.0, 0.0, 0.0);
    gluSphere(sphere_1, radius, slices, stacks);
    //---
    glTranslatef(0.0, -1.0, 0.0);
    gluSphere(sphere_2, radius, slices, stacks);
    //---
    glTranslatef(1.0, 0.0, 0.0);
    gluSphere(sphere_3, radius, slices, stacks);
    //---
    glTranslatef(0.0, 1.0, 1.0);
    //---
    glTranslatef(0.0, 0.0, 0.0);
    gluSphere(sphere_4, radius, slices, stacks);
    //---
    glTranslatef(-1.0, 0.0, 0.0);
    gluSphere(sphere_5, radius, slices, stacks);
    //---
    glTranslatef(0.0, -1.0, 0.0);
    gluSphere(sphere_6, radius, slices, stacks);
    //---
    glTranslatef(1.0, 0.0, 0.0);
    gluSphere(sphere_7, radius, slices, stacks);
    //---

    glColor3f(0, 0, 1);

    glTranslatef(0.0, 1.0, -1.0);

    //---
    glTranslatef(0.0, 0.0, 0.0);
    gluCylinder(cylinder_00, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylinder_01, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylinder_02, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylinder_03, 0.05, 0.05, 1.0, 10, 20);
    //---
    glTranslatef(-1.0, 0.0, 0.0);
    //---
    glTranslatef(0.0, 0.0, 0.0);
    gluCylinder(cylinder_04, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylinder_05, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylinder_06, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylinder_07, 0.05, 0.05, 1.0, 10, 20);
    //---

    glRotatef(90.0, 0.0, 1.0, 0.0);
    gluCylinder(cylinder_08, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, -1.0, 0.0);
    gluCylinder(cylinder_09, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(-1.0, 0.0, 0.0);
    gluCylinder(cylinder_10, 0.05, 0.05, 1.0, 10, 20);

    glTranslatef(0.0, 1.0, 0.0);
    gluCylinder(cylinder_11, 0.05, 0.05, 1.0, 10, 20);
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::draw_cube(void)
{
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
void Test_QGLWidget::set_X(qreal value)
{
    x = value;
    updateGL();

    emit info(QString("x = %1").arg(x));
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::set_Y(qreal value)
{
    y = value;
    updateGL();

    emit info(QString("y = %1").arg(y));
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::set_Z(qreal value)
{
    z = value;
    updateGL();

    emit info(QString("z = %1").arg(z));
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::set_angle_X(qreal value)
{
    rotationX = value;
    updateGL();

    emit info(QString("z = %1").arg(rotationX));
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::set_angle_Y(qreal value)
{
    rotationY = value;
    updateGL();

    emit info(QString("y = %1").arg(rotationY));
}
//--------------------------------------------------------------------------------
void Test_QGLWidget::set_angle_Z(qreal value)
{
    rotationZ = value;
    updateGL();

    emit info(QString("z = %1").arg(rotationZ));
}
//--------------------------------------------------------------------------------
qreal Test_QGLWidget::get_x(void)
{
    return x;
}
//--------------------------------------------------------------------------------
qreal Test_QGLWidget::get_y(void)
{
    return y;
}
//--------------------------------------------------------------------------------
qreal Test_QGLWidget::get_z(void)
{
    return z;
}
//--------------------------------------------------------------------------------
qreal Test_QGLWidget::get_angle_x(void)
{
    return rotationX;
}
//--------------------------------------------------------------------------------
qreal Test_QGLWidget::get_angle_y(void)
{
    return rotationY;
}
//--------------------------------------------------------------------------------
qreal Test_QGLWidget::get_angle_z(void)
{
    return rotationZ;
}
//--------------------------------------------------------------------------------
