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
#include "minimap.hpp"
#include "world.hpp"
#include "cell.hpp"
//--------------------------------------------------------------------------------
Minimap::Minimap(QWidget *parent)
    : MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Minimap::init(void)
{
    QGridLayout *grid = new QGridLayout();
    //grid->setMargin(0);
    //grid->setSpacing(0);

    CELL cell;
    uint8_t value = 0;
    for(int y=0; y<4; y++)
    {
        for(int x=0; x<4; x++)
        {
            cell.value = value;
            cw[x][y] = new Cell(cell, CELL_SIZE, this);
            grid->addWidget(cw[x][y], y, x);
            value++;
        }
    }
    setLayout(grid);
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void Minimap::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        int x = event->x() / CELL_SIZE;
        int y = event->y() / CELL_SIZE;

        Cell *cell = cw[x][y];
        if(cell)
        {
            uint8_t value = cell->get_value();
            emit cell_click(value);

            emit info(QString("click: %1:%2")
                      .arg(x)
                      .arg(y));
        }
        else
        {
            emit error("Cell is NULL");
        }
    }
}
//--------------------------------------------------------------------------------
void Minimap::updateText(void)
{

}
//--------------------------------------------------------------------------------
