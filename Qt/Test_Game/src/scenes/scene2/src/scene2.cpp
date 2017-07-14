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
#include "scene2.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Scene2::Scene2(void)
{
    add_background(":/images/10.11-astronomiya.jpg");

    item = get_item(":/images/computer.ico");
    item->setPos(400, 200);

    group = new QGraphicsItemGroup;
    for(int y=0; y<10; y++)
    {
        for(int x=0; x<10; x++)
        {
            QGraphicsPixmapItem *item_0 = get_item(":/images/computer.ico");
            item_0->setPos(x*35, y*33);
            group->addToGroup(item_0);
        }
    }

    addLine(0, 0, 100, 50, QPen(Qt::green));
    addLine(0, 0, 100, 100, QPen(Qt::green));
    addLine(0, 0, 100, 150, QPen(Qt::green));

    addItem(group);

    //---
#if 0
    QGraphicsItemGroup *test_group = get_text_rect(1234567890);
#else
    QGraphicsItemGroup *test_group = get_text_rect(1234567890,
                                                   QColor(Qt::red),
                                                   QPen(Qt::green));
#endif

    test_group->setPos(800, 500);
    test_group->setRotation(45);
    test_group->setData(0x1000, 5);

    addItem(test_group);
    //---
    QList<QGraphicsItem *>list_items = items();
    qDebug() << "count =" << list_items.count();
#if 1
    foreach (QGraphicsItem *item, list_items)
    {
        QVariant var = item->data(0x1000);
        if(var.toInt() != 0)
        {
            qDebug() << "var =" << var.toInt();
            //delete item;
        }
    }
    //list_items = items();
    //qDebug() << "count =" << list_items.count();
#endif
    //---

    tux = get_item(":/images/Tux.png");
    tux->setPos(WIDTH/2  - tux->boundingRect().width()/2,
                HEIGHT/2 - tux->boundingRect().height()/2);

    min_x = 0;
    max_x = WIDTH - group->boundingRect().width();
    min_y = 0;
    max_y = HEIGHT - group->boundingRect().height();

    pos_x = 100;
    pos_y = 100;
}
//--------------------------------------------------------------------------------
Scene2::~Scene2()
{
    if(item)
    {
        delete item;
    }
    if(tux)
    {
        delete tux;
    }
}
//--------------------------------------------------------------------------------
void Scene2::update(void)
{
    qreal rot = tux->rotation() + 0.5;
    tux->setRotation(rot);

    move();

#if 0
    qreal scale = tux->scale() - 0.0001;
    if(scale > 1) scale = 1;
    if(scale < 0) scale = 1;
    tux->setScale(scale);
#endif
}
//--------------------------------------------------------------------------------
void Scene2::move(void)
{
    if(flag_up)
    {
        if(pos_y > min_y)
        {
            pos_y--;
        }
    }
    if(flag_down)
    {
        if(pos_y < max_y)
        {
            pos_y++;
        }
    }
    if(flag_left)
    {
        if(pos_x > min_x)
        {
            pos_x--;
        }
    }
    if(flag_right)
    {
        if(pos_x < max_x)
        {
            pos_x++;
        }
    }
    //---
#if 1
    if(group)
    {
        group->setPos(pos_x, pos_y);
    }
#else
    if(item)
    {
        item->setPos(pos_x, pos_y);
    }
#endif
}
//--------------------------------------------------------------------------------
