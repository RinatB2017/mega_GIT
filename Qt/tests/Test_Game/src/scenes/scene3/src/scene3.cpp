/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QtMath>
#include <QTime>
//--------------------------------------------------------------------------------
#include "scene3.hpp"
//--------------------------------------------------------------------------------
#include "player.hpp"
#include "rock.hpp"
#include "ship.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Scene3::Scene3(void)
{
    add_background(":/images/10.11-astronomiya.jpg");

    angle = rand() % 89;

    init_player();
    init_ship();
    init_rocks();

#ifdef QT_DEBUG
    qDebug() << "items().count() =" << items().count();
#endif
}
//--------------------------------------------------------------------------------
Scene3::~Scene3()
{
#ifdef QT_DEBUG
    qDebug() << "~Scene";
#endif
}
//--------------------------------------------------------------------------------
void Scene3::init_player(void)
{
    player = new Player;
    player->setPixmap(QPixmap(ICON_PROGRAMM));

    w_item = player->boundingRect().width();
    h_item = player->boundingRect().height();

    addItem(player);

    x = WIDTH/2  - player->boundingRect().width()/2;
    y = HEIGHT/2 - player->boundingRect().height()/2;

    player->setPos(x, y);
}
//--------------------------------------------------------------------------------
void Scene3::init_ship(void)
{
    ship = new Ship;
    addItem(ship);

    ship->setPos(WIDTH/2  - ship->boundingRect().width()/2,
                 HEIGHT/2 - ship->boundingRect().height()/2);
}
//--------------------------------------------------------------------------------
void Scene3::init_rocks(void)
{
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    int x = 0;
    for(int n=0; n<20; n++)
    {
        Rock *rock = new Rock;
        rocks.append(rock);
        if(x>30)
            x=0;
        else
            x++;

        int w = rock->boundingRect().width();
        int h = rock->boundingRect().height();
        if((w==0) && (h==0))
        {
            emit error(QString("m=%1").arg(n));
#ifdef QT_DEBUG
            qDebug() << "n =" << n;
#endif
            return;
        }


        int max_x = qrand() % WIDTH - rock->boundingRect().width();
        int max_y = qrand() % HEIGHT - rock->boundingRect().height();
        if(max_x < 0) max_x = 0;
        if(max_y < 0) max_y = 0;
        rock->setPos(max_x, max_y);

        addItem(rock);
    }
}
//--------------------------------------------------------------------------------
void Scene3::update(void)
{
    if(is_worked)
    {
        return;
    }
    is_worked = true;
    draw();
    is_worked = false;
}
//--------------------------------------------------------------------------------
void Scene3::draw(void)
{
    move_player();
    move_rocks();

    if(ship)
    {
        ship->animation();
    }

    //---
    //fire();
    //---

    check_collision();
}
//--------------------------------------------------------------------------------
void Scene3::fire(void)
{
    if(moved_ship == 0)
    {
        moved_ship = new Ship(false);
        int y = rand() % HEIGHT;
        if(y < moved_ship->boundingRect().height()) y = moved_ship->boundingRect().height();
        if(y > (HEIGHT - moved_ship->boundingRect().height())) y = HEIGHT - moved_ship->boundingRect().height();
        moved_ship->setPos(0, y);

        addItem(moved_ship);
    }

    int x = moved_ship->pos().x();
    int y = moved_ship->pos().y();
    if(x < (WIDTH - moved_ship->boundingRect().width()) )
    {
        x++;
        moved_ship->setPos(x, y);
    }
    else
    {
        removeItem(moved_ship);
        delete moved_ship;
        moved_ship = 0;
    }
}
//--------------------------------------------------------------------------------
void Scene3::move_player(void)
{
    if(player == 0)
    {
        return;
    }

    qreal temp = (int)(angle + 0.5) % 90;
    qreal alpha = 180.0 - (2.0 * temp);
    if(x<0)
    {
        emit error(QString("angle = %1").arg(angle));
        emit error(QString("temp = %1").arg(temp));
        emit error(QString("alpha = %1").arg(alpha));
        if(angle > 180)
            angle += alpha;
        else
            angle -= alpha;
        emit info(QString("angle = %1").arg(angle));
    }

    if(x>(WIDTH-w_item))
    {
        emit error(QString("angle = %1").arg(angle));
        emit error(QString("temp = %1").arg(temp));
        emit error(QString("alpha = %1").arg(alpha));
        if((angle >= 0) && (angle < 90))
            angle += alpha;
        else
            angle -= alpha;
        emit info(QString("angle = %1").arg(angle));
    }

    if(y<0)
    {
        emit error(QString("angle = %1").arg(angle));
        emit error(QString("temp = %1").arg(temp));
        emit error(QString("alpha = %1").arg(alpha));
        if(angle > 270)
            angle += alpha;
        else
            angle -= alpha;
        emit info(QString("angle = %1").arg(angle));
    }

    if(y>(HEIGHT-h_item))
    {
        emit error(QString("angle = %1").arg(angle));
        emit error(QString("temp = %1").arg(temp));
        emit error(QString("alpha = %1").arg(alpha));
        if(angle > 90)
            angle += alpha;
        else
            angle -= alpha;
        emit info(QString("angle = %1").arg(angle));
    }
    if(angle < 0)   angle += 360;
    if(angle > 360) angle -= 360;

    calc_line(x, y, angle, 1, &x, &y);

    player->setPos(x, y);
}
//--------------------------------------------------------------------------------
void Scene3::move_rocks(void)
{
    foreach (Rock *rock, rocks)
    {
        rock->animation();
        rock->move();
    }
}
//--------------------------------------------------------------------------------
void Scene3::check_collision(void)
{
    if(player == 0)
    {
        return;
    }

    QList<QGraphicsItem*> hits = player->collidingItems(Qt::IntersectsItemBoundingRect);
    foreach (QGraphicsItem *item, hits)
    {
        int index = 0;
        foreach (QGraphicsPixmapItem *p_item, rocks)
        {
            if(p_item == item)
            {
                removeItem(p_item);
                rocks.removeAt(index);

                emit info(QString("осталось %1").arg(rocks.count()));
            }
            index++;
        }
    }
    if(rocks.count() == 0)
    {
        //stop();
    }
}
//--------------------------------------------------------------------------------
void Scene3::calc_line(qreal center_x,
                       qreal center_y,
                       qreal angle,
                       qreal radius,
                       qreal *end_x,
                       qreal *end_y)
{
    Q_CHECK_PTR(end_x);
    Q_CHECK_PTR(end_y);

    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    *end_x = center_x + B;
    *end_y = center_y + C;
}
//--------------------------------------------------------------------------------
