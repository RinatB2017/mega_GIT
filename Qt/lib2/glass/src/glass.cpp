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
#include <QEvent>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QtMath>
//--------------------------------------------------------------------------------
#include "glass.hpp"
//--------------------------------------------------------------------------------
Glass::Glass(QWidget *parent) :
    QWidget(parent)
{
    center.setX(width() / 2);
    center.setY(height() / 2);
    radius = 0;

    installEventFilter(this);

    qDebug() << "constructor";
}
//--------------------------------------------------------------------------------
Glass::~Glass()
{
    qDebug() << "destructor";
}
//--------------------------------------------------------------------------------
void Glass::install(QWidget* widget)
{
    remove();
    setParent(widget);
    show();

    // Пошлем событие сами себе, чтобы стекло и информационный блок правильно
    // разместились на виджете
    QEvent event(QEvent::Resize);
    eventFilter(0, &event);
}
//--------------------------------------------------------------------------------
void Glass::remove(void)
{
    // Если стекло было установлено, то удаляем его
    if (parentWidget() != 0)
    {
        // Перестаем отлавливать события на низлежащем виджете
        parentWidget()->removeEventFilter(this);

        // Скрываем все компоненты стекла
        hide();
        setParent(0);
    }
}
//--------------------------------------------------------------------------------
void Glass::test(void)
{

}
//--------------------------------------------------------------------------------
bool Glass::eventFilter(QObject* object, QEvent* event)
{
    Q_UNUSED(object);
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(event->type() == QEvent::MouseButtonPress)
    {
        center= mouseEvent->pos();
        return true;
    }
    //---
    if(event->type() == QEvent::MouseMove)
    {
        QPoint end = mouseEvent->pos();
        radius = qSqrt((end.x() - center.x())*(end.x() - center.x()) + (end.y() - center.y())*(end.y() - center.y()));
        repaint();
        return true;
    }
    //---
    if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize))
    {
        resize(parentWidget()->size());
        move(0,0);
        qDebug() << "resize" << parentWidget()->size();
        return true;
    }
    //---
    return false;
}
//--------------------------------------------------------------------------------
void Glass::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    painter.drawEllipse(center, radius, radius);

    int x1 = center.x() - radius;
    int y1 = center.y() - radius;
    int x2 = center.x() + radius;
    int y2 = center.y() + radius;

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    painter.drawRect(x1, y1, (x2 - x1), (y2 - y1));
    painter.end();
}
//--------------------------------------------------------------------------------
