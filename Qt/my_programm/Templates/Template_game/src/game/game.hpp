/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QPainter>
#include <QTimer>

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#   include <QGLWidget>
#else
#   include <QtOpenGLWidgets/QOpenGLWidget>
#endif
//--------------------------------------------------------------------------------
#define MAX_OBJECTS 4
//--------------------------------------------------------------------------------
enum {
    SPACE = 0,
    WALL = 1,
    PLAYER = 2,
    ALIEN = 3
};
enum {
    TOP = 0,
    RIGHT,
    LEFT,
    BOTTOM
};
//--------------------------------------------------------------------------------
class Player;
class Map;
//--------------------------------------------------------------------------------
#if (QT_VERSION > QT_VERSION_CHECK(6, 0, 0))
class Game : public QOpenGLWidget
#else
class Game : public QGLWidget
#endif
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    
signals:
    
private slots:
    void animate(void);
    
private:
    void DrawBackground(QPainter*);
    void DrawBlock(QPainter*, QBrush brush, int x, int y);
    void DrawMap(QPainter*);
    QPointer<QTimer> timer;
    QPainter painter;
    QBrush objects[MAX_OBJECTS];

    Player *player = nullptr;
    Map *map = nullptr;
    QSize *windowSize = nullptr;

    int player_x;
    int player_y;

    int alien_x;
    int alien_y;
    int alien_direction;

    bool m_doVisualUpdates;

    void init_GL(void);
    void init_map(const QString &filename);
    void init_player(void);
    void init_objects(void);
    void init_timer(void);
    void move_alien(void);

protected:
    void initializeGL(void);
    void resizeGL(int w, int h);
    void setupViewport(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintGL(void);
};
//--------------------------------------------------------------------------------
#endif
