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
#include <QSerialPort>
#include <QVariant>
#include <QtMath>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "H4_X_serialport_test.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
SerialPort_H4_X_test::SerialPort_H4_X_test(QObject *parent) :
    QObject(parent)
{
    output.clear();

    is_open = false;
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_H4_X_test::readAll(void)
{
    return output;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::write(QByteArray data)
{
    if(data.isEmpty()) return;

    input.clear();
    input.append(data);

    if(input == "T0\r\n")
    {
        output.clear();
        output.append("00\r\n");

        emit readyRead();
        return;
    }

    //qDebug() << "data" << data;
    if(input.contains("UD"))
    {
        bool ok = false;
        QString temp = input.data();
        temp.remove("UD");
        float x = temp.toFloat(&ok);
#ifdef QT_DEBUG
        qDebug() << "=== UD" << x;
#endif
        if(ok) emit send(x);
        return;
    }
    if(input.contains("UA"))
    {
        bool ok = false;
        QString temp = input.data();
        temp.remove("UA");
        int pos = temp.indexOf('F');
        if(pos) temp.resize(pos);
        float x = temp.toFloat(&ok);
#ifdef QT_DEBUG
        qDebug() << "=== UA" << x;
#endif
        if(ok) emit send(x);
        return;
    }
    if(input.contains("ID"))
    {
        bool ok = false;
        QString temp = input.data();
        temp.remove("ID");
        float x = temp.toFloat(&ok);
#ifdef QT_DEBUG
        qDebug() << "=== ID" << x;
#endif
        if(ok) emit send(x);
        return;
    }
    if(input.contains("IA"))
    {
        bool ok = false;
        QString temp = input.data();
        temp.remove("IA");
        int pos = temp.indexOf('F');
        if(pos) temp.resize(pos);
        float x = temp.toFloat(&ok);
#ifdef QT_DEBUG
        qDebug() << "=== IA" << x;
#endif
        if(ok) emit send(x);
        return;
    }
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::write(const char *data, int len)
{
    QByteArray ba;
    ba.clear();
    ba.append(data, len);
    write(ba);
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::flush(void)
{

}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::setFlowControl(QSerialPort::FlowControl flow)
{
#ifdef QT_DEBUG
    qDebug() << "setFlowControl" << flow;
#else
    Q_UNUSED(flow)    ;
#endif
    return true;
}
//--------------------------------------------------------------------------------
