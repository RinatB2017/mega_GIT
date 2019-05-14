/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "tile_widget.hpp"
//--------------------------------------------------------------------------------
Tile_widget::Tile_widget(int type, QWidget *parent)
    :QWidget(parent)
{
    this->type = type;

    setFixedSize(256, 256);
}
//--------------------------------------------------------------------------------
void Tile_widget::draw_horizontal(QPainter *painter)
{
#if 0
    QLinearGradient *shader_up = new QLinearGradient(QPointF(width(), 0),
                                                     QPointF(width(), height() / 2.0));
    QLinearGradient *shader_down = new QLinearGradient(QPointF(width(), height()),
                                                       QPointF(width(), height() / 2.0));

    shader_up->setColorAt(0, QColor(Qt::white));
    shader_up->setColorAt(1, QColor(Qt::black));

    shader_down->setColorAt(0,   QColor(Qt::white));
    shader_down->setColorAt(1,   QColor(Qt::black));

    painter->fillRect(0, 0,              width(),    height() / 2,   *shader_up);
    painter->fillRect(0, height() / 2,   width(),    height() / 2,   *shader_down);
#endif

#if 1
    int h_up = height() / 3;
    int h_mid = height() / 3;

    painter->fillRect(0, h_up, width(), h_mid, QBrush(Qt::black));
#endif
}
//--------------------------------------------------------------------------------
void Tile_widget::draw_vertical(QPainter *painter)
{
#if 0
    QLinearGradient *shader_left = new QLinearGradient(QPointF(0,           height() / 2),
                                                       QPointF(width() / 2, height() / 2));
    QLinearGradient *shader_right = new QLinearGradient(QPointF(width() / 2,    height() / 2),
                                                        QPointF(width(),        height() / 2));

    shader_left->setColorAt(0, QColor(Qt::white));
    shader_left->setColorAt(1, QColor(Qt::black));

    shader_right->setColorAt(0,   QColor(Qt::black));
    shader_right->setColorAt(1,   QColor(Qt::white));

    painter->fillRect(0,            0,  width() / 2,    height(),   *shader_left);
    painter->fillRect(width() / 2,  0,  width() / 2,    height(),   *shader_right);
#endif

#if 1
    int w_left = width() / 3;
    int w_mid = width() / 3;

    painter->fillRect(w_left, 0, w_mid, height(), QBrush(Qt::black));
#endif
}
//--------------------------------------------------------------------------------
void Tile_widget::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);

    switch (type)
    {
    case Horizontal:
        draw_horizontal(painter);
        break;

    case Vertical:
        draw_vertical(painter);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
