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
#ifndef SERIALPORT_B590_TEST_HPP
#define SERIALPORT_B590_TEST_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <stdint.h>
#include <QObject>
//--------------------------------------------------------------------------------
#include "B590_fram.hpp"
//--------------------------------------------------------------------------------
#define MIN_TEMPERATURE 20
#define MAX_TEMPERATURE 85
//--------------------------------------------------------------------------------
class B590_Fram;
class QPushButton;

class QDoubleSpinBox;
class QCheckBox;
class QSpinBox;
class QTimer;
//--------------------------------------------------------------------------------
class SerialPort_B590_test : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort_B590_test();
    virtual ~SerialPort_B590_test();

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);
    bool setStopBits(QSerialPort::StopBits);
    bool setParity(QSerialPort::Parity);
    bool setDataBits(QSerialPort::DataBits);
    bool setFlowControl(QSerialPort::FlowControl);
    bool open(int mode);
    bool clear(void);
    QByteArray readAll(void);

    void write(QByteArray data);
    void write(const char *data, int len);

signals:
    void readyRead(void);
    void s_error(QSerialPort::SerialPortError);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void create_fake_toolbar(void);
    void update(void);
    void regenerate_all(void);

    void set_R1(void);
    void set_R2(void);

    void set_voltage(void);
    void set_current(void);

private:
    QByteArray input;
    QByteArray output;

    int address;

    B590_Fram *fram = nullptr;
    QTimer *timer = nullptr;
    //---
    s_zero_b590_U zero_U;
    s_zero_b590_I zero_I;
    //---

    double factor_k_DAC;
    double factor_k_ADC;

    int dac_point;

    int32_t  setting_U;
    uint16_t setting_I;

    int32_t  current_U;
    uint16_t current_I;

    float temperature;

    uint16_t dac_U[0xFFFF]; //[MAX_CALIBRATION_POINTS_B590_U];
    uint16_t dac_I[0xFFFF]; //[MAX_CALIBRATION_POINTS_B590_I];

    QSpinBox *sb_noise = nullptr;
    QDoubleSpinBox *sb_R = nullptr;
    QPushButton *btn_set_noice = nullptr;

    QSpinBox *sb_voltage = nullptr;
    QSpinBox *sb_current = nullptr;
    //---

    bool is_open;

    void init_fram(void);

    void regenerate_dac_U_values(int noise_value);
    void regenerate_dac_I_values(int noise_value);

    bool check_question_11(void);
    bool check_question_12(void);
    bool check_question_13(void);
    bool check_question_14(void);
    bool check_question_15(void);
    bool check_question_16(void);
    bool check_question_17(void);

    bool check_question_41(void);
    bool check_question_42(void);
    bool check_question_46(void);
    bool check_question_47(void);
    bool check_question_48(void);
    bool check_question_49(void);
    bool check_question_4A(void);

    bool check_question_54(void);
    bool check_question_55(void);
    bool check_question_56(void);
    bool check_question_57(void);
    bool check_question_5A(void);
    bool check_question_5B(void);
    bool check_question_5C(void);
    bool check_question_5D(void);
    bool check_question_5E(void);
    bool check_question_5F(void);

    bool check_question_61(void);
    bool check_question_62(void);
    bool check_question_63(void);
    bool check_question_64(void);
    bool check_question_65(void);
    bool check_question_66(void);
    bool check_question_67(void);
    bool check_question_68(void);
    bool check_question_69(void);
    bool check_question_6A(void);
    bool check_question_6B(void);
    bool check_question_6C(void);
    bool check_question_6D(void);
    bool check_question_6E(void);
    bool check_question_6F(void);

    bool check_question_71(void);
    bool check_question_72(void);
    bool check_question_73(void);
    bool check_question_75(void);

    void prepare_answer_11(void);
    void prepare_answer_12(void);
    void prepare_answer_15(void);
    void prepare_answer_16(void);
    void prepare_answer_17(void);

    void prepare_answer_41(void);
    void prepare_answer_42(void);
    void prepare_answer_46(void);
    void prepare_answer_47(void);
    void prepare_answer_48(void);
    void prepare_answer_49(void);
    void prepare_answer_4A(void);

    void prepare_answer_54(void);
    void prepare_answer_55(void);
    void prepare_answer_56(void);
    void prepare_answer_57(void);
    void prepare_answer_5A(void);
    void prepare_answer_5B(void);
    void prepare_answer_5C(void);
    void prepare_answer_5D(void);
    void prepare_answer_5E(void);
    void prepare_answer_5F(void);

    void prepare_answer_61(void);
    void prepare_answer_62(void);
    void prepare_answer_63(void);
    void prepare_answer_64(void);
    void prepare_answer_65(void);
    void prepare_answer_66(void);
    void prepare_answer_67(void);
    void prepare_answer_68(void);
    void prepare_answer_69(void);
    void prepare_answer_6A(void);
    void prepare_answer_6B(void);
    void prepare_answer_6C(void);

    void secret_CMD_GET_ID(void);
    void secret_CMD_RESET(void);
    void secret_CMD_CLEAR_MOTOHOUR(void);
    void secret_CMD_SAVE_DATE_AND_SERNO(void);
    void secret_CMD_COPY_USER_TO_FACTORY(void);

    void secret_CMD_CLEAR_FRAM(void);
    void secret_CMD_READ_FRAM_8BIT(void);
    void secret_CMD_READ_FRAM_16BIT(void);
    void secret_CMD_READ_FRAM_32BIT(void);
    void secret_CMD_READ_FRAM_MORE_BYTES(void);
    void secret_CMD_SAVE_FRAM_8BIT(void);
    void secret_CMD_SAVE_FRAM_16BIT(void);
    void secret_CMD_SAVE_FRAM_32BIT(void);
    void secret_CMD_SAVE_FRAM_MORE_BYTES(void);

    void prepare_answer_6D(void);
    void prepare_answer_6E(void);
    void prepare_answer_6F(void);

    void prepare_answer_71(void);
    void prepare_answer_72(void);
    void prepare_answer_73(void);
    void prepare_answer_75(void);

    bool clear_profile(int number_profile);
    bool add_point_to_profile(int number_profile,
                              int32_t  U,
                              uint16_t I,
                              uint8_t Hour,
                              uint8_t Min,
                              uint8_t Sec);

    void calculate(int32_t  new_setting_U,
                   uint16_t new_setting_I);
};
//--------------------------------------------------------------------------------
#endif
