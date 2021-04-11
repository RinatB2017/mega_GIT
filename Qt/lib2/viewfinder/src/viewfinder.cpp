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
#include "mainwindow.hpp"
#include "viewfinder.hpp"
//--------------------------------------------------------------------------------
ViewFinder::ViewFinder(QWidget *parent) :
    QWidget(parent)
{
    center.setX(width() / 2);
    center.setY(height() / 2);

    installEventFilter(this);
}
//--------------------------------------------------------------------------------
ViewFinder::~ViewFinder()
{

}
//--------------------------------------------------------------------------------
void ViewFinder::install(QWidget* widget)
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
void ViewFinder::remove(void)
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
void ViewFinder::test(void)
{

}
//--------------------------------------------------------------------------------
bool ViewFinder::eventFilter(QObject* , QEvent* event)
{
    if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize))
    {
        resize(parentWidget()->size());
        move(0,0);
        qDebug() << "######### resize" << parentWidget()->size();
        return true;
    }

    return false;
}
//--------------------------------------------------------------------------------
void ViewFinder::paintEvent(QPaintEvent *)
{
    //FIXME дурь, конечно. Но eventFilter как то странно отрабатывает
    resize(parentWidget()->size());
    move(0,0);

    int w = parentWidget()->size().width();
    int h = parentWidget()->size().height();

    int x1 = w / 3;
    int x2 = x1 * 2;

    int y1 = h / 3;
    int y2 = y1 * 2;

    int marker_w = 20;
    int marker_h = 20;

    QPainter painter;
    painter.begin(this);

    painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine));

    //painter.drawRect(x1, y1, (x2 - x1), (y2 - y1));

    painter.drawRect(x1 - marker_w / 2,
                     y1 - marker_h / 2,
                     marker_w,
                     marker_h);
    painter.drawRect(x2 - marker_w / 2,
                     y1 - marker_h / 2,
                     marker_w,
                     marker_h);
    painter.drawRect(x1 - marker_w / 2,
                     y2 - marker_h / 2,
                     marker_w,
                     marker_h);
    painter.drawRect(x2 - marker_w / 2,
                     y2 - marker_h / 2,
                     marker_w,
                     marker_h);

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine));

    painter.drawLine(x1, 0,  x1, h);
    painter.drawLine(x2, 0,  x2, h);
    painter.drawLine(0,  y1, w,  y1);
    painter.drawLine(0,  y2, w,  y2);

    painter.end();
}
//--------------------------------------------------------------------------------
