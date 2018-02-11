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
#include <QGridLayout>
#include <QVariant>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "minimap.hpp"
//--------------------------------------------------------------------------------
MiniMap::MiniMap(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MiniMap::~MiniMap()
{

}
//--------------------------------------------------------------------------------
void MiniMap::init(void)
{
    grid_map = new QGridLayout(this);
    grid_map->setSpacing(0);
    grid_map->setMargin(0);
    setLayout(grid_map);
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
bool MiniMap::new_map(int new_max_x, int new_max_y)
{
    if(new_max_x < MIN_WIDTH)   return false;
    if(new_max_y < MIN_HEIGHT)  return false;
    if(new_max_x > MAX_WIDTH)   return false;
    if(new_max_y > MAX_HEIGHT)  return false;

    // удаляем все объекты
    QLayoutItem *child = 0;
    while ((child = grid_map->takeAt(0)) != 0)
    {
        grid_map->removeItem(child);
        if (child->widget())
            delete child->widget();
        if (child->layout())
            delete child->layout();
        delete child;
    }
    //---
    max_x = new_max_x;
    max_y = new_max_y;
    //---
    for(int x=0; x<max_x; x++)
    {
        add_item(x, 0, WALL_ID);
        add_item(x, max_y-1, WALL_ID);
    }
    for(int y=1; y<(max_y-1); y++)
    {
        add_item(0, y, WALL_ID);
        add_item(max_x-1, y, WALL_ID);
    }

    for(int y=1; y<max_y-1; y++)
    {
        for(int x=1; x<max_x-1; x++)
        {
            add_item(x, y, SPACE_ID);
        }
    }
    setFixedSize(sizeHint());
    return true;
}
//--------------------------------------------------------------------------------
bool MiniMap::add_item(int x, int y, int id)
{
    if(x < 0)   return false;
    if(y < 0)   return false;
    if(x >= MAX_WIDTH)   return false;
    if(y >= MAX_HEIGHT)  return false;

    QPixmap pixmap;
    QLabel *label = 0;
    bool ok = false;

    switch (id)
    {
    case PLAYER_ID:
    case WALL_ID:
    case SPACE_ID:
    case START_ID:
    case EXIT_ID:
        pixmap.load(QString(":/images/%1.png").arg(id));

        label = new QLabel();
        label->setProperty(PROPERTY_ID, id);
        label->setProperty(PROPERTY_X, x);
        label->setProperty(PROPERTY_Y, y);
        label->installEventFilter(this);
        label->setPixmap(pixmap);

        id_map[x][y] = id;
        grid_map->addWidget(label, y, x);
        ok = true;
        break;

    default:
        emit error(QString("item %1 is not valid").arg(id));
        break;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MiniMap::set(int x, int y, int id)
{
    if(x<0) return false;
    if(y<0) return false;
    if(x>max_x) return false;
    if(y>max_y) return false;

    id_map[x][y] = id;

    QPixmap pixmap;
    pixmap.load(QString(":/images/%1.png").arg(id));

    QLayoutItem *item = grid_map->itemAtPosition(y, x);
    if(item == nullptr)
    {
        return false;
    }
    QLabel *label = dynamic_cast<QLabel*>(item->widget());
    if(label)
    {
        label->setPixmap(pixmap);
    }
    else
    {
        emit error(QString("ERROR: set(%1, %2, %3)")
                   .arg(x)
                   .arg(y)
                   .arg(id));
    }

    return true;
}
//--------------------------------------------------------------------------------
bool MiniMap::get(int x, int y, int *id)
{
    if(x<0) return false;
    if(y<0) return false;
    if(x>=max_x) return false;
    if(y>=max_y) return false;

    *id = id_map[x][y];

    return true;
}
//--------------------------------------------------------------------------------
void MiniMap::updateText(void)
{

}
//--------------------------------------------------------------------------------
