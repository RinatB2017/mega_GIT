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
#include <QMouseEvent>
#include <QPainter>
//--------------------------------------------------------------------------------
#include "button.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Button::Button(int width,
               int height,
               QWidget *parent) :
    QToolButton(parent)
{
    color_ON  = Qt::red;
    color_OFF = Qt::black;
    setFixedSize(width, height);
}
//--------------------------------------------------------------------------------
Button::~Button()
{

}
//--------------------------------------------------------------------------------
void Button::set_color(QColor value_ON, QColor value_OFF)
{
    color_ON  = value_ON;
    color_OFF = value_OFF;

    update();
}
//--------------------------------------------------------------------------------
void Button::nextCheckState(void)
{
    setChecked(!isChecked());

    if(isChecked())
    {
        color = color_ON;
    }
    else
    {
        color = color_OFF;
    }
    emit send(isChecked());
    update();
}
//--------------------------------------------------------------------------------
void Button::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    painter.setPen(QPen(QBrush(color), 1, Qt::SolidLine));
    painter.setBrush(QBrush(color));
    painter.drawEllipse(QPointF(width()/2, height()/2), width()/2 - 1, height()/2 - 1);

    painter.end();
}
//--------------------------------------------------------------------------------
