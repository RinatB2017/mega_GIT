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
#include <QDebug>
#include <QTime>
#include "thread.hpp"
//--------------------------------------------------------------------------------
Serial_thread::Serial_thread()
{

}
//--------------------------------------------------------------------------------
Serial_thread::~Serial_thread()
{

}
//--------------------------------------------------------------------------------
void Serial_thread::process(void)
{
    QSerialPort serial;
    //connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));

    emit info("thread init");
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString("port %1").arg(port.portName()));
        serial.setPort(port);
        bool ok = serial.setBaudRate(115200);
        if(ok)
        {
            if(!serial.open(QIODevice::ReadWrite))
            {
                emit error("error open");
                return;
            }
            emit info("processing ...");
            QTime time;
            time.start();
            while(time.elapsed() < 10000)
            {
                bool ok = true;
                while(ok)
                {
                    ok = serial.waitForReadyRead(2);
                }
                QByteArray ba = serial.readAll();
                if(ba.isEmpty() == false)
                {
                    emit info("packet received");
                    emit info(ba.toHex());
                }
            }
        }
        break;
    }
    if(serial.isOpen()) serial.close();
    emit finished();
}
//--------------------------------------------------------------------------------
void Serial_thread::port_read(void)
{
    emit info("port_read");
}
//--------------------------------------------------------------------------------
