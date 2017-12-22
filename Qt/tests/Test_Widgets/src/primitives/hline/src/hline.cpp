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
#include <QPainter>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "hline.hpp"
//--------------------------------------------------------------------------------
HLine::HLine(unsigned int len,
             QWidget *parent) :
    QWidget(parent),
    len(len)
{
    setFixedHeight(3);
    setFixedWidth(len);
    show();
}
//--------------------------------------------------------------------------------
HLine::~HLine()
{

}
//--------------------------------------------------------------------------------
void HLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();
    painter.drawLine(0, 0, len, 0);
    painter.restore();
}
//--------------------------------------------------------------------------------
void HLine::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        raise();
        lastPoint = event->pos();
        b_move = true;
    }
}
//--------------------------------------------------------------------------------
void HLine::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - lastPoint));
    }
}
//--------------------------------------------------------------------------------
void HLine::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && b_move)
    {
        b_move = false;
    }
}
//--------------------------------------------------------------------------------
