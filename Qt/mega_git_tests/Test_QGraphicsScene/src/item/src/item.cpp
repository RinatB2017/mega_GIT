/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "item.hpp"
//--------------------------------------------------------------------------------
Item::Item(qreal x, qreal y, qreal w, qreal h,
           const QPen &pen, const QBrush &brush,
           QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);

    setRect(x, y, w, h);
    setPen(pen);
    setBrush(brush);

    // важно для обработчиков
    setAcceptHoverEvents(true);
}
//--------------------------------------------------------------------------------
Item::~Item()
{

}
//--------------------------------------------------------------------------------
void Item::set_name(const QString &name)
{
    obj_name = name;
}
//--------------------------------------------------------------------------------
//bool Item::sceneEvent(QEvent *event)
//{
//    qDebug() << obj_name;

//    QGraphicsItem::sceneEvent(event);
//    return true;
//}
//--------------------------------------------------------------------------------
void Item::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "enter" << obj_name;
    QGraphicsItem::hoverEnterEvent(event);
}
//--------------------------------------------------------------------------------
void Item::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "move" << obj_name;
    QGraphicsItem::hoverMoveEvent(event);
}
//--------------------------------------------------------------------------------
void Item::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "leave" << obj_name;
    QGraphicsItem::hoverLeaveEvent(event);
}
//--------------------------------------------------------------------------------
