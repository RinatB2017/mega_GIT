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
#ifndef CALIBRATOR_HPP
#define CALIBRATOR_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QWidget>
//--------------------------------------------------------------------------------
#ifdef TEST
    #include "H4_X_serialport_test.hpp"
#endif

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define TIME_CALIBRATION        3000
#define MAX_TIME_CALIBRATION    3000
#define MIN_TIME_CALIBRATION    99

#define P_CALIBRATOR_PORTNAME   "calibrator_portname"

enum {
    H4_6 = 6,
    H4_7 = 7
};
//--------------------------------------------------------------------------------
class Calibrator : public MyWidget
{
    Q_OBJECT

signals:
    // выходные данные
    void output(const QByteArray &);

public:
    explicit Calibrator(QWidget *parent);
    virtual ~Calibrator();

    bool port_is_open(void);

    bool find_device(void);
    bool open_device(const QString &port_name,
                     int max_repeat);
    void close_device(void);

    bool set_U_V(float value);
    bool set_U_V(float value, float freq_khz);
    bool set_I_mA(float value);
    bool set_I_mA(float value, float freq_khz);
    bool set_R(unsigned int value);
    void set_timeout(int value);
    int get_timeout(void);
    void set_delay_ms(int value);
    int get_delay_ms(void);

    //входные данные
    bool input(const QByteArray &data);

    bool send(QByteArray data, int max_repeat);
    bool check_answer_calibrator(void);
    bool check_calibrator(int max_error);
    bool step_to_step(QString text,
                      int step_to_step_timeout,
                      int calibrator_timeout,
                      bool no_delay);

    bool reset_U(void);
    bool reset_I(void);

    int get_last_error(void);
    void print_last_error(void);

    int set_safe_mode(void);

    // посимвольные команды
    bool set_V(void);
    bool set_I(void);
    bool set_R(void);
    bool set_VI(double value);
    bool set_F(double value);
    bool set_Hz(void);
    bool set_kHz(void);
    bool set_limit_1000(void);
    bool set_limit_10000(void);
    bool set_OFF(void);
    QString get_value_string(qreal value);
    //---

    void set_type_calibrator(int type_c);
    int get_type_calibrator(void);

signals:
#ifdef TEST
    void send(const QVariant &);
#endif

public slots:
    bool check_calibrator(void);

private slots:
    void log(const QString &data);
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifdef TEST
    SerialPort_H4_X_test *serial = nullptr;
#else
    QSerialPort *serial = nullptr;
#endif

    // bool is_ready;
    QByteArray data_calibrator;

    int timeout = 0;
    int last_error = 0;

    // задержка после подачи символьной команды
    int delay_ms = 1000;

    int type_calibrator = H4_6;

    void init(void);
    void wait_msec(int timeout_msec, bool wait_answer = true);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // CALIBRATOR_HPP
