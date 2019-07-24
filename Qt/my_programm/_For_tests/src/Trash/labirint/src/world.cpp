/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "world.hpp"
#include "cell.hpp"

#include "defines.hpp"
//--------------------------------------------------------------------------------
World::World(QWidget *parent)
    : MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void World::init(void)
{
    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);

    CELL cell;
    uint8_t value = 0;
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            cell.value = value;
            cw[x][y] = new Cell(cell, CELL_SIZE, this);
            grid->addWidget(cw[x][y], y, x);
            //value++;
        }
    }
    setLayout(grid);
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void World::cell_click(uint8_t value)
{
    this->value = value;
    //cw[x][y]->set_cell(value);
}
//--------------------------------------------------------------------------------
void World::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        int x = event->x() / CELL_SIZE;
        int y = event->y() / CELL_SIZE;

        cw[x][y]->set_value(value);

        emit info(QString("World click: %1:%2")
                  .arg(x)
                  .arg(y));
    }
}
//--------------------------------------------------------------------------------
void World::updateText(void)
{

}
//--------------------------------------------------------------------------------
