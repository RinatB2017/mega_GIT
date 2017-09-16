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
#include "test_glass.hpp"
//--------------------------------------------------------------------------------
Test_Glass::Test_Glass(QWidget *parent) :
    MyWidget(parent)
{
    qDebug() << QString("%1 %2").arg(width()).arg(height());

    qreal WIDTH = 399;
    qreal HEIGHT = 392;

    center_x = WIDTH / 2.0f;
    center_y = HEIGHT / 2.0f;
    center_r = WIDTH / 14.0f;
    led_r = WIDTH / 17.0f;
    min_r = center_r + led_r + 10.0f;
    max_r = WIDTH / 2.0f - 20.0f;
    min_angle = -30.0f;
    max_angle = 330.0f;
    inc_r = (int)((max_r - min_r) / 2.4f);

    for(int index=0; index<MAX_POINT; index++)
    {
        points[index].color = Qt::transparent;
    }

    installEventFilter(this);
}
//--------------------------------------------------------------------------------
Test_Glass::~Test_Glass()
{

}
//--------------------------------------------------------------------------------
void Test_Glass::install(QWidget* widget)
{
    setParent(widget);
    //show();

    // Пошлем событие сами себе, чтобы стекло правильно разместилось на виджете
    QEvent event(QEvent::Resize);
    eventFilter(0, &event);
}
//--------------------------------------------------------------------------------
void Test_Glass::test(void)
{

}
//--------------------------------------------------------------------------------
bool Test_Glass::eventFilter(QObject* object, QEvent* event)
{
    Q_UNUSED(object);
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(event->type() == QEvent::MouseButtonPress)
    {
        int x = mouseEvent->pos().x();
        int y = mouseEvent->pos().y();
        for(int index=0; index<MAX_POINT; index++)
        {
            qreal line = qSqrt((x-points[index].center.x())*(x-points[index].center.x()) +
                               (y-points[index].center.y())*(y-points[index].center.y()));
            if(line < points[index].radius)
            {
                //emit info(QString("%1 %2").arg(x).arg(y));
                emit info(QString("%1").arg(index));

                if(points[index].color == Qt::red)
                    points[index].color = Qt::green;
                else
                    points[index].color = Qt::red;

                repaint();
                return true;
            }
        }
        return true;
    }
    //---
    if(event->type() == QEvent::MouseMove)
    {
        repaint();
        return true;
    }
    //---
    if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize))
    {
        resize(parentWidget()->size());
        center_x = (qreal)width() / 2.0;
        center_y = (qreal)height() / 2.0;
        return true;
    }
    //---
    return false;
}
//--------------------------------------------------------------------------------
void Test_Glass::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    int index = 0;

    painter.setBrush(QBrush(points[index].color));
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    painter.drawEllipse(center_x - center_r,
                        center_y - center_r,
                        center_r*2,
                        center_r*2);

    points[index].center.setX(center_x);
    points[index].center.setY(center_y);
    points[index].radius=center_r;
    index++;

    qreal angle = min_angle;
    while(angle < max_angle)
    {
        for(int n=0; n<3; n++)
        {
            calc_line(center_x,
                      center_y,
                      angle,
                      inc_r * (n + 1),
                      &temp_x,
                      &temp_y);
            painter.setBrush(QBrush(points[index].color));
            painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
            painter.drawEllipse(temp_x-led_r,
                                temp_y-led_r,
                                led_r*2,
                                led_r*2);

            points[index].center.setX(temp_x);
            points[index].center.setY(temp_y);
            points[index].radius = led_r;
            index++;
        }
        angle += 60.0;
    }

    painter.end();
}
//--------------------------------------------------------------------------------
qreal Test_Glass::rad_to_grad(qreal rad)
{
    return (rad * 180.0f) / M_PI;
}
//--------------------------------------------------------------------------------
qreal Test_Glass::grad_to_rad(qreal grad)
{
    return grad * (M_PI / 180.0f);
}
//--------------------------------------------------------------------------------
void Test_Glass::calc_line(qreal center_x,
                           qreal center_y,
                           qreal angle,
                           qreal radius,
                           qreal *end_x,
                           qreal *end_y)
{
    qreal A = radius;
    qreal B = qCos(grad_to_rad(angle)) * A;
    qreal C = qSin(grad_to_rad(angle)) * A;

    *end_x = center_x + B;
    *end_y = center_y + C;
}
//--------------------------------------------------------------------------------
