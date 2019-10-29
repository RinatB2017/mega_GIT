/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                      **
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
#include "fake_serialbox5.hpp"
//--------------------------------------------------------------------------------
FakeSerialBox5::FakeSerialBox5(QWidget *parent) :
    QWidget(parent)
{

}
//--------------------------------------------------------------------------------
bool FakeSerialBox5::setBaudRate(qint32 baudRate, QSerialPort::Directions directions)
{
    Q_UNUSED(baudRate)
    Q_UNUSED(directions)

    return true;
}
//--------------------------------------------------------------------------------
qint64 FakeSerialBox5::bytesAvailable(void)
{
    return 0;
}
//--------------------------------------------------------------------------------
QString FakeSerialBox5::portName(void)
{
    return "FAKE";
}
//--------------------------------------------------------------------------------
void FakeSerialBox5::setPortName(const QString &name)
{
    Q_UNUSED(name)
}
//--------------------------------------------------------------------------------
QString FakeSerialBox5::errorString(void)
{
    return "";
}
//--------------------------------------------------------------------------------
bool FakeSerialBox5::isOpen(void)
{
    return true;
}
//--------------------------------------------------------------------------------
bool FakeSerialBox5::open(QIODevice::OpenMode mode)
{
    Q_UNUSED(mode)
    return true;
}
//--------------------------------------------------------------------------------
qint64 FakeSerialBox5::write(const char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)

    return 0;
}
//--------------------------------------------------------------------------------
qint64 FakeSerialBox5::write(const char *data)
{
    Q_UNUSED(data)

    return 0;
}
//--------------------------------------------------------------------------------
qint64 FakeSerialBox5::write(const QByteArray &byteArray)
{
    Q_UNUSED(byteArray)
    return 0;
}
//--------------------------------------------------------------------------------
QByteArray FakeSerialBox5::readAll(void)
{
    return  QByteArray();
}
//--------------------------------------------------------------------------------
qint32 FakeSerialBox5::baudRate(QSerialPort::Directions directions)
{
    Q_UNUSED(directions)
    return 0;
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits FakeSerialBox5::dataBits(void)
{
    return QSerialPort::Data8;
}
//--------------------------------------------------------------------------------
QSerialPort::Parity	FakeSerialBox5::parity(void)
{
    return QSerialPort::NoParity;
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits FakeSerialBox5::stopBits(void)
{
    return QSerialPort::OneStop;
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl FakeSerialBox5::flowControl(void)
{
    return QSerialPort::NoFlowControl;
}
//--------------------------------------------------------------------------------
