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
#ifndef V786_2_SERIALPORT_TEST_HPP
#define V786_2_SERIALPORT_TEST_HPP
//--------------------------------------------------------------------------------
#include <QtGlobal>

#ifdef Q_OS_LINUX
#   include <stdint.h>
#endif
#ifdef Q_OS_WIN
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QToolBar>
#include <QObject>

#include "v786_2_serialport_test_param.hpp"
#include "mainwindow.hpp"
#include "breloque.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#if 1
// старый свисток
static const QString breloque_answer("don't worry, be happy");
#else
// новый свисток
static const QString breloque_answer("use linux, bro!");
#endif
//--------------------------------------------------------------------------------
class SerialPort_V786_2_test : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort_V786_2_test(QObject *parent = nullptr);
    virtual ~SerialPort_V786_2_test();

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);

    bool setSerNo(uint16_t serno);
    bool setYear(uint16_t year);
    bool setAddress(uint8_t address);
    bool setChannel(uint8_t channel);

    bool set_data_0(uint8_t value);
    bool set_data_1(uint8_t value);
    bool set_timeout(uint16_t value);

    bool set_Range(int value);
    bool set_Mode(int value);
    bool set_Math_Hold(int value);
    bool set_limits(uint8_t value);
    bool set_deviation_retention(int value);
    bool set_Major(int value);
    bool set_Minor(int value);
    bool set_Release(int value);
    bool set_measuring_value(int value);
    bool set_BattVoltage(qreal value);

    uint16_t getSerNo(void);
    uint16_t getYear(void);
    uint8_t getAddress(void);
    uint8_t getChannel(void);

    bool open(int mode);
    QByteArray readAll(void);

    // входные данные
    void write(const QByteArray &data);
    void write(const char *data, int len);

    void receive(const QVariant &data);

    void port_close(void);
    bool serial_open(void);
    void serial_close(void);
    QStringList get_port_names(void);
    bool setPortName(const QString &name);

    void set_answer_delay(int delay_ms);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    // выходные данные
    void output(QByteArray);

    void s_readyRead(void);
    void s_error(QSerialPort::SerialPortError);

public slots:
    // входные данные
    void input(QByteArray data);

private:
    typedef struct
    {
        int id;
        QString caption;
        void (SerialPort_V786_2_test::*func)(void);
    } FUNC;
    QList<FUNC> functions;

    QByteArray ba_input;
    QByteArray ba_output;

    bool is_open = false;

    QString SerialName;

    MainWindow *mw = nullptr;
    QToolBar *test_tb = nullptr;
    SerialPort_V786_2_test_param *tp = nullptr;

    //---
    uint16_t SerialNo = 1234;
    uint16_t SerialYear = 2022;
    uint8_t  Address = 15;
    uint8_t  Channel = 15;

    uint8_t Range = 0; // предел
    uint8_t Mode = 0;  // режим
    uint8_t Math_Hold = 0;
    uint16_t BattVoltage = 0;

    char DisplaySymbols[7];
    uint8_t limits = 0;
    uint8_t deviation_retention = 0;

    uint8_t  Major = 0;
    uint8_t  Minor = 0;
    uint8_t  Release = 0;

    uint8_t data_0 = 0;
    uint8_t data_1 = 0;

    uint16_t timeout_ms = 0;

    float measuring_value;

    int answer_delay = 0;
    //---

    void init(void);
    void init_functions(void);
    void add_toolbar(void);

    void set_param(void);

    void print_log(const QString &text);
    void print_err_log(const QString &text);

    void f_get(void);
    void f_set(void);

    template<typename T1>
    bool check_packet(void);
    bool check_V764_2_RF_STATUS_REQUEST(void);
    bool check_V764_2_RF_REMOTE_CONTROL_ENABLE(void);
    bool check_V764_2_RF_REMOTE_CONTROL_DISABLE(void);
    bool check_V764_2_RF_SERVICE_ENABLE(void);
    bool check_V764_2_RF_POWER_OFF(void);
    bool check_V764_2_RF_DC_VOLTAGE_MEASUREMENT(void);
    bool check_V764_2_RF_AC_VOLTAGE_MEASUREMENT(void);
    bool check_V764_2_RF_DC_CURRENT_MEASUREMENT(void);
    bool check_V764_2_RF_AC_CURRENT_MEASUREMENT(void);
    bool check_V764_2_RF_RESISTANCE_MEASUREMENT(void);
    bool check_V764_2_RF_CAPACITANCE_MEASUREMENT(void);
    bool check_V764_2_RF_INDUCTANCE_MEASUREMENT(void);
    bool check_V764_2_RF_BUZZER_TEST(void);
    bool check_V764_2_RF_FREQUENCY_MEASUREMENT(void);
    bool check_V764_2_RF_ACTIVE_POWER_MEASUREMENT(void);
    bool check_V764_2_RF_REACTIVE_POWER_MEASUREMENT(void);
    bool check_V764_2_RF_FULL_POWER_MEASUREMENT(void);
    bool check_V764_2_RF_COS_FI_MEASUREMENT(void);
    bool check_V764_2_RF_UDC_CAL(void);
    bool check_V764_2_RF_UAC_CAL(void);
    bool check_V764_2_RF_IDC_CAL(void);
    bool check_V764_2_RF_IAC_CAL(void);
    bool check_V764_2_RF_RES_CAL(void);
    bool check_V764_2_RF_FLASH_WRITE(void);
    bool check_V764_2_RF_SERVICE_DISABLE(void);
    bool check_V764_2_RF_DIODE_TEST(void);
    bool check_V764_2_RF_UDC_CAL_ZEROES(void);
    bool check_V764_2_RF_IDC_CAL_ZEROES(void);
    bool check_V764_2_RF_RES_CAL_ZEROES(void);
    bool check_V764_2_RF_CAP_CAL_ZEROES(void);
    bool check_V764_2_RF_IND_CAL_ZEROES(void);
    bool check_V764_2_RF_FLASH_WRITE_ZEROES(void);
    bool check_V764_2_RF_READ_POINT_FROM_MEMORY(void);
    bool check_V764_2_RF_WRITE_POINT_TO_MEMORY(void);
    bool check_V764_2_RF_READ_FIRMWARE_VERSION(void);
    bool check_V764_2_RF_SET_NEW_ADDRESS(void);
    bool check_V764_2_RF_GET_RSSI(void);

    bool check_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES(void);
    bool check_V764_2_RF_GET_CNT_E21(void);
    bool check_V764_2_RF_TEST(void);

    void prepare_answer_V764_2_RF_STATUS_REQUEST(void);
    void prepare_answer_V764_2_RF_REMOTE_CONTROL_ENABLE(void);
    void prepare_answer_V764_2_RF_REMOTE_CONTROL_DISABLE(void);
    void prepare_answer_V764_2_RF_SERVICE_ENABLE(void);
    void prepare_answer_V764_2_RF_POWER_OFF(void);
    void prepare_answer_V764_2_RF_DC_VOLTAGE_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_AC_VOLTAGE_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_DC_CURRENT_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_AC_CURRENT_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_RESISTANCE_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_CAPACITANCE_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_INDUCTANCE_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_BUZZER_TEST(void);
    void prepare_answer_V764_2_RF_FREQUENCY_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_ACTIVE_POWER_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_REACTIVE_POWER_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_FULL_POWER_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_COS_FI_MEASUREMENT(void);
    void prepare_answer_V764_2_RF_UDC_CAL(void);
    void prepare_answer_V764_2_RF_UAC_CAL(void);
    void prepare_answer_V764_2_RF_IDC_CAL(void);
    void prepare_answer_V764_2_RF_IAC_CAL(void);
    void prepare_answer_V764_2_RF_RES_CAL(void);
    void prepare_answer_V764_2_RF_FLASH_WRITE(void);
    void prepare_answer_V764_2_RF_SERVICE_DISABLE(void);
    void prepare_answer_V764_2_RF_DIODE_TEST(void);
    void prepare_answer_V764_2_RF_UDC_CAL_ZEROES(void);
    void prepare_answer_V764_2_RF_IDC_CAL_ZEROES(void);
    void prepare_answer_V764_2_RF_RES_CAL_ZEROES(void);
    void prepare_answer_V764_2_RF_CAP_CAL_ZEROES(void);
    void prepare_answer_V764_2_RF_IND_CAL_ZEROES(void);
    void prepare_answer_V764_2_RF_FLASH_WRITE_ZEROES(void);
    void prepare_answer_V764_2_RF_READ_POINT_FROM_MEMORY(void);
    void prepare_answer_V764_2_RF_WRITE_POINT_TO_MEMORY(void);
    void prepare_answer_V764_2_RF_READ_FIRMWARE_VERSION(void);
    void prepare_answer_V764_2_RF_SET_NEW_ADDRESS(void);
    void prepare_answer_V764_2_RF_GET_RSSI(void);

    void prepare_answer_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES(void);
    void prepare_answer_V764_2_RF_GET_CNT_E21(void);
    void prepare_answer_V764_2_RF_TEST(void);

    QByteArray convert_ascii_to_data(QString input, bool *ok);
    QByteArray convert_data_to_ascii(QByteArray data);

    uint8_t calc_limit(void);

    void delay(void);
    void wait(int timeout_msec);
};
//--------------------------------------------------------------------------------
#endif
