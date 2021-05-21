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
#include <QApplication>
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
    ModbusAsciiWidget(QWidget *parent = nullptr);

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
    QSerialPort *serial;
    QByteArray clean_data;
    QComboBox *port_name;
    QComboBox *port_baud;
    QComboBox *port_databits;
    QComboBox *port_parity;
    QComboBox *port_stopbits;
    QComboBox *port_flowcontrol;

    QToolButton *btn_refresh;
    QToolButton *btn_start;
    QToolButton *btn_stop;

    void init(void);
    bool processing(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MODBUSASCIIWIDGET_HPP
