/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifdef Q_OS_WIN
#include <modbus-rtu.h>
#else
#include </usr/include/modbus/modbus-rtu.h>
#endif
//--------------------------------------------------------------------------------
#include "mythread.hpp"
//--------------------------------------------------------------------------------
MyThread::MyThread(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
MyThread::~MyThread()
{

}
//--------------------------------------------------------------------------------
void MyThread::process(void)
{
    emit info("process");
    if(portname.isEmpty())
    {
        emit error("portname is empty");
        emit finished();
        return;
    }

    // Prepare a Modbus mapping with 30 holding registers
    // (plus no output coil, one input coil and two input registers)
    // This will also automatically set the value of each register to 0
    modbus_mapping_t *mapping = modbus_mapping_new(0, 1, 30, 2);
    if (!mapping)
    {
        emit error(QString("Failed to allocate the mapping: %1").arg(modbus_strerror(errno)));
        emit finished();
        return;
    }

    //---
    mapping->tab_registers[0] = 0x0000; // status
    mapping->tab_registers[3] = 0x1188; // reset
    mapping->tab_registers[4] = 1234;   // firmware

    mapping->tab_input_registers[0] = 1000; // temperature 0
    mapping->tab_input_registers[1] = 500;  // temperature 1
    //---

    modbus_t *ctx = modbus_new_rtu(portname.toLocal8Bit(),
                                   115200,
                                   'N', 8, 1);
    if (!ctx)
    {
        emit error(QString("Failed to create the context: %1").arg(modbus_strerror(errno)));
        emit finished();
        return;
    }

    //Set the Modbus address of this slave (to 1)
    modbus_set_slave(ctx, 1);

    if (modbus_connect(ctx) == -1)
    {
        emit error(QString("Unable to connect: %1").arg(modbus_strerror(errno)));
        modbus_free(ctx);
        emit finished();
        return;
    }

    uint8_t req[MODBUS_RTU_MAX_ADU_LENGTH];// request buffer
    int len;// length of the request/response

    while(!flag_exit)
    {
        if(!flag_exit)
        {
            emit message("modbus_receive");
            len = modbus_receive(ctx, req);
            if (len == -1)
            {
                emit error("modbus_receive");
                flag_exit = true;
            }
            else
            {
                QByteArray ba_receive;
                ba_receive.clear();
                ba_receive.append((char *)&req, len);
                emit message(QString("len %1 [%2]")
                             .arg(len)
                             .arg(ba_receive.toHex().data()));
            }
        }

        if(!flag_exit)
        {
            emit message("modbus_reply");
            len = modbus_reply(ctx, req, len, mapping);
            if (len == -1)
            {
                emit error("modbus_reply");
                flag_exit = true;
            }
            else
            {
                QByteArray ba_reply;
                ba_reply.clear();
                ba_reply.append((char *)&req, len);
                emit message(QString("len %1 [%2]")
                             .arg(len)
                             .arg(ba_reply.toHex().data()));
            }
        }

        memset(req, 0, MODBUS_RTU_MAX_ADU_LENGTH);
    }

    modbus_mapping_free(mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    emit finished();
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
void MyThread::set_portname(QString value)
{
    portname = value;
}
//--------------------------------------------------------------------------------
