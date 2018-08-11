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
#include <QPainter>
#include "mesh_led.hpp"
//--------------------------------------------------------------------------------
Mesh_Led::Mesh_Led(QWidget *parent) :
    QWidget(parent)
{
    this->color_off = Qt::black;
    this->color_on = Qt::red;

    setFixedSize(height(), height());

    current_color = color_off;
    update();
}
//--------------------------------------------------------------------------------
void Mesh_Led::setState(bool state)
{
    if(state)
    {
        current_color = color_on;
    }
    else
    {
        current_color = color_off;
    }
    update();
}
//--------------------------------------------------------------------------------
void Mesh_Led::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), Qt::transparent);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(current_color));
    painter.drawEllipse(QPointF(width() / 2.0, height() / 2.0), width() / 2.0 - 1.0, height() / 2.0 - 1.0);
}
//--------------------------------------------------------------------------------
