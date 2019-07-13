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
#include "myfiledialog.hpp"
//--------------------------------------------------------------------------------
Display_widget::Display_widget(QWidget *parent)
    : MyWidget(parent)
{
    max_x = SIZE_X;
    max_y = SIZE_Y;

    d_width  = max_x * led_width;
    d_height = max_y * led_height;

    setFixedSize(d_width, d_height);
    update();
}
//--------------------------------------------------------------------------------
void Display_widget::set_color(int x, int y, QColor color)
{
#ifdef ROTATE_DISPLAY
    int index = x*cnt_y + y;
#else
    int index = y*max_x + x;
#endif
    if(index > MAX_BUF)
    {
        return;
    }
    leds[index] = color;
}
//--------------------------------------------------------------------------------
QColor Display_widget::get_color(int x, int y)
{
#ifdef ROTATE_DISPLAY
    int index = x*cnt_y + y;
#else
    int index = y*max_x + x;
#endif
    if(index > MAX_BUF)
    {
        return Qt::black;
    }
    return leds[index];
}
//--------------------------------------------------------------------------------
bool Display_widget::set_size_led(int value)
{
    if(value < MIN_SIZE_LED)
    {
        emit error("size_led < MIN_SIZE_LED");
        return false;
    }
    if(value > MAX_SIZE_LED)
    {
        emit error("size_led too large");
        return false;
    }

    led_width  = value;
    led_height = value;

    return true;
}
//--------------------------------------------------------------------------------
int Display_widget::get_size_x(void)
{
    return max_x;
}
//--------------------------------------------------------------------------------
int Display_widget::get_size_y(void)
{
    return max_y;
}
//--------------------------------------------------------------------------------
void Display_widget::clear(void)
{
    for(int n=0; n<MAX_BUF; n++)
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
    for(int y=0; y<max_y; y++)
    {
        painter.drawLine(0, y * led_height, width(), y * led_height);
    }
    for(int x=0; x<max_x; x++)
    {
        painter.drawLine(x * led_width, 0, x * led_width, height());
    }

    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
#ifdef ROTATE_DISPLAY
            int index = show_begin_address + x*cnt_y + (cnt_y - 1 - y);
#else
            int index = (y*max_x + x);
#endif
            painter.setBrush(QBrush(leds[index]));
            painter.setBrush(QBrush(get_color(x, y)));
            painter.drawRect(x*led_width,
                             y*led_height,
                             led_width,
                             led_height);
        }
    }
}
//--------------------------------------------------------------------------------
void Display_widget::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Display_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Display_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Display_widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
