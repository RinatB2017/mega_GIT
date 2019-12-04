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
    // emit trace("process");

    serial5 = new QSerialPort(this);
    if(serial5 == nullptr)
    {
        emit finished();
        return;
    }

    while(!flag_exit)
    {
        if(flag_port_open)
        {
            if(serial5)
            {
                // emit debug("open:");
                bool state = serial5->open(QIODevice::ReadWrite);
                emit port_get_state(state);
                flag_port_open = state;

                if(state)
                {
                    emit port_get_name(serial5->portName());
                    emit port_get_baudrate(serial5->baudRate());
                    emit port_get_bits(serial5->dataBits());
                    emit port_get_patity(serial5->parity());
                    emit port_get_stop_bits(serial5->stopBits());
                    emit port_get_flow_control(serial5->flowControl());
                }
            }
        }
        else
        {
            if(serial5)
            {
                // emit debug("close:");
                serial5->close();
                emit port_get_state(false);
                flag_port_name = false;
            }
        }
        if (serial5->error() != QSerialPort::NoError)
        {
            emit port_error(serial5->error());
        }
        if(flag_port_name)
        {
            serial5->setPortName(port_name);
            flag_port_name = false;
        }
        if(flag_baudrate)
        {
            serial5->setBaudRate(baudrate);
            flag_baudrate = false;
        }
        if(flag_data_bits)
        {
            serial5->setDataBits(data_bits);
            flag_data_bits = false;
        }
        if(flag_stop_bits)
        {
            serial5->setStopBits(stop_bits);
            flag_stop_bits = false;
        }
        if(flag_parity)
        {
            serial5->setParity(parity);
            flag_parity = false;
        }
        if(flag_flow_control)
        {
            serial5->setFlowControl(flow_control);
            flag_flow_control = false;
        }

        if(serial5->isOpen())
        {
            if(serial5->waitForReadyRead())
            {
#if 0
                emit trace("---");
                emit trace(QString("%1").arg(serial5->portName()));
                emit trace(QString("%1").arg(serial5->baudRate()));
                emit trace(QString("%1").arg(serial5->dataBits()));
                emit trace(QString("%1").arg(serial5->parity()));
                emit trace(QString("%1").arg(serial5->stopBits()));
                emit trace(QString("%1").arg(serial5->flowControl()));
                emit trace("---");
#endif

                serial_data = serial5->readAll();
                if(!serial_data.isEmpty())
                {
                    // emit info(serial_data);
                    emit port_read_all(serial_data);
                    emit readyRead();
                    emit readChannelFinished();
                }
            }
        }
    }
    emit finished();
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_open(void)
{
    flag_port_open = true;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_close(void)
{
    flag_port_open = false;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_set_name(QString name)
{
    port_name = name;
    flag_port_name = true;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_set_baudrate(qint32 value)
{
    baudrate = value;
    flag_baudrate = true;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_set_bits(QSerialPort::DataBits value)
{
    data_bits = value;
    flag_data_bits = true;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_set_stop_bits(QSerialPort::StopBits value)
{
    stop_bits = value;
    flag_stop_bits = true;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_set_parity(QSerialPort::Parity value)
{
    parity = value;
    flag_parity = true;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_set_flow_control(QSerialPort::FlowControl value)
{
    flow_control = value;
    flag_flow_control = true;
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_write(const char *data)
{
    Q_CHECK_PTR(serial5);
    if(serial5)
    {
        serial5->write(data);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::port_write(const char *data, qint64 maxSize)
{
    Q_CHECK_PTR(serial5);
    if(serial5)
    {
        serial5->write(data, maxSize);
    }
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
