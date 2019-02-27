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
#ifndef CALIBRATOR_HPP
#define CALIBRATOR_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QWidget>
//--------------------------------------------------------------------------------
#ifdef FAKE
    #include "H4_X_serialport_test.hpp"
#endif
//--------------------------------------------------------------------------------
#define TIME_CALIBRATION        3000
#define MAX_TIME_CALIBRATION    3000
#define MIN_TIME_CALIBRATION    99
//--------------------------------------------------------------------------------
class Calibrator : public QWidget
{
    Q_OBJECT

public:
    explicit Calibrator(QWidget *parent);
    ~Calibrator();

    bool port_is_open(void);
    bool open_device(const QString &port_name,
                     QSerialPort::FlowControl flow);

    bool set_U_V(float value);
    bool set_U_V(float value, float freq);
    bool set_I_mA(float value);
    bool set_I_mA(float value, float freq);
    bool set_R(unsigned int value);
    void set_timeout(unsigned int value);

    bool send(QByteArray data);
    bool send_wo_pause(QByteArray data);
    bool check_calibrator(int max_error);
    bool step_to_step(QString text,
                      int step_to_step_timeout,
                      int calibrator_timeout,
                      bool no_delay);

    bool reset_U(void);
    bool reset_I(void);

    int get_last_error(void);
    void print_last_error(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

#ifdef FAKE
    void send(const QVariant &);
#endif

public slots:
    bool check_calibrator(void);

private slots:
    void log(const QString &data);
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifdef FAKE
    SerialPort_H4_X_test serial;
#else
    QSerialPort serial;
#endif

    // bool is_ready;
    QByteArray data_calibrator;

    int timeout;
    int last_error;

    void init(void);

    void wait_msec(int timeout_msec);
};
//--------------------------------------------------------------------------------
#endif // CALIBRATOR_HPP
