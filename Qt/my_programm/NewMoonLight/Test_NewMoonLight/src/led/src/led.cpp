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
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
//--------------------------------------------------------------------------------
#include "led.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Led::Led(int width,
         int height,
         QWidget *parent) :
    MyWidget(parent)
{
    setFixedSize(width, height);
}
//--------------------------------------------------------------------------------
Led::~Led()
{

}
//--------------------------------------------------------------------------------
void Led::set_color(uint16_t color)
{
#if 1
    cold_color  = (color >> 8) & 0xFF;
    hot_color = color & 0xFF;
#else
    cold_color  = ((color >> 8) & 0xFF) * 255 / 100;
    hot_color = (color & 0xFF) * 255 / 100;
#endif

    update();
}
//--------------------------------------------------------------------------------
void Led::set_hot_color(uint8_t color)
{
    hot_color = color;
    update();
}
//--------------------------------------------------------------------------------
void Led::set_cold_color(uint8_t color)
{
    cold_color = color;
    update();
}
//--------------------------------------------------------------------------------
void Led::lock(void)
{
    is_block = true;
}
//--------------------------------------------------------------------------------
void Led::unlock(void)
{
    is_block = false;
}
//--------------------------------------------------------------------------------
void Led::paintEvent(QPaintEvent *)
{
    bool ok = false;
    int index = property("index").toInt(&ok);

    QPainter painter;
    painter.begin(this);

    // hot
    painter.setPen(QPen(QColor(hot_color, 0, 0), 1, Qt::SolidLine));
    painter.setBrush(QBrush(QColor(hot_color, 0, 0)));
    painter.drawChord(QRect(0, 0, width(), height()), 90 * 16, 180 * 16);

    // cold
    painter.setPen(QPen(QColor(0, 0, cold_color), 1, Qt::SolidLine));
    painter.setBrush(QBrush(QColor(0, 0, cold_color)));
    painter.drawChord(QRect(0, 0, width(), height()), -90 * 16, 180 * 16);

    if(ok)
    {
        //emit info(QString("show %1").arg(index));
        painter.setPen(QPen(QColor(Qt::white)));

        QFontMetrics fm = painter.fontMetrics();
        QString text = QString("%1").arg(index);
        QRect r = fm.boundingRect(text);
        int w_text = r.width();
        int h_text = r.height();
        painter.drawText(width() / 2 - w_text / 2,
                         height() / 2 + h_text / 2,
                         text);
    }
    else
    {
        emit error("caanot show index");
    }

    painter.end();
}
//--------------------------------------------------------------------------------
void Led::mousePressEvent(QMouseEvent *event)
{
    if(is_block)    return;
    if(event->button() == Qt::LeftButton)
    {
        raise();
        lastPoint = event->pos();
        b_move = true;
    }
}
//--------------------------------------------------------------------------------
void Led::mouseMoveEvent(QMouseEvent *event)
{
    if(is_block)    return;
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - lastPoint));
    }
}
//--------------------------------------------------------------------------------
void Led::mouseReleaseEvent(QMouseEvent *event)
{
    if(is_block)    return;
    if (event->button() == Qt::LeftButton && b_move)
    {
        b_move = false;
    }
}
//--------------------------------------------------------------------------------
void Led::updateText(void)
{

}
//--------------------------------------------------------------------------------
