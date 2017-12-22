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
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "scene.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Scene::Scene(void)
{
    int r_width = 100;
    int r_height = 100;

    pix = new QPixmap(":/images/computer.ico");

    addRect(10, 10, r_width, r_height);
    addRect(10, HEIGHT-110, r_width, r_height);
    addRect(WIDTH-110, 10, r_width, r_height);
    addRect(WIDTH-110, HEIGHT-110, r_width, r_height);

    item = addPixmap(*pix);
    item->setPos(0, 0);

    x = 10;
}
//--------------------------------------------------------------------------------
Scene::~Scene()
{
    qDebug() << "~Scene";
}
//--------------------------------------------------------------------------------
void Scene::update(void)
{
    switch (direction)
    {
    case TOP:
        if(y>0)
        {
            y-=delta;
        }
        else
        {
            direction = RIGHT;
        }
        break;

    case BOTTOM:
        if(y<(HEIGHT - 32))
        {
            y+=delta;
        }
        else
        {
            direction = LEFT;
        }
        break;

    case LEFT:
        if(x>0)
        {
            x-=delta;
        }
        else
        {
            direction = TOP;
        }
        break;

    case RIGHT:
        if(x<(WIDTH - 32))
        {
            x+=delta;
        }
        else
        {
            direction = BOTTOM;
        }
        break;

    default:
        break;
    }

    if(item)
    {
#if 1
        item->setPos(x, y);
#else
        if(item->pos().x() > 500)
        {
            delete item;
            item = 0;
        }
        else
        {
            item->setPos(x, y);
        }
#endif
    }
}
//--------------------------------------------------------------------------------
