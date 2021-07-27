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
#include "cell.hpp"
//--------------------------------------------------------------------------------
Cell::Cell(CELL cell,
           int s_cell,
           QWidget *parent)
    : QWidget(parent)
{
    this->cell = cell;
    this->c_width = s_cell;
    this->c_height = s_cell;
    this->c_border = s_cell / 10;

    setFixedSize(c_width, c_height);
}
//--------------------------------------------------------------------------------
uint8_t Cell::get_value(void)
{
    return cell.value;
}
//--------------------------------------------------------------------------------
void Cell::set_value(uint8_t value)
{
    cell.value = value;
    update();
}
//--------------------------------------------------------------------------------
void Cell::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(color_cell);
    painter.setBrush(QBrush(color_cell));
    painter.drawRect(0, 0, width(), height());

    painter.setPen(color_border);
    painter.setBrush(QBrush(color_border));

    if(cell.bites.b_up)
    {
        painter.drawRect(0, 0, c_width, c_border);
    }

    if(cell.bites.b_down)
    {
        painter.drawRect(0, c_height - c_border, c_width, c_border);
    }

    if(cell.bites.b_left)
    {
        painter.drawRect(0, 0, c_border, c_height);
    }

    if(cell.bites.b_right)
    {
        painter.drawRect(c_width - c_border, 0, c_border, c_height);
    }
}
//--------------------------------------------------------------------------------
