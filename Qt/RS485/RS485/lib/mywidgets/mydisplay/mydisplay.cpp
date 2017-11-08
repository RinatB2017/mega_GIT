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
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QDebug>
#include <QBrush>
#include <QEvent>
#include <QRect>

#include "mydisplay.hpp"
//--------------------------------------------------------------------------------
MyDisplay::MyDisplay(QWidget *parent) :
    QWidget(parent)
{
    border_width = 4;
    display_width = 24;
    display_height = 2 * (display_width -  2 * border_width) + 3 *border_width;

    a = false;
    b = false;
    c = false;
    d = false;
    e = false;
    f = false;
    g = false;
    h = false;

    color = Qt::red;
    background_color = Qt::black;

    setFixedSize(display_width, display_height);
}
//--------------------------------------------------------------------------------
void MyDisplay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int x = 1;
    int y = 1;
    int w = width()-2;

    int hs_w = w - (2 * border_width);
    int hs_h = border_width;

    int vs_w = border_width;
    int vs_h = w - (2 * border_width);

    painter.fillRect(0, 0, width(), height(), QBrush(background_color));

    if(a)
    {
#if 1
        QPoint points[6] = {
            QPoint(x + (vs_w / 2), y + (hs_h / 2)),
            QPoint(x + vs_w, y),
            QPoint(x + vs_w + hs_w, y),
            QPoint(x + vs_w + hs_w + (vs_w / 2), y + (vs_w / 2)),
            QPoint(x + vs_w + hs_w, y + hs_h),
            QPoint(x + vs_w, y + hs_h)
        };
        painter.save();
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawPolygon(points, 6);
        painter.restore();
#else
        painter.drawRect(x + vs_w,
                         y,
                         hs_w,
                         hs_h);
#endif
    }
    if(b)
    {
#if 1
        QPoint points[6] = {
            QPoint(x + vs_w + hs_w + (vs_w / 2), y + (hs_h / 2)),
            QPoint(x + w, y + hs_h),
            QPoint(x + w, y + hs_h + vs_h),
            QPoint(x + vs_w + hs_w + (vs_w / 2), y + hs_h + vs_h + (vs_w / 2)),
            QPoint(x + vs_w + hs_w, y + hs_h + vs_h),
            QPoint(x + vs_w + hs_w, y + hs_h)
        };
        painter.save();
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawPolygon(points, 6);
        painter.restore();
#else
        painter.drawRect(x + w - vs_w,
                         y + hs_h,
                         vs_w,
                         vs_h);
#endif
    }
    if(c)
    {
#if 1
        QPoint points[6] = {
            QPoint(x + vs_w + hs_w + (vs_w / 2), y + (hs_h + vs_h) + (hs_h / 2)),
            QPoint(x + w, y + (hs_h + vs_h) + hs_h),
            QPoint(x + w, y + (hs_h + vs_h) + hs_h + vs_h),
            QPoint(x + vs_w + hs_w + (vs_w / 2), y + (hs_h + vs_h) + hs_h + vs_h + (vs_w / 2)),
            QPoint(x + vs_w + hs_w, y + (hs_h + vs_h) + hs_h + vs_h),
            QPoint(x + vs_w + hs_w, y + (hs_h + vs_h) + hs_h)
        };
        painter.save();
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawPolygon(points, 6);
        painter.restore();
#else
        painter.drawRect(x + w - vs_w,
                         y + (2 * hs_h) + vs_h,
                         vs_w,
                         vs_h);
#endif
    }
    if(d)
    {
#if 1
        QPoint points[6] = {
            QPoint(x + (vs_w / 2), y + (2 * hs_h + 2 * vs_h) + (hs_h / 2)),
            QPoint(x + vs_w, y + (2 * hs_h + 2 * vs_h)),
            QPoint(x + vs_w + hs_w, y + (2 * hs_h + 2 * vs_h)),
            QPoint(x + vs_w + hs_w + (vs_w / 2), y + (2 * hs_h + 2 * vs_h) + (vs_w / 2)),
            QPoint(x + vs_w + hs_w, y + (2 * hs_h + 2 * vs_h) + hs_h),
            QPoint(x + vs_w, y + (2 * hs_h + 2 * vs_h) + hs_h)
        };
        painter.save();
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawPolygon(points, 6);
        painter.restore();
#else
        painter.drawRect(x + vs_w,
                         y + (2 * hs_h) + (2 * vs_h),
                         hs_w,
                         hs_h);
#endif
    }
    if(e)
    {
#if 1
        QPoint points[6] = {
            QPoint(x + (vs_w / 2), y + (hs_h + vs_h) + (hs_h / 2)),
            QPoint(x + vs_w, y + (hs_h + vs_h) + hs_h),
            QPoint(x + vs_w, y + (hs_h + vs_h) + hs_h + vs_h),
            QPoint(x + (vs_w / 2), y + (hs_h + vs_h) + hs_h + vs_h + (vs_w / 2)),
            QPoint(x, y + (hs_h + vs_h) + hs_h + vs_h),
            QPoint(x, y + (hs_h + vs_h) + hs_h)
        };
        painter.save();
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawPolygon(points, 6);
        painter.restore();
#else
        painter.drawRect(x,
                         y + (2 * hs_h) + vs_h,
                         vs_w,
                         vs_h);
#endif
    }
    if(f)
    {
#if 1
        QPoint points[6] = {
            QPoint(x + (vs_w / 2), y + (hs_h / 2)),
            QPoint(x + vs_w, y + hs_h),
            QPoint(x + vs_w, y + hs_h + vs_h),
            QPoint(x + (vs_w / 2), y + hs_h + vs_h + (vs_w / 2)),
            QPoint(x, y + hs_h + vs_h),
            QPoint(x, y + hs_h)
        };
        painter.save();
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawPolygon(points, 6);
        painter.restore();
#else
        painter.drawRect(x,
                         y + hs_h,
                         vs_w,
                         vs_h);
#endif
    }
    if(g)
    {
#if 1
        QPoint points[6] = {
            QPoint(x + (vs_w / 2), y + (hs_h + vs_h) + (hs_h / 2)),
            QPoint(x + vs_w, y + (hs_h + vs_h)),
            QPoint(x + vs_w + hs_w, y + (hs_h + vs_h)),
            QPoint(x + vs_w + hs_w + (vs_w / 2), y + (hs_h + vs_h) + (vs_w / 2)),
            QPoint(x + vs_w + hs_w, y + (hs_h + vs_h) + hs_h),
            QPoint(x + vs_w, y + (hs_h + vs_h) + hs_h)
        };
        painter.save();
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawPolygon(points, 6);
        painter.restore();
#else
        painter.drawRect(x + hs_h,
                         y + hs_h + vs_h,
                         hs_w,
                         hs_h);
#endif
    }
    //painter.drawRect(0, 0, w, h-1);
}
//--------------------------------------------------------------------------------
bool MyDisplay::display(unsigned int value)
{
    draw(value);
    repaint();

    return true;
}
//--------------------------------------------------------------------------------
void MyDisplay::draw(unsigned int value)
{
    switch(value)
    {
    case 0:   a = true; b = true; c = true; d = true; e = true; f = true; g = false; break;
    case 1:   a = false; b = true; c = true; d = false; e = false; f = false; g = false; break;
    case 2:   a = true; b = true; c = false; d = true; e = true; f = false; g = true; break;
    case 3:   a = true; b = true; c = true; d = true; e = false; f = false; g = true; break;
    case 4:   a = false; b = true; c = true; d = false; e = false; f = true; g = true; break;
    case 5:   a = true; b = false; c = true; d = true; e = false; f = true; g = true; break;
    case 6:   a = true; b = false; c = true; d = true; e = true; f = true; g = true; break;
    case 7:   a = true; b = true; c = true; d = false; e = false; f = false; g = false; break;
    case 8:   a = true; b = true; c = true; d = true; e = true; f = true; g = true; break;
    case 9:   a = true; b = true; c = true; d = true; e = false; f = true; g = true; break;
    case 0xA: a = true; b = true; c = true; d = false; e = true; f = true; g = true; break;
    case 0xB: a = true; b = true; c = true; d = true; e = true; f = true; g = true; break;
    case 0xC: a = true; b = false; c = false; d = true; e = true; f = true; g = false; break;
    case 0xD: a = true; b = true; c = true; d = true; e = true; f = true; g = false; break;
    case 0xE: a = true; b = false; c = false; d = true; e = true; f = true; g = true; break;
    case 0xF: a = true; b = false; c = false; d = false; e = true; f = true; g = true; break;
    default:  a = false; b = false; c = false; d = false; e = false; f = false; g = false; break;
    }
}
//--------------------------------------------------------------------------------
void MyDisplay::set_width(unsigned int width)
{
    display_width = width;
    display_height = 2 * width;
}
//--------------------------------------------------------------------------------
void MyDisplay::set_height(unsigned int height)
{
    display_height = height;
    display_width = height / 2;
}
//--------------------------------------------------------------------------------
void MyDisplay::set_color(QColor color)
{
    this->color = color;
}
//--------------------------------------------------------------------------------
void MyDisplay::set_background_color(QColor color)
{
    this->background_color = color;
}
//--------------------------------------------------------------------------------
