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
#include "creator_images.hpp"
//--------------------------------------------------------------------------------
Creator_images::Creator_images(QObject *parent)
    : QObject{parent}
{
    pen.setColor(pen_color);
    pen.setWidth(pen_width);

    brush.setColor(brush_color);
}
//--------------------------------------------------------------------------------
void Creator_images::set_background_color(QColor color)
{
    background_color = color;
}
//--------------------------------------------------------------------------------
void Creator_images::set_pen_color(QColor color)
{
    pen_color = color;
    pen.setColor(pen_color);
}
//--------------------------------------------------------------------------------
void Creator_images::set_brush_color(QColor color)
{
    brush_color = color;
    brush.setColor(brush_color);
}
//--------------------------------------------------------------------------------
void Creator_images::set_pen_width(int w)
{
    pen_width = w;
    pen.setWidth(pen_width);
}
//--------------------------------------------------------------------------------
QImage Creator_images::create_rectangle(int w, int h)
{
    QImage image = QImage(w, h, QImage::Format_ARGB32);
    image.fill(background_color);

    QPen pen;

    QPainter p(&image);
    p.setPen(pen);
    p.drawRect(0, 0, w, h);

    //image.save(QString("/dev/shm/%1.png").arg(Q_FUNC_INFO));

    return image;
}
//--------------------------------------------------------------------------------
QImage Creator_images::create_circle(int w, int h)
{
    QImage image = QImage(w, h, QImage::Format_ARGB32);
    image.fill(background_color);

    QPainter p(&image);
    p.setPen(pen);
    p.drawEllipse(0, 0, w, h);

    //image.save(QString("/dev/shm/%1.png").arg(Q_FUNC_INFO));

    return image;
}
//--------------------------------------------------------------------------------
QImage Creator_images::create_rhombus(int w, int h)
{
    QImage image = QImage(w, h, QImage::Format_ARGB32);
    image.fill(background_color);

    QPainter p(&image);
    p.setPen(pen);
    p.drawLine(0,   h/2, w/2, h);
    p.drawLine(w/2, h,   w,   h/2);
    p.drawLine(w,   h/2, w/2, 0);
    p.drawLine(w/2, 0,   0,   h/2);

    //image.save(QString("/dev/shm/%1.png").arg(Q_FUNC_INFO));

    return image;
}
//--------------------------------------------------------------------------------
QImage Creator_images::create_text(int w, int h, const QString &text)
{
    QImage image = QImage(w, h, QImage::Format_ARGB32);
    image.fill(background_color);

    QFont font("Liberation Mono", 32);

    QPainter p(&image);
    p.setPen(pen);
    p.setFont(font);
    p.drawText(w/2, h/2, text);

    //image.save(QString("/dev/shm/%1.png").arg(Q_FUNC_INFO));

    return image;
}
//--------------------------------------------------------------------------------
QImage Creator_images::create_triangle(int w, int h, SHAPE shape)
{
    QImage image = QImage(w, h, QImage::Format_ARGB32);
    image.fill(background_color);

    QPainter p(&image);

    const int cnt = 3;
    QPointF points[cnt];

    switch (shape)
    {
    case SHAPE::TRIANGLE_LEFT:
        points[0] = QPointF(0,    h/2);
        points[1] = QPointF(w-1,  h-1);
        points[2] = QPointF(w-1,  0);
        break;

    case SHAPE::TRIANGLE_RIGHT:
        points[0] = QPointF(0,   0);
        points[1] = QPointF(0,   h-1);
        points[2] = QPointF(w-1, h/2);
        break;

    case SHAPE::TRIANGLE_DOWN:
        points[0] = QPointF(0,   h-1);
        points[1] = QPointF(w-1, h-1);
        points[2] = QPointF(w/2, 0);
        break;

    case SHAPE::TRIANGLE_UP:
        points[0] = QPointF(0,   0);
        points[1] = QPointF(w/2, h-1);
        points[2] = QPointF(w-1, 0);
        break;
    }

    p.setPen(pen);
    p.setBrush(brush);
    p.drawPolygon(points, cnt);

    return image;
}
//--------------------------------------------------------------------------------
