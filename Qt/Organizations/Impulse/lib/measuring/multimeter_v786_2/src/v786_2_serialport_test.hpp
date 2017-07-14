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
#ifndef SERIALPORT_V786_2_TEST_HPP
#define SERIALPORT_V786_2_TEST_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
class SerialPort_V786_2_test : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort_V786_2_test(QObject *parent = 0);
    ~SerialPort_V786_2_test();

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);

    bool setSerNo(uint16_t serno);
    bool setYear(uint16_t year);
    bool setAddress(uint8_t address);
    bool setChannel(uint8_t channel);

    uint16_t getSerNo(void);
    uint16_t getYear(void);
    uint8_t getAddress(void);
    uint8_t getChannel(void);

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

    bool is_open;
    float measuring_value;

    uint8_t data_0;

    uint16_t SerialNo;
    uint16_t SerialYear;
    uint8_t  Address;
    uint8_t  Channel;

    bool check_question_01(void);
    bool check_question_02(void);
    bool check_question_03(void);
    bool check_question_04(void);
    bool check_question_05(void);
    bool check_question_06(void);
    bool check_question_07(void);
    bool check_question_08(void);
    bool check_question_09(void);
    bool check_question_0A(void);
    bool check_question_0B(void);
    bool check_question_0C(void);
    bool check_question_0D(void);
    bool check_question_0E(void);
    bool check_question_0F(void);
    bool check_question_10(void);
    bool check_question_11(void);
    bool check_question_12(void);
    bool check_question_13(void);
    bool check_question_14(void);
    bool check_question_15(void);
    bool check_question_16(void);
    bool check_question_17(void);
    bool check_question_18(void);
    bool check_question_19(void);
    bool check_question_1A(void);
    bool check_question_1B(void);
    bool check_question_1C(void);
    bool check_question_1D(void);

    void prepare_answer_01(void);
    void prepare_answer_02(void);
    void prepare_answer_03(void);
    void prepare_answer_04(void);
    void prepare_answer_05(void);
    void prepare_answer_06(void);
    void prepare_answer_07(void);
    void prepare_answer_08(void);
    void prepare_answer_09(void);
    void prepare_answer_0A(void);
    void prepare_answer_0B(void);
    void prepare_answer_0C(void);
    void prepare_answer_0D(void);
    void prepare_answer_0E(void);
    void prepare_answer_0F(void);
    void prepare_answer_10(void);
    void prepare_answer_11(void);
    void prepare_answer_12(void);
    void prepare_answer_13(void);
    void prepare_answer_14(void);
    void prepare_answer_15(void);
    void prepare_answer_16(void);
    void prepare_answer_17(void);
    void prepare_answer_18(void);
    void prepare_answer_19(void);
    void prepare_answer_1A(void);
    void prepare_answer_1B(void);
    void prepare_answer_1C(void);
    void prepare_answer_1D(void);

    QByteArray convert_ascii_to_data(QString input, bool *ok);
    QByteArray convert_data_to_ascii(QByteArray data);

    unsigned char calc_limit(void);
};
//--------------------------------------------------------------------------------
#endif
