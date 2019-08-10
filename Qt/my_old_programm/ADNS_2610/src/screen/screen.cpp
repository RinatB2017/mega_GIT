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
#include "defines.hpp"
#include "screen.hpp"
//--------------------------------------------------------------------------------
Screen::Screen(QWidget *parent) :
    MyWidget(parent)
{
    for(int n=0; n<MAX_SIZE; n++)
    {
        buf[n] = 0;
    }

    setMinimumSize(MAX_X * 24 + 10,
                   MAX_Y * 24 + 10);
}
//--------------------------------------------------------------------------------
void Screen::draw(QByteArray data)
{
    if(data.length() > MAX_SIZE) return;

    for(int n=0; n<data.length(); n++)
    {
        buf[n] = data.at(n) * 4; //map 63 >> 255
    }
    update();
}
//--------------------------------------------------------------------------------
void Screen::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Screen::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Screen::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Screen::save_setting(void)
{

}
//--------------------------------------------------------------------------------
void Screen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int n = 0;
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            painter.setBrush(QBrush(QColor(buf[n], buf[n], buf[n])));
            //painter.setBrush(QBrush(buf[n]));
            painter.drawRect(x*(width() / MAX_X), y*(height() / MAX_Y), width() / MAX_X, height() / MAX_Y);
            n++;
        }
    }
}
//--------------------------------------------------------------------------------

