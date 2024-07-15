/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include "color_widget.hpp"
//--------------------------------------------------------------------------------
Color_widget::Color_widget(QWidget *parent) :
    QWidget(parent)
{

}
//--------------------------------------------------------------------------------
void Color_widget::set_color(QColor color)
{
    current_color = color;
    update();
}
//--------------------------------------------------------------------------------
QColor Color_widget::get_color(void)
{
    return current_color;
}
//--------------------------------------------------------------------------------
void Color_widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(current_color));
    painter.fillRect(0, 0, width(), height(), current_color);
}
//--------------------------------------------------------------------------------
