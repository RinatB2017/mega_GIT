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
    add_item(0, 0, SPACE_ID);   add_item(1, 0, SPACE_ID);   add_item(2, 0, SPACE_ID);
    add_item(0, 1, SPACE_ID);   add_item(1, 1, SPACE_ID);   add_item(2, 1, SPACE_ID);
    add_item(0, 2, SPACE_ID);   add_item(1, 2, SPACE_ID);   add_item(2, 2, SPACE_ID);
    setLayout(grid_map);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
    if(x>=MAX_X) return false;
    if(y>=MAX_Y) return false;

    id_map[x][y] = id;

    QPixmap pixmap;
    pixmap.load(QString(":/images/%1.png").arg(id));

    QLabel *label = dynamic_cast<QLabel*>(grid_map->itemAtPosition(y, x)->widget());
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
    if(x>=MAX_X) return false;
    if(y>=MAX_Y) return false;

    *id = id_map[x][y];

    return true;
}
//--------------------------------------------------------------------------------
void MiniMap::updateText(void)
{

}
//--------------------------------------------------------------------------------
