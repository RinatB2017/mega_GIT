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
#ifndef FAKE_GDS_840C_HPP
#define FAKE_GDS_840C_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QFrame>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Fake_oscilloscope_GDS_840C : public MyWidget
{
    Q_OBJECT
public:
    Fake_oscilloscope_GDS_840C(QWidget *parent = 0);

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);
    bool open(int mode);
    QByteArray readAll(void);

    void write(QByteArray data);
    void write(const char *data, int len);

signals:
    void readyRead(void);
    void error(QSerialPort::SerialPortError);

public slots:
    void receive(const QVariant &data);

private:
    QByteArray input;
    QByteArray output;

    bool is_open = false;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
