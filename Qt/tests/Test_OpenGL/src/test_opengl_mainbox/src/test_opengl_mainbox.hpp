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

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (Test_QGLWidget::*func)(void);
    } CMD_t;

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    void createTestBar(void);
    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void draw(void);
    void draw_sphere(void);
    void draw_cylinder(void);
    void draw_object(void);
    void draw_cube(void);

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
