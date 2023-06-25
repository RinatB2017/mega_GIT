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
#ifndef FAKESERIAL_HPP
#define FAKESERIAL_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QObject>
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
class FakeSerial : public QObject
{
    Q_OBJECT
public:
    explicit FakeSerial(QObject *parent = 0);
    const QByteArray readAll(void);
    bool isOpen(void);
    void close(void);
    void setPort(const QSerialPortInfo port);
    bool setBaudRate(int speed);
    bool open(QIODevice::OpenModeFlag flag);
    void write(const QByteArray &data);

signals:
    void readyRead(void);
    void error(QSerialPort::SerialPortError);

private:
    bool analize(const QByteArray &data);
    QByteArray output;

    typedef struct
    {
        unsigned short SerialNo;
        unsigned short SerialYear;
    } SERIAL;

    typedef struct
    {
        unsigned char address;
        unsigned char cmd;
        unsigned char count_data;
    } B7_86_HEADER;

    union B7_86_EMPTY
    {
        struct {
            B7_86_HEADER header;
            SERIAL serial_year;
            unsigned short crc16;
        } data;
        unsigned char buf[sizeof(data)];
    };

    B7_86_EMPTY packet;
};
//--------------------------------------------------------------------------------
#endif
