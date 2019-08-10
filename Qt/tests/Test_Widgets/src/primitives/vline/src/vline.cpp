/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
//--------------------------------------------------------------------------------
#include "vline.hpp"
//--------------------------------------------------------------------------------
VLine::VLine(unsigned int len,
             QWidget *parent) :
    QWidget(parent),
    len(len)
{
    setFixedWidth(3);
    setFixedHeight(len);
    show();
}
//--------------------------------------------------------------------------------
VLine::~VLine()
{

}
//--------------------------------------------------------------------------------
void VLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();
    painter.drawLine(0, 0, 0, len);
    painter.restore();
}
//--------------------------------------------------------------------------------
void VLine::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        raise();
        lastPoint = event->pos();
        b_move = true;
    }
}
//--------------------------------------------------------------------------------
void VLine::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - lastPoint));
    }
}
//--------------------------------------------------------------------------------
void VLine::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && b_move)
    {
        b_move = false;
    }
}
//--------------------------------------------------------------------------------
