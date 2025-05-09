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
#ifndef ITEM_HPP
#define ITEM_HPP
//--------------------------------------------------------------------------------
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QEvent>
#include <QDebug>
//--------------------------------------------------------------------------------
class Item : public QGraphicsRectItem
{
public:
    Item(qreal x, qreal y, qreal w, qreal h,
         const QPen &pen, const QBrush &brush,
         QGraphicsItem *parent = nullptr);
    ~Item();

    void set_name(const QString &name);

private:    
    QString obj_name = "no name";

protected:
    //bool sceneEvent(QEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};
//--------------------------------------------------------------------------------
#endif // ITEM_HPP
