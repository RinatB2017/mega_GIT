/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "test_widget.hpp"
//--------------------------------------------------------------------------------
TestWidget::TestWidget(int l_width,
                       int l_height,
                       QWidget *parent)
    : QWidget(parent)
{
    cnt_x = l_width;
    cnt_y = l_height;

    d_width  = l_width  * led_width;
    d_height = l_height * led_height;

    for(int n=0; n<l_width * l_height; n++)
    {
        leds.append(Qt::black);
    }

    setFixedSize(d_width, d_height);
    update();
}
//--------------------------------------------------------------------------------
void TestWidget::set_color(int x, int y, QColor color)
{
    if(x > cnt_x)
    {
        return;
    }
    if(y > cnt_y)
    {
        return;
    }
    int index = y*cnt_x + x;
    leds[index] = color;

    update();
}
//--------------------------------------------------------------------------------
void TestWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(led_color));
    painter.drawRect(0, 0, width(), height());

    painter.setPen(border_color);
    for(int y=0; y<cnt_y; y++)
    {
        painter.drawLine(0, y * led_height, width(), y * led_height);
    }
    for(int x=0; x<cnt_x; x++)
    {
        painter.drawLine(x * led_width, 0, x * led_width, height());
    }

    for(int y=0; y<cnt_y; y++)
    {
        for(int x=0; x<cnt_x; x++)
        {
            int index = y*cnt_x + x;
            painter.setBrush(QBrush(leds[index]));
            painter.drawRect(x*led_width,
                             y*led_height,
                             led_width,
                             led_height);
        }
    }
}
//--------------------------------------------------------------------------------
