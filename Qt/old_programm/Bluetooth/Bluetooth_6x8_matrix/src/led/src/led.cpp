/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QDebug>
#include "led.hpp"
//--------------------------------------------------------------------------------
Led::Led(QToolButton *parent) :
    QToolButton(parent)
{
    setFixedSize(64, 64);
    color = '0';

    connect(this, SIGNAL(clicked(bool)), this, SLOT(click()));
}
//--------------------------------------------------------------------------------
char Led::get_color(void)
{
    return color;
}
//--------------------------------------------------------------------------------
void Led::set_color(char color)
{
    switch(color)
    {
    case '0':
        setStyleSheet("");
        setText("");
        break;

    case 'R':
        setStyleSheet("background:red;");
        setText("R");
        break;

    case 'G':
        setStyleSheet("background:green;");
        setText("G");
        break;

    case 'B':
        setStyleSheet("background:blue;");
        setText("B");
        break;

    default:
        qDebug() << "unknown color" << color;
        break;
    }
}
//--------------------------------------------------------------------------------
void Led::click(void)
{
    switch(color)
    {
    case '0':
        setStyleSheet("");
        setText("");
        color = 'R';
        break;

    case 'R':
        setStyleSheet("background:red;");
        setText("R");
        color = 'G';
        break;

    case 'G':
        setStyleSheet("background:green;");
        setText("G");
        color = 'B';
        break;

    case 'B':
        setStyleSheet("background:blue;");
        setText("B");
        color = '0';
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
