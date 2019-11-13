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
    return is_open;
}
//--------------------------------------------------------------------------------
bool FakeSerialBox5::close(void)
{
    is_open = false;
    return true;
}
//--------------------------------------------------------------------------------
bool FakeSerialBox5::open(QIODevice::OpenMode mode)
{
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
qint64 FakeSerialBox5::write(const char *data, qint64 maxSize)
{
    fake_data.clear();
    fake_data.append(data, static_cast<int>(maxSize));

    return 0;
}
//--------------------------------------------------------------------------------
qint64 FakeSerialBox5::write(const char *data)
{
    fake_data.clear();
    fake_data.append(data);
    return 0;
}
//--------------------------------------------------------------------------------
qint64 FakeSerialBox5::write(const QByteArray &byteArray)
{
    fake_data.clear();
    fake_data.append(byteArray);
    return 0;
}
//--------------------------------------------------------------------------------
QByteArray FakeSerialBox5::readAll(void)
{
    return fake_data;
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
int FakeSerialBox5::fake_input(const QByteArray &sending_data)
{
    fake_data.clear();
    fake_data.append(sending_data);

    return 0;   //E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int FakeSerialBox5::fake_input(const QString &data)
{
    fake_data.clear();
    fake_data.append(data);

    return 0;   //E_NO_ERROR;
}
//--------------------------------------------------------------------------------
