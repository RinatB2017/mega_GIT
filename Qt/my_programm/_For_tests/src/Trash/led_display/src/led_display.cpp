/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include "led_display.hpp"
//--------------------------------------------------------------------------------
LED_display::LED_display(unsigned int max_x,
                         unsigned int max_y,
                         unsigned int led_width,
                         unsigned int led_height,
                         QWidget *parent) :
    QGraphicsView(parent)
{
    if(max_x < MIN_WIDTH)   max_x = MIN_WIDTH;
    if(max_x > MAX_WIDTH)   max_x = MAX_WIDTH;
    if(max_y < MIN_WIDTH)   max_y = MIN_WIDTH;
    if(max_y > MAX_WIDTH)   max_y = MAX_WIDTH;

    if(led_width < MIN_LED_WIDTH)   led_width = MIN_LED_WIDTH;
    if(led_width > MAX_LED_WIDTH)   led_width = MAX_LED_WIDTH;
    if(led_height < MIN_LED_HEIGHT) led_height = MIN_LED_HEIGHT;
    if(led_height > MAX_LED_HEIGHT) led_height = MAX_LED_HEIGHT;

    this->max_x = max_x;
    this->max_y = max_y;
    this->led_width = led_width;
    this->led_height = led_height;

    scene = new QGraphicsScene(0, 0, max_x * led_width, max_y * led_height, this);
    setScene(scene);

    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            items[x][y] = scene->addRect(x*led_width,
                                         y*led_height,
                                         led_width,
                                         led_height,
                                         QPen(Qt::black),
                                         QBrush(Qt::red));
            Q_CHECK_PTR(items[x][y]);
        }
    }
    setFixedSize(QSize(scene->sceneRect().width() + 4,
                       scene->sceneRect().height() + 4));
}
//--------------------------------------------------------------------------------
bool LED_display::draw_led(unsigned int x, unsigned int y, QColor color)
{
    if(x > max_x)   return false;
    if(y > max_y)   return false;

    items[x][y]->setBrush(color);
    return true;
}
//--------------------------------------------------------------------------------
unsigned int LED_display::get_max_x(void)
{
    return max_x;
}
//--------------------------------------------------------------------------------
unsigned int LED_display::get_max_y(void)
{
    return max_y;
}
//--------------------------------------------------------------------------------
void LED_display::show_param(void)
{
    emit info(QString("width %1").arg(width()));
    emit info(QString("height %1").arg(height()));
    emit info(QString("max_x %1").arg(max_x));
    emit info(QString("max_y %1").arg(max_y));
    emit info(QString("X %1").arg(max_x * led_width));
    emit info(QString("Y %1").arg(max_y * led_height));
    emit info(QString("scene width %1").arg(scene->width()));
    emit info(QString("scene height %1").arg(scene->height()));
}
//--------------------------------------------------------------------------------
