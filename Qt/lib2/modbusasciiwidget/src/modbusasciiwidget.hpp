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
#ifndef MODBUSASCIIWIDGET_HPP
#define MODBUSASCIIWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QWidget>
//--------------------------------------------------------------------------------
union DATA
{
    struct {
        unsigned short reg_0;
        unsigned short reg_1;
        unsigned short reg_2;
        unsigned short reg_3;
    } data;
    unsigned char buf[sizeof(data)];
};
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QToolButton;
class QComboBox;
//--------------------------------------------------------------------------------
class ModbusAsciiWidget : public MyWidget
{
    Q_OBJECT

public:
    ModbusAsciiWidget(QWidget *parent = 0);

signals:
    void get_data(const DATA &);
    void get_data(const QByteArray &);

private slots:
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

    void refresh(void);
    bool open(void);
    void close(void);

private:
    QSerialPort *serial = 0;
    QByteArray clean_data;
    QComboBox *port_name = 0;
    QComboBox *port_baud = 0;
    QComboBox *port_databits = 0;
    QComboBox *port_parity = 0;
    QComboBox *port_stopbits = 0;
    QComboBox *port_flowcontrol = 0;

    QToolButton *btn_refresh = 0;
    QToolButton *btn_start = 0;
    QToolButton *btn_stop = 0;

    void init(void);
    bool processing(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MODBUSASCIIWIDGET_HPP
