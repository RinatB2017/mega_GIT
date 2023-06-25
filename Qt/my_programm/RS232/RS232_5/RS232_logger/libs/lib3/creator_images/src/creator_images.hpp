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
#ifndef CREATOR_IMAGES_HPP
#define CREATOR_IMAGES_HPP
//--------------------------------------------------------------------------------
#include <QPainter>
#include <QObject>
#include <QImage>
#include <QFont>
#include <QPen>
//--------------------------------------------------------------------------------
enum SHAPE {
    TRIANGLE_LEFT = 0,
    TRIANGLE_RIGHT,
    TRIANGLE_DOWN,
    TRIANGLE_UP
};
//--------------------------------------------------------------------------------
class Creator_images : public QObject
{
    Q_OBJECT
public:
    explicit Creator_images(QObject *parent = nullptr);

    void set_background_color(QColor color);
    void set_pen_color(QColor color);
    void set_brush_color(QColor color);
    void set_pen_width(int w);

    QImage create_rectangle(int w, int h);
    QImage create_circle(int w, int h);
    QImage create_rhombus(int w, int h);
    QImage create_text(int w, int h, const QString &text);
    QImage create_triangle(int w, int h, SHAPE shape);

private:
    QColor background_color = QColor(Qt::white);
    QColor pen_color = QColor(Qt::black);
    QColor brush_color = QColor(Qt::transparent);
    int pen_width = 3;

    QBrush brush;
    QPen pen;
};
//--------------------------------------------------------------------------------
#endif // CREATOR_IMAGES_HPP
