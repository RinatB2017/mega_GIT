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
#ifndef SCENE3_HPP
#define SCENE3_HPP
//--------------------------------------------------------------------------------
#include <QRandomGenerator>
#include <QGraphicsScene>
//--------------------------------------------------------------------------------
#include "basescene.hpp"
//--------------------------------------------------------------------------------
class Player;
class Rock;
class Ship;
//--------------------------------------------------------------------------------
class Scene3 : public BaseScene
{
    Q_OBJECT

public:
    Scene3();
    virtual ~Scene3();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void update();

private:
    qreal x = 0.0;
    qreal y = 0.0;
    qreal angle  = 0;
    qreal w_item = 0;
    qreal h_item = 0;

    Player  *player = nullptr;
    Ship    *ship = nullptr;
    QList<Rock *> rocks;

    Ship    *moved_ship = nullptr;

    bool is_worked = false;

    void init_player();
    void init_ship();
    void init_rocks();

    void move_player();
    void move_rocks();
    void check_collision();

    void fire();

    void draw();
    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);
};
//--------------------------------------------------------------------------------
#endif
