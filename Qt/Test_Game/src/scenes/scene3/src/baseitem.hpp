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
#ifndef BASEITEM_HPP
#define BASEITEM_HPP
//--------------------------------------------------------------------------------
#include <QGraphicsPixmapItem>
#include <QtMath>
//--------------------------------------------------------------------------------
class BaseItem : public QGraphicsPixmapItem
{
public:
    BaseItem(void);
    void move(void);
    void setPos(qreal ax, qreal ay);
    void setPixmap(const QPixmap &pixmap);

private:
    qreal x = 0.0;
    qreal y = 0.0;
    qreal angle  = 0.0;
    qreal w_item = 0;
    qreal h_item = 0;

    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);
};
//--------------------------------------------------------------------------------
#endif // BASEITEM_HPP
