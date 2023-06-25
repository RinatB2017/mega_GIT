/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef V243_TEST_HPP
#define V243_TEST_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
class V243_test : public QObject
{
    Q_OBJECT

public:
    explicit V243_test(QObject *parent = 0);

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);
    bool open(int mode);
    QByteArray readAll(void);

    void write(QByteArray data);
    void write(const char *data, int len);
    void flush(void);

    bool setFlowControl(QSerialPort::FlowControl flow);

signals:
    void readyRead(void);
    void error(QSerialPort::SerialPortError);

    void send(const QVariant &);

private:
    QByteArray input;
    QByteArray output;

    bool is_open;
};
//--------------------------------------------------------------------------------
#endif
