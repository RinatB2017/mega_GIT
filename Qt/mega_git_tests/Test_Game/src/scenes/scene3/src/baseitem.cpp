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
#include "baseitem.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
BaseItem::BaseItem(void)
{
    angle = rand() % 180 + 1;
}
//--------------------------------------------------------------------------------
void BaseItem::setPixmap(const QPixmap &pixmap)
{
    w_item = pixmap.width();
    h_item = pixmap.height();
    setTransformOriginPoint(pixmap.width()/2,
                            pixmap.height()/2);
    QGraphicsPixmapItem::setPixmap(pixmap);
}
//--------------------------------------------------------------------------------
void BaseItem::setPos(qreal ax, qreal ay)
{
    x = ax;
    y = ay;
    QGraphicsPixmapItem::setPos(ax, ay);
}
//--------------------------------------------------------------------------------
void BaseItem::move(void)
{
    if((x<0) || (y<0) || (x>(WIDTH-w_item)) || (y>(HEIGHT-h_item)))
    {
        angle -= 90;
        if(angle < 0)   angle += 360;
        if(angle > 360) angle -= 360;
    }

    calc_line(x, y, angle, 1, &x, &y);
    setPos(x, y);
}
//--------------------------------------------------------------------------------
void BaseItem::calc_line(qreal center_x,
                         qreal center_y,
                         qreal angle,
                         qreal radius,
                         qreal *end_x,
                         qreal *end_y)
{
    Q_ASSERT(end_x);
    Q_ASSERT(end_y);

    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    *end_x = center_x + B;
    *end_y = center_y + C;
}
//--------------------------------------------------------------------------------
