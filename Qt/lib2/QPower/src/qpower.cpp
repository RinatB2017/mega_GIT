/***************************************************************************
 *   Copyright (C) 2008 - Giuseppe Cigala                                  *
 *   g_cigala@virgilio.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "qpower.h"

QPower::QPower(QWidget *parent) : QWidget(parent)
{
    setState(false);
    setColor(Qt::red);
}

void QPower::paintEvent(QPaintEvent *)
{
    paintBorder();

    if (pow_state)
        paintPowerOn();

    else
        paintPowerOff();

}

void QPower::paintBorder()

{
    QPainter painter(this);
    painter.setWindow(0, 0, 300, 200);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QLinearGradient linGrad1(30, 100, 150, 100);
    linGrad1.setColorAt(0, Qt::gray);
    linGrad1.setColorAt(1, Qt::white);
    linGrad1.setSpread(QGradient::ReflectSpread);
    painter.setBrush(linGrad1);

    QRectF border1(20, 20, 260, 160);
    painter.drawRoundRect(border1, 20, 20);

    painter.setPen(QPen(Qt::black, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF border2(25, 25, 250, 150);
    painter.drawRoundRect(border2, 20, 20);

}


void QPower::paintPowerOn()

{
    QPainter painter(this);
    painter.setWindow(0, 0, 300, 200);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QLinearGradient linGrad2(150, 50, 150, 175);
    linGrad2.setColorAt(0, Qt::darkGray);
    linGrad2.setColorAt(1, pow_color);
    linGrad2.setSpread(QGradient::PadSpread);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(linGrad2);

    QRectF border2(40, 40, 220, 120);
    painter.drawRoundRect(border2, 20, 20);

    QLinearGradient linGrad1(30, 70, 150, 130);
    linGrad1.setColorAt(0, Qt::gray);
    linGrad1.setColorAt(1, Qt::white);
    linGrad1.setSpread(QGradient::ReflectSpread);
    painter.setBrush(linGrad1);

    QRectF border1(50, 50, 200, 100);
    painter.drawRoundRect(border1, 20, 20);

    painter.setPen(QPen(pow_color, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF rect(122, 72, 56, 56);
    int startAngle = 115 * 16;
    int spanAngle = 310 * 16;
    painter.drawArc(rect, startAngle, spanAngle);
    painter.drawLine(150, 67, 150, 89);

}


void QPower::paintPowerOff()

{
    QPainter painter(this);
    painter.setWindow(0, 0, 300, 200);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QLinearGradient linGrad2(150, 50, 150, 175);
    linGrad2.setColorAt(0, Qt::darkGray);
    linGrad2.setColorAt(1, Qt::white);
    linGrad2.setSpread(QGradient::PadSpread);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(linGrad2);

    QRectF border2(40, 40, 220, 120);
    painter.drawRoundRect(border2, 20, 20);

    QLinearGradient linGrad1(30, 80, 150, 120);
    linGrad1.setColorAt(0, Qt::gray);
    linGrad1.setColorAt(1, Qt::white);
    linGrad1.setSpread(QGradient::ReflectSpread);
    painter.setBrush(linGrad1);

    QRectF border1(45, 45, 210, 110);
    painter.drawRoundRect(border1, 20, 20);

    painter.setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF rect(120, 70, 60, 60);
    int startAngle = 115 * 16;
    int spanAngle = 310 * 16;
    painter.drawArc(rect, startAngle, spanAngle);
    painter.drawLine(150, 65, 150, 90);

}

void QPower::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pow_state = !pow_state;
        emit toggled(pow_state);
        update();
    }
}


void QPower::setColor(QColor color)
{
    pow_color = color;
    update();
}


void QPower::setState(bool state)
{
    if (state == true)
        pow_state = true;

    else
        pow_state = false;

    update();

}

QSize QPower::minimumSizeHint() const
{
    return QSize(30, 20);
}

QSize QPower::sizeHint() const
{
    return QSize(300, 200);
}


