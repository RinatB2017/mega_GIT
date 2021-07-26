/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "analog_clock.hpp"
//--------------------------------------------------------------------------------
Analog_clock::Analog_clock(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Analog_clock::~Analog_clock()
{

}
//--------------------------------------------------------------------------------
void Analog_clock::init(void)
{
    font.setFamily("Arial");
    font.setPointSize(font_size);
    font.setBold(true);

    setMinimumSize(200, 200);
}
//--------------------------------------------------------------------------------
void Analog_clock::calc_line(qreal center_x,
                             qreal center_y,
                             qreal angle,
                             qreal radius,
                             qreal *end_x,
                             qreal *end_y)
{
    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    *end_x = center_x + B;
    *end_y = center_y + C;
}
//--------------------------------------------------------------------------------
void Analog_clock::paintEvent(QPaintEvent *)
{
    QPainter painter;

    int w = width();
    int h = height();

    int d = ((w < h) ? w : h) - 1;

    qreal center_x = d / 2.0;
    qreal center_y = center_x;
    qreal radius_o = d / 2.0 - 1;   //внешний радиус
    qreal radius_i = d / 2.0 - 7;   //внутренний радиус
    qreal angle = 300;

    qreal end_x = 0;
    qreal end_y = 0;

    painter.begin(this);
    painter.setPen(QColor(Qt::black));
    painter.setFont(font);
    //painter.drawEllipse(QPointF(center_x, center_y), radius_o, radius_o);
    //painter.drawEllipse(QPointF(center_x, center_y), radius_i, radius_i);
    painter.drawEllipse(QPointF(center_x, center_y), 5, 5);

    QFontMetrics fontMetrics(font);

    for(int n=0; n<60; n++)
    {
        qreal x1 = 0;
        qreal y1 = 0;
        qreal x2 = 0;
        qreal y2 = 0;
        calc_line(center_x,
                  center_y,
                  angle,
                  radius_i,
                  &x1,
                  &y1);
        calc_line(center_x,
                  center_y,
                  angle,
                  radius_o,
                  &x2,
                  &y2);
        if(((int)angle % 30) == 0)
            painter.setPen(QPen(Qt::black, 3));
        else
            painter.setPen(QPen(Qt::black, 1));
        painter.drawLine(x1, y1, x2, y2);
        angle += 6;
        if(angle >= 360) angle = 0;
    }

    angle = 300;
    painter.setPen(QPen(Qt::black, 1));
    for(int n=1; n<=12; n++)
    {
        QString text = QString::number(n);
        calc_line(center_x,
                  center_y,
                  angle,
                  radius_o - fontMetrics.height(),
                  &end_x,
                  &end_y);
        QRect rect(end_x - (fontMetrics.horizontalAdvance(text) / 2.0),
                   end_y - (fontMetrics.height() / 2.0),
                   fontMetrics.horizontalAdvance(text),
                   fontMetrics.height());
        painter.drawText(rect, Qt::AlignCenter, text);
        angle += 30;
        if(angle >= 360) angle = 0;
    }

    painter.end();
}
//--------------------------------------------------------------------------------
