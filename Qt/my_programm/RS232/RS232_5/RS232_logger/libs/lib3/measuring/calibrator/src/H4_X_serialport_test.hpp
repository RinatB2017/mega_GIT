/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef SERIALPORT_V764_2_TEST_HPP
#define SERIALPORT_V764_2_TEST_HPP
//--------------------------------------------------------------------------------
#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QElapsedTimer>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QTextStream>
#include <QToolBar>
#include <QObject>
#include <QFile>
#include <QDir>

#include "H4_X_serialport_test_param.hpp"
#include "converter_uir.hpp"
#include "connection.hpp"
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
class SerialPort_H4_X_test : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort_H4_X_test(QObject *parent = nullptr);
    virtual ~SerialPort_H4_X_test();

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo sinfo);

    bool setBaudRate(int speed);
    bool setDataBits(QSerialPort::DataBits dataBits);
    bool setParity(QSerialPort::Parity parity);
    bool setStopBits(QSerialPort::StopBits stopBits);

    bool open(int mode);
    QByteArray readAll(void);

    bool check_manual(const QByteArray &ba);
    bool check_auto(const QByteArray &ba);
    void check_command(const QByteArray &ba);

    void write(const QByteArray &data);
    void write(const char *data, int len);
    void flush(void);

    bool setFlowControl(QSerialPort::FlowControl flow);
    void set_answer_delay(int delay_ms);

signals:
    void s_info(const QString &);
    void s_debug(const QString &);
    void s_error(const QString &);
    void s_trace(const QString &);

    void readyRead(void);
    void error(QSerialPort::SerialPortError);

    // передача данных
    void output(const QByteArray &);

    // передача единичных данных
    void output_qvariant(const QVariant &);

public slots:
    //приём данных
    void input(const QByteArray &data);

private:
    QByteArray ba_input;
    QByteArray ba_output;

    bool is_open = false;

    int answer_delay = 0;

    MainWindow *mw = nullptr;
    QToolBar *test_tb = nullptr;
    SerialPort_H4_X_test_param *tp = nullptr;

    Converter_UIR *converter = nullptr;

    void init(void);
    void add_toolbar(void);
    void delay(void);
    void wait(int timeout_msec);
};
//--------------------------------------------------------------------------------
#endif
