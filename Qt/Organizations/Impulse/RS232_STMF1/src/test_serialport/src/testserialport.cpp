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
#include <QtMath>
//--------------------------------------------------------------------------------
#include "testserialport.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
TestSerialPort::TestSerialPort(QObject *parent) :
    QObject(parent)
{
    output.clear();

    is_open = false;
}
//--------------------------------------------------------------------------------
bool TestSerialPort::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool TestSerialPort::open(int mode)
{
    qDebug() << "open" << mode;
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void TestSerialPort::close(void)
{
    qDebug() << "close";
    is_open = false;
}
//--------------------------------------------------------------------------------
void TestSerialPort::setPort(QSerialPortInfo info)
{
    qDebug() << "setPort" << info.portName();
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool TestSerialPort::setBaudRate(int speed)
{
    qDebug() << "setBaudRate" << speed;
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
QByteArray TestSerialPort::readAll(void)
{
    return convert_data_to_ascii(output);
}
//--------------------------------------------------------------------------------
void TestSerialPort::write(QByteArray data)
{
    bool ok = false;
    QByteArray ba = convert_ascii_to_data(data.data(), &ok).toHex();
    if(ok)
    {
        QByteArray temp = QByteArray::fromHex(ba);
        if(!temp.isEmpty())
        {
            if(temp.size() == sizeof(CMD_QUESTION))
            {
                CMD_QUESTION *question = (CMD_QUESTION *)temp.data();
                unsigned char crc = CRC::crc8((unsigned char *)&question->buf, sizeof(CMD_QUESTION) - 1);
                if(crc == question->data.crc8)
                {
                    //qDebug() << question->data.cmd;
                    CMD_ANSWER answer;
                    switch(question->data.cmd)
                    {
                    case CMD_GET_ID:
                        answer.data.cmd = question->data.cmd;
                        answer.data.error = 0;
                        answer.data.value = ID;
                        answer.data.crc8  = CRC::crc8((unsigned char *)&answer.buf, sizeof(CMD_ANSWER) - 1);

                        output.clear();
                        output.append((const char *)answer.buf, sizeof(CMD_ANSWER));

                        emit readyRead();
                        break;

                    case CMD_TEST:
                    case CMD_INIT:
                    case CMD_GET_DATA:
                    case CMD_CALIBRATION_0:
                    case CMD_CALIBRATION_1:
                    case CMD_CALIBRATION_2:
                    case CMD_CALIBRATION_3:
                    case CMD_CALIBRATION_4:
                    case CMD_CALIBRATION_5:
                        answer.data.cmd = question->data.cmd;
                        answer.data.error = ERR_PASS;
                        answer.data.state = 0;
                        answer.data.value = (qrand() % 1000000);
                        answer.data.crc8  = CRC::crc8((unsigned char *)&answer.buf, sizeof(CMD_ANSWER) - 1);

                        output.clear();
                        output.append((const char *)answer.buf, sizeof(CMD_ANSWER));

                        emit readyRead();
                        break;

                    default:
                        qDebug() << "unknown command" << question->data.cmd;
                        break;
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------
QByteArray TestSerialPort::convert_ascii_to_data(QString input, bool *ok)
{
    QByteArray temp_ba;

    int begin = input.indexOf(':');
    if(begin < 0)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    int end = input.indexOf('\r');
    if(end < 0)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    if(end < begin)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    temp_ba = QByteArray::fromHex(input.toLocal8Bit());

    *ok = true;
    return temp_ba;
}
//--------------------------------------------------------------------------------
QByteArray TestSerialPort::convert_data_to_ascii(QByteArray data)
{
    QByteArray temp;

    temp.clear();
    temp.append(':');
    temp.append(data.toHex());
    temp.append((char)0x0D);

    qDebug() << "convert_data_to_ascii" << temp.data();
    return temp;
}
//--------------------------------------------------------------------------------
