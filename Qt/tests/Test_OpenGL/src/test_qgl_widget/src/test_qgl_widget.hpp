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
#ifndef TEST_QGLWIDGET_HPP
#define TEST_QGLWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include <QGLWidget>
#include <GL/glu.h>
//--------------------------------------------------------------------------------
class Test_QGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    Test_QGLWidget(QWidget *parent = 0);
    ~Test_QGLWidget();

    qreal get_x(void);
    qreal get_y(void);
    qreal get_z(void);

    qreal get_angle_x(void);
    qreal get_angle_y(void);
    qreal get_angle_z(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void draw(void);
    void draw_sphere(void);
    void draw_cylinder(void);
    void draw_object(void);
    void draw_cube(void);

    void set_X(qreal value);
    void set_Y(qreal value);
    void set_Z(qreal value);

    void set_angle_X(qreal value);
    void set_angle_Y(qreal value);
    void set_angle_Z(qreal value);

private:
    int faceAtPosition(const QPoint &pos);

    GLfloat rotationX = 0;
    GLfloat rotationY = 0;
    GLfloat rotationZ = 0;

    QColor faceColors[4];
    QPoint lastPos;

    GLUquadricObj *cylinder;
    GLUquadricObj *cylinder2;

    GLUquadricObj *sphere;

    GLfloat x = 0;
    GLfloat y = 0;
    GLfloat z = 0;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
};
//--------------------------------------------------------------------------------
#endif
