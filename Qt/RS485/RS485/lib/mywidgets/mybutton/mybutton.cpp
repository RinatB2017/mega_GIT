/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QPointF>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>
#include <QRect>

#include "mybutton.hpp"
//--------------------------------------------------------------------------------
MyButton::MyButton(int width,
                   int height,
                   int b_width,
                   const QString &caption,
                   QWidget *parent) :
    QWidget(parent),
    caption(caption)
{
    setFixedSize(width, height);
    if(b_width > 3)
        border_width = b_width;
    else
        border_width = 3;
    state = false;
}
//--------------------------------------------------------------------------------
void MyButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int x = 0;
    int y = 0;
    int w = width()-1;
    int h = height()-1;
    int bw = border_width;

    if(state)
    {
        painter.drawLine(x+w-bw, y+h-bw, x+w, y+h);
        for(int n=0; n<bw; n++)
        {
            painter.drawLine(x+n, y+n, x+n, y+h-n); // левая
            painter.drawLine(x+n, y+n, x+w-n, y+n); // верхняя
        }
    }
    else
    {
        painter.drawLine(x, y, x+bw, y+bw);
        for(int n=0; n<bw; n++)
        {
            painter.drawLine(x+w-n, y+n, x+w-n, y+h-n); // правая
            painter.drawLine(x+n, y+h-n, x+w-n, y+h-n); // нижняя
        }
    }
    painter.drawRect(x, y, w, h);
    painter.drawRect(x+bw, y+bw, w-2*bw, h-2*bw);

    QRect rect(x, y, w, h);
    painter.drawText(rect, Qt::AlignCenter, caption);
}
//--------------------------------------------------------------------------------
void MyButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        state = true;
        repaint();
        emit clicked();
    }
}
//--------------------------------------------------------------------------------
void MyButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        state = false;
        repaint();
    }
}
//--------------------------------------------------------------------------------
