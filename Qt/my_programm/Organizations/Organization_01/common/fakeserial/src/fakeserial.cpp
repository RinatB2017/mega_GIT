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
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QIODevice>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "fakeserial.hpp"
//--------------------------------------------------------------------------------
FakeSerial::FakeSerial(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
const QByteArray FakeSerial::readAll(void)
{
    qDebug() << "send" << output.toHex();
    return output;
}
//--------------------------------------------------------------------------------
bool FakeSerial::isOpen(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void FakeSerial::close(void)
{

}
//--------------------------------------------------------------------------------
void FakeSerial::setPort(const QSerialPortInfo port)
{
    Q_UNUSED(port);
}
//--------------------------------------------------------------------------------
bool FakeSerial::setBaudRate(int speed)
{
    Q_UNUSED(speed);
    return true;
}
//--------------------------------------------------------------------------------
bool FakeSerial::open(QIODevice::OpenModeFlag flag)
{
    Q_UNUSED(flag);
    return true;
}
//--------------------------------------------------------------------------------
void FakeSerial::write(const QByteArray &data)
{
    qDebug() << data.toHex();
    bool ok = false;
    ok = analize(data);
    if(ok)
    {
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
bool FakeSerial::analize(const QByteArray &data)
{
    if(data==QByteArray::fromHex("3a464635354141353546460d"))
    {
        output.clear();
        output.append("don't worry, be happy");
        return true;
    }
    if(data==QByteArray::fromHex("3a3031303230303964636530310d"))
    {
#if 0
        qDebug() << sizeof(B7_86_EMPTY);
        packet.data.serial_year.SerialNo = 314;
        packet.data.serial_year.SerialYear = 2014;
        packet.data.header.count_data = 1;
        packet.data.header.address = 1;
        packet.data.header.cmd = CMD_REMOTE_CONTROL_ON;
        packet.data.crc16 = 0x5d03;

        QByteArray temp;
        temp.clear();
        for(unsigned int n=0; n<sizeof(B7_86_EMPTY); n++)
            temp.append(packet.buf[n]);
#endif
        Multimeter_V786_2 *m = new Multimeter_V786_2(0);
        output.clear();
        output.append(m->get_test_service_on_answer(1,
                                                    CMD_REMOTE_CONTROL_ON,
                                                    1,
                                                    314,
                                                    2014));
        m->deleteLater();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
