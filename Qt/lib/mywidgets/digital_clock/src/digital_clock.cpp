/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "digital_clock.hpp"
//--------------------------------------------------------------------------------
Digital_clock::Digital_clock(QWidget *parent) :
    QLabel(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Digital_clock::~Digital_clock()
{
    if(timer)
    {
        timer->stop();
        disconnect(timer,  &QTimer::timeout,   this,   &Digital_clock::timeout);
        timer->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void Digital_clock::init(void)
{
    timer = new QTimer(this);

    connect(timer,  &QTimer::timeout,   this,   &Digital_clock::timeout);

    time = QTime::currentTime();
    hour = time.hour();
    min  = time.minute();
    sec  = time.second();

    QString clock = QString("%1:%2:%3")
            .arg(hour, 2, 10, QChar('0'))
            .arg(min,  2, 10, QChar('0'))
            .arg(sec,  2, 10, QChar('0'));
    setText(clock);

    timer->start(1000);
}
//--------------------------------------------------------------------------------
void Digital_clock::timeout(void)
{
    sec++;
    if(sec > 59)
    {
        sec = 0;
        min++;
    }
    if(min > 59)
    {
        min = 0;
        hour++;
    }
    if(hour > 23)
    {
        hour = 0;
    }

    QString clock = QString("%1:%2:%3")
            .arg(hour, 2, 10, QChar('0'))
            .arg(min, 2, 10, QChar('0'))
            .arg(sec, 2, 10, QChar('0'));
    setText(clock);
}
//--------------------------------------------------------------------------------
