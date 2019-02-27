/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef V243_HPP
#define V243_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QWidget>
//--------------------------------------------------------------------------------
#ifdef FAKE
    #include "v2_43_test.hpp"
#endif
//--------------------------------------------------------------------------------
class V243 : public QWidget
{
    Q_OBJECT

public:
    explicit V243(QWidget *parent);
    virtual ~V243();

    bool port_is_open(void);
    bool open_device(const QString &port_name,
                     QSerialPort::FlowControl flow);

    bool send(QByteArray data);
    bool send_wo_pause(QByteArray data);

    int  get_last_error(void);
    void print_last_error(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

#ifdef FAKE
    void send(const QVariant &);
#endif

private slots:
    void log(const QString &data);
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifdef FAKE
    V243_test serial;
#else
    QSerialPort serial;
#endif

    QByteArray dirty_data;

    int last_error;

    void init(void);
    void connect_log(void);

    void wait_msec(int timeout_msec);
};
//--------------------------------------------------------------------------------
#endif // CALIBRATOR_HPP
