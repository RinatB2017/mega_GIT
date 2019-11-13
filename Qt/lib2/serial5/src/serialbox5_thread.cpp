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
#include <QCoreApplication>
#include <QTime>
//--------------------------------------------------------------------------------
#include "serialbox5_thread.hpp"
//--------------------------------------------------------------------------------
SerialBox5_thread::SerialBox5_thread(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
SerialBox5_thread::~SerialBox5_thread()
{

}
//--------------------------------------------------------------------------------
void SerialBox5_thread::process(void)
{
    emit info("process");
    QTime time;
    while(!flag_exit)
    {
        time.start();
        while(time.elapsed() < 1000)
        {
            if(flag_exit)
            {
                emit finished();
                return;
            }
        }

        if(sec<59)
        {
            sec++;
        }
        else
        {
            sec = 0;
            if(min<59)
            {
                min++;
            }
            else
            {
                min = 0;
                if(hour<23)
                {
                    hour++;
                }
                else
                {
                    hour = 0;
                }
            }
        }

        emit set_hour(hour);
        emit set_min(min);
        emit set_sec(sec);
    }
    emit finished();
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::set_time(QDateTime dt)
{
    hour = static_cast<uint8_t>(dt.time().hour());
    min  = static_cast<uint8_t>(dt.time().minute());
    sec  = static_cast<uint8_t>(dt.time().second());
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::start(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5_thread::stop(void)
{
    flag_exit = true;
}
//--------------------------------------------------------------------------------
