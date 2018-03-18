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
#include <QtWidgets>
//#include <QAbstractEventDispatcher>
//--------------------------------------------------------------------------------
#include "player.hpp"
#include "map.hpp"
//--------------------------------------------------------------------------------
#include "game.hpp"
//--------------------------------------------------------------------------------
#ifndef GL_MULTISAMPLE
#   define GL_MULTISAMPLE  0x809D
#endif
//--------------------------------------------------------------------------------
#define SIZE_IMAGE 32
//--------------------------------------------------------------------------------
#define INTERVAL_TIMER 100
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Game::Game(QWidget *parent) :
    QGLWidget(parent),
    WindowSize(NULL),
    m_doVisualUpdates(true)
{
    init_GL();
    init_map(":/map.txt");
    init_player();
    init_objects();

    alien_direction = RIGHT;

    init_timer();
    //connect(QAbstractEventDispatcher::instance(), SIGNAL(aboutToBlock()), this, SLOT(animate()));
    //connect(QAbstractEventDispatcher::instance(), SIGNAL(awake()), this, SLOT(animate()));

    setFixedSize(map->get_max_x()*32, map->get_max_y()*32);
}
//--------------------------------------------------------------------------------
void Game::init_timer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(INTERVAL_TIMER);
}
//--------------------------------------------------------------------------------
void Game::init_objects(void)
{
    objects[SPACE]  = QBrush(QImage(":/5.png"));
    objects[WALL]   = QBrush(QImage(":/14.png"));
    objects[PLAYER] = QBrush(QImage(":/31.png"));
    objects[ALIEN]  = QBrush(QImage(":/4.png"));
}
//--------------------------------------------------------------------------------
void Game::init_player(void)
{
    int x, y;
    for(y=0; y<map->get_max_y(); y++)
    {
        for(x=0; x<map->get_max_x(); x++)
        {
            if(map->get(x, y) == ALIEN)
            {
                alien_x = x;
                alien_y = y;
            }
            if(map->get(x, y) == PLAYER)
            {
                player_x = x;
                player_y = y;
                player = new Player(map, true, player_x, player_y);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void Game::init_map(const QString filename)
{
    map = new Map;
    int r = map->load(filename);
    if(r != NO_ERROR_ADV)
        QTimer::singleShot(1000, qApp, SLOT(quit()));
}
//--------------------------------------------------------------------------------
void Game::init_GL(void)
{
    makeCurrent();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    qglClearColor(QColor(100,150,80));

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT5);
    glEnable(GL_MULTISAMPLE);

    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT5, GL_POSITION, lightPosition);
    setupViewport(width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    setupViewport(width(), height());

    setAutoFillBackground(false);
}
//--------------------------------------------------------------------------------
void Game::animate(void)
{
    paintGL();
}
//--------------------------------------------------------------------------------
void Game::DrawBackground(QPainter *painter)
{
    painter->save();
    painter->setBrush(objects[SPACE]);
    painter->drawRect(0, 0, map->get_max_x()*32, map->get_max_y()*32);
    painter->restore();
}
//--------------------------------------------------------------------------------
void Game::DrawBlock(QPainter *painter, QBrush brush, int x, int y)
{
    painter->save();
    painter->setBrush(brush);
    painter->drawRect(x, y, brush.textureImage().size().width(), brush.textureImage().height());
    painter->restore();
}
//--------------------------------------------------------------------------------
void Game::initializeGL(void)
{
    glEnable(GL_MULTISAMPLE);
}
//--------------------------------------------------------------------------------
void Game::resizeGL(int width, int height)
{
    setupViewport(width,height);
    if (WindowSize)
        delete WindowSize;
    WindowSize = new QSize(width,height);
}
//--------------------------------------------------------------------------------
void Game::setupViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES
    glOrthof(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}
//--------------------------------------------------------------------------------
void Game::DrawMap(QPainter *painter)
{
    int x,y;

    painter->save();
    for(y=0; y<map->get_max_y(); y++)
    {
        for(x=0; x<map->get_max_x(); x++)
        {
            painter->setBrush(objects[(int)map->get(x,y)]);
            painter->drawRect(x*SIZE_IMAGE, y*SIZE_IMAGE, SIZE_IMAGE, SIZE_IMAGE);
        }
    }
    painter->restore();
}
//--------------------------------------------------------------------------------
void Game::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
#ifdef QT_DEBUG
        qDebug() << "left press" << event->x() << ":" << event->y();
#endif
        break;

    case Qt::RightButton:
#ifdef QT_DEBUG
        qDebug() << "right press" << event->x() << ":" << event->y();
#endif
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void Game::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        player->left();
        break;

    case Qt::Key_Right:
        player->right();
        break;

    case Qt::Key_Up:
        player->top();
        break;

    case Qt::Key_Down:
        player->bottom();
        break;

    case Qt::Key_Space:
#ifdef QT_DEBUG
        qDebug() << "Key_Space";
#endif
        break;

    case Qt::Key_Escape:
        qApp->quit();
        break;
    }
}
//--------------------------------------------------------------------------------
void Game::move_alien(void)
{
    switch(alien_direction)
    {
    case TOP:
        if(map->get(alien_x, alien_y-1) == SPACE)
        {
            map->set(alien_x, alien_y, SPACE);
            map->set(alien_x, alien_y-1, ALIEN);
            alien_y--;
        }
        else
        {
            alien_direction = RIGHT;
        }
        break;

    case LEFT:
        if(map->get(alien_x-1, alien_y) == SPACE)
        {
            map->set(alien_x, alien_y, SPACE);
            map->set(alien_x-1, alien_y, ALIEN);
            alien_x--;
        }
        else
        {
            alien_direction = TOP;
        }
        break;

    case RIGHT:
        if(map->get(alien_x+1, alien_y) == SPACE)
        {
            map->set(alien_x, alien_y, SPACE);
            map->set(alien_x+1, alien_y, ALIEN);
            alien_x++;
        }
        else
        {
            alien_direction = BOTTOM;
        }
        break;

    case BOTTOM:
        if(map->get(alien_x, alien_y+1) == SPACE)
        {
            map->set(alien_x, alien_y, SPACE);
            map->set(alien_x, alien_y+1, ALIEN);
            alien_y++;
        }
        else
        {
            alien_direction = LEFT;
        }
        break;

    }
}
//--------------------------------------------------------------------------------
void Game::paintGL(void)
{
    if(m_doVisualUpdates)
    {
        m_doVisualUpdates = false;

        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);
        move_alien();
        DrawMap(&painter);

        painter.end();
        m_doVisualUpdates = true;
    }
}
//--------------------------------------------------------------------------------
