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
#include "display_widget.hpp"
//--------------------------------------------------------------------------------
Display_widget::Display_widget(QWidget *parent)
    : QWidget(parent)
{
    cnt_x = 64;
    cnt_y = 32;

    d_width  = cnt_x * led_width;
    d_height = cnt_y * led_height;

    for(int n=0; n<MAX_MP_BUF; n++)
    {
        leds[n] = Qt::black;
    }

    show_last_address = MAX_MP_BUF - cnt_x * cnt_y * 3;

    setFixedSize(d_width, d_height);
    update();
}
//--------------------------------------------------------------------------------
void Display_widget::set_color(int x, int y, QColor color)
{
    int index = x*cnt_y + y;
    if(index > MAX_MP_BUF)
    {
        return;
    }
    leds[index] = color;

    update();
}
//--------------------------------------------------------------------------------
void Display_widget::add_color(QColor color)
{
    if(index_add > cnt_x * cnt_y * 3)
    {
        index_add = 0;
    }
    leds[index_add] = color;
    index_add++;
}
//--------------------------------------------------------------------------------
void Display_widget::go_first(void)
{
    show_begin_address = 0;
    update();
}
//--------------------------------------------------------------------------------
void Display_widget::go_last(void)
{
    show_begin_address = show_last_address;
    update();
}
//--------------------------------------------------------------------------------
void Display_widget::go_prev(void)
{
    if(show_begin_address >= cnt_y)
    {
        show_begin_address-=cnt_y;
        update();
    }
}
//--------------------------------------------------------------------------------
void Display_widget::go_next(void)
{
    if(show_begin_address < show_last_address - cnt_y)
    {
        show_begin_address+=cnt_y;
        update();
    }
}
//--------------------------------------------------------------------------------
void Display_widget::clear(void)
{
    for(int n=0; n<MAX_MP_BUF; n++)
    {
        leds[n] = Qt::black;
    }
    update();
}
//--------------------------------------------------------------------------------
void Display_widget::paintEvent(QPaintEvent *)
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
//            int index = y*cnt_x + x;
            int index = show_begin_address + x*cnt_y + (cnt_y - 1 - y);
            painter.setBrush(QBrush(leds[index]));
            painter.drawRect(x*led_width,
                             y*led_height,
                             led_width,
                             led_height);
        }
    }
}
//--------------------------------------------------------------------------------
