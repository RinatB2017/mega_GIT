/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QTime>
//--------------------------------------------------------------------------------
#include "mythread.hpp"
//--------------------------------------------------------------------------------
MyThread::MyThread(QObject *parent) :
    QObject(parent)
{
    counter = 5;
}
//--------------------------------------------------------------------------------
MyThread::~MyThread()
{

}
//--------------------------------------------------------------------------------
void MyThread::process(void)
{
    emit info("process");
    while(counter)
    {
        QTime time;
        time.start();
        while(time.elapsed() < 1000)
        {

        }
        emit info(QString("counter %1").arg(counter));
        counter--;
    }
    emit finished();
}
//--------------------------------------------------------------------------------
