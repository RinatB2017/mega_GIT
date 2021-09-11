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
#include <QCoreApplication>
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "mythread.hpp"
//--------------------------------------------------------------------------------
MyThread::MyThread(QObject *parent) :
    QObject(parent)
{
    init();;
}
//--------------------------------------------------------------------------------
MyThread::~MyThread()
{
    if(udp_socket1) udp_socket1->deleteLater();
    if(udp_socket2) udp_socket2->deleteLater();
}
//--------------------------------------------------------------------------------
void MyThread::process(void)
{
    emit info("process");
    while(!flag_exit)
    {

    }
    emit finished();
}
//--------------------------------------------------------------------------------
void MyThread::init(void)
{
    udp_socket1 = new QUdpSocket(this);
    udp_socket2 = new QUdpSocket(this);
}
//--------------------------------------------------------------------------------
void MyThread::set_port1(quint16 value)
{
    port1 = value;
}
//--------------------------------------------------------------------------------
void MyThread::set_port2(quint16 value)
{
    port2 = value;
}
//--------------------------------------------------------------------------------
void MyThread::start(void)
{

}
//--------------------------------------------------------------------------------
void MyThread::stop(void)
{
    flag_exit = true;
}
//--------------------------------------------------------------------------------
