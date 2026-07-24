/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
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
#include "draw_widget.hpp"
//--------------------------------------------------------------------------------
Draw_widget::Draw_widget(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Draw_widget::~Draw_widget()
{

}
//--------------------------------------------------------------------------------
void Draw_widget::init(void)
{

}
//--------------------------------------------------------------------------------
void Draw_widget::paintEvent(QPaintEvent *)
{
    QPainter painter;

    painter.begin(this);
    painter.setPen(QPen(QColor(210, 210, 210), 1));

    painter.drawLine(0, 0,          width(), height());
    painter.drawLine(0, height(),   width(), 0);

    painter.end();
}
//--------------------------------------------------------------------------------
void Draw_widget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}
//--------------------------------------------------------------------------------
