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
#include <QVariant>
#include <QtMath>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_serialport_test.hpp"
#include "v786_2_packets.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
SerialPort_V786_2_test::SerialPort_V786_2_test(QObject *parent) :
    QObject(parent)
{
    input.clear();
    output.clear();

    data_0 = 0;

    SerialNo    = 1;
    SerialYear  = 2013;

    is_open = false;
    measuring_value = 0;
}
//--------------------------------------------------------------------------------
SerialPort_V786_2_test::~SerialPort_V786_2_test()
{
#ifdef QT_DEBUG
    qDebug() << "~SerialPort_V786_2_test()";
#endif
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setSerNo(uint16_t serno)
{
    SerialNo = serno;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setYear(uint16_t year)
{
    SerialYear = year;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setAddress(uint8_t address)
{
    Address = address;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::setChannel(uint8_t channel)
{
    Channel = channel;
    return true;
}
//--------------------------------------------------------------------------------
uint16_t SerialPort_V786_2_test::getSerNo(void)
{
    return SerialNo;
}
//--------------------------------------------------------------------------------
uint16_t SerialPort_V786_2_test::getYear(void)
{
    return SerialYear;
}
//--------------------------------------------------------------------------------
uint8_t SerialPort_V786_2_test::getAddress(void)
{
    return Address;
}
//--------------------------------------------------------------------------------
uint8_t SerialPort_V786_2_test::getChannel(void)
{
    return Channel;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_2_test::readAll(void)
{
#ifdef QT_DEBUG
    qDebug() << "output" << output.toHex();
#endif
    return output;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::write(QByteArray data)
{
    if(data.isEmpty()) return;

    if(data == ":FF55AA55FF\r")
    {
        output.clear();
        output.append("don't worry, be happy");
        emit readyRead();
        return;
    }

    bool ok = false;
    input.clear();
    input.append(convert_ascii_to_data(data, &ok));
    if(!ok)
    {
#ifdef QT_DEBUG
        qDebug() << "error data" << data.toHex();
#endif
        return;
    }

    switch(input.at(1))
    {
    case V764_2_CMD_0x01: prepare_answer_01(); break;
    case V764_2_CMD_0x02: prepare_answer_02(); break;
    case V764_2_CMD_0x03: prepare_answer_03(); break;
    case V764_2_CMD_0x04: prepare_answer_04(); break;
    case V764_2_CMD_0x05: prepare_answer_05(); break;
    case V764_2_CMD_0x06: prepare_answer_06(); break;
    case V764_2_CMD_0x07: prepare_answer_07(); break;
    case V764_2_CMD_0x08: prepare_answer_08(); break;
    case V764_2_CMD_0x09: prepare_answer_09(); break;
    case V764_2_CMD_0x0A: prepare_answer_0A(); break;
    case V764_2_CMD_0x0B: prepare_answer_0B(); break;
    case V764_2_CMD_0x0C: prepare_answer_0C(); break;
    case V764_2_CMD_0x0D: prepare_answer_0D(); break;
    case V764_2_CMD_0x0E: prepare_answer_0E(); break;
    case V764_2_CMD_0x0F: prepare_answer_0F(); break;
    case V764_2_CMD_0x10: prepare_answer_10(); break;
    case V764_2_CMD_0x11: prepare_answer_11(); break;
    case V764_2_CMD_0x12: prepare_answer_12(); break;
    case V764_2_CMD_0x13: prepare_answer_13(); break;
    case V764_2_CMD_0x14: prepare_answer_14(); break;
    case V764_2_CMD_0x15: prepare_answer_15(); break;
    case V764_2_CMD_0x16: prepare_answer_16(); break;
    case V764_2_CMD_0x17: prepare_answer_17(); break;
    case V764_2_CMD_0x18: prepare_answer_18(); break;
    case V764_2_CMD_0x19: prepare_answer_19(); break;
    case V764_2_CMD_0x1A: prepare_answer_1A(); break;
    case V764_2_CMD_0x1B: prepare_answer_1B(); break;
    case V764_2_CMD_0x1C: prepare_answer_1C(); break;
    case V764_2_CMD_0x1D: prepare_answer_1D(); break;

    default:
#ifdef QT_DEBUG
        qDebug() << "unknown cmd" << data.at(1);
#endif
        break;
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::write(const char *data, int len)
{
#ifdef QT_DEBUG
    qDebug() << "write";
#endif

    QByteArray ba;
    ba.clear();
    ba.append(data, len);
    write(ba);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_01(void)
{
    //#define V764_2_CMD_0x01         0x01
    //#define V764_2_CMD_0x01_TEXT    tr("Запрос статуса прибора.")

    V764_2_01_QUESTION *question = (V764_2_01_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_01_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_01: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_02(void)
{
    //#define CMD_REMOTE_CONTROL_ON               0x02
    //#define CMD_REMOTE_CONTROL_ON_TEXT          tr("Включение удаленного управления")

    V764_2_02_QUESTION *question = (V764_2_02_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_02_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_02: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_03(void)
{
    //#define CMD_REMOTE_CONTROL_OFF              0x03
    //#define CMD_REMOTE_CONTROL_OFF_TEXT         tr("Отключение удаленного управления")

    V764_2_03_QUESTION *question = (V764_2_03_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_03_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_03: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_04(void)
{
    //#define CMD_SERVICE_ON                      0x04
    //#define CMD_SERVICE_ON_TEXT                 tr("Включение сервисных команд (защищено паролем)")

    V764_2_04_QUESTION *question = (V764_2_04_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_04_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_04: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_05(void)
{
    //#define CMD_POWER_OFF                       0x05
    //#define CMD_POWER_OFF_TEXT                  tr("Отключение питания прибора")

    V764_2_05_QUESTION *question = (V764_2_05_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_05_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_05: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_06(void)
{
    //#define CMD_MEASURING_UDC                   0x06
    //#define CMD_MEASURING_UDC_TEXT              tr("Измерение напряжения постоянного тока")

    V764_2_06_QUESTION *question = (V764_2_06_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_06_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_06: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_07(void)
{
    //#define CMD_MEASURING_UAC                   0x07
    //#define CMD_MEASURING_UAC_TEXT              tr("Измерение напряжения переменного тока")

    V764_2_07_QUESTION *question = (V764_2_07_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_07_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_07: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_08(void)
{
    //#define CMD_MEASURING_IDC                   0x08
    //#define CMD_MEASURING_IDC_TEXT              tr("Измерение силы постоянного тока")

    V764_2_08_QUESTION *question = (V764_2_08_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_08_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_08: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_09(void)
{
    //#define CMD_MEASURING_IAC                   0x09
    //#define CMD_MEASURING_IAC_TEXT              tr("Измерение силы переменного тока")

    V764_2_09_QUESTION *question = (V764_2_09_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_09_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_09: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_0A(void)
{
    //#define CMD_MEASURING_R                     0x0A
    //#define CMD_MEASURING_R_TEXT                tr("Измерение сопротивления")

    V764_2_0A_QUESTION *question = (V764_2_0A_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_0A_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0A: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_0B(void)
{
    //#define CMD_MEASURING_C                     0x0B
    //#define CMD_MEASURING_C_TEXT                tr("Измерение емкости")

    V764_2_0B_QUESTION *question = (V764_2_0B_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_0B_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0B: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_0C(void)
{
    //#define CMD_MEASURING_L                     0x0C
    //#define CMD_MEASURING_L_TEXT                tr("Измерение индуктивности")

    V764_2_0C_QUESTION *question = (V764_2_0C_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_0C_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0C: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_0D(void)
{
    //#define CMD_CONTINUITY                      0x0D
    //#define CMD_CONTINUITY_TEXT                 tr("Прозвонка цепи")

    V764_2_0D_QUESTION *question = (V764_2_0D_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_0D_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0D: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_0E(void)
{
    //#define CMD_MEASURING_F                     0x0E
    //#define CMD_MEASURING_F_TEXT                tr("Измерение частоты")

    V764_2_0E_QUESTION *question = (V764_2_0E_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_0E_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0E: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_0F(void)
{
    //#define CMD_MEASURING_P                     0x0F
    //#define CMD_MEASURING_P_TEXT                tr("Измерение активной мощности")

    V764_2_0F_QUESTION *question = (V764_2_0F_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_0F_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0F: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_10(void)
{
    //#define CMD_MEASURING_Q                     0x10
    //#define CMD_MEASURING_Q_TEXT                tr("Измерение реактивной мощности")

    V764_2_10_QUESTION *question = (V764_2_10_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_10_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_10: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_11(void)
{
    //#define CMD_MEASURING_S                     0x11
    //#define CMD_MEASURING_S_TEXT                tr("Измерение полной мощности")

    V764_2_11_QUESTION *question = (V764_2_11_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_11_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_11: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_12(void)
{
    //#define CMD_MEASURING_PF                    0x12
    //#define CMD_MEASURING_PF_TEXT               tr("Измерение коэффициента мощности")

    V764_2_12_QUESTION *question = (V764_2_12_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_12_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_12: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_13(void)
{
    //#define CMD_CALIBRATION_UDC                 0x13
    //#define CMD_CALIBRATION_UDC_TEXT            tr("Калибровка напряжения постоянного тока")

    V764_2_13_QUESTION *question = (V764_2_13_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_13_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_13: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_14(void)
{
    //#define CMD_CALIBRATION_UAC                 0x14
    //#define CMD_CALIBRATION_UAC_TEXT            tr("Калибровка напряжения переменного тока")

    V764_2_14_QUESTION *question = (V764_2_14_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_14_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_14: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_15(void)
{
    //#define CMD_CALIBRATION_IDC                 0x15
    //#define CMD_CALIBRATION_IDC_TEXT            tr("Калибровка силы постоянного тока")

    V764_2_15_QUESTION *question = (V764_2_15_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_15_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_15: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_16(void)
{
    //#define CMD_CALIBRATION_IAC                 0x16
    //#define CMD_CALIBRATION_IAC_TEXT            tr("Калибровка силы переменного тока")

    V764_2_16_QUESTION *question = (V764_2_16_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_16_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_16: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_17(void)
{
    //#define CMD_CALIBRATION_R                   0x17
    //#define CMD_CALIBRATION_R_TEXT              tr("Калибровка сопротивления")

    V764_2_17_QUESTION *question = (V764_2_17_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_17_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_17: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_18(void)
{
    //#define CMD_ENABLING_CHARGE                 0x18
    //#define CMD_ENABLING_CHARGE_TEXT            tr("Включение режима заряда АКБ")

    V764_2_18_QUESTION *question = (V764_2_18_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_18_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_18: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_19(void)
{
    //#define CMD_DISABLING_CHARGE                0x19
    //#define CMD_DISABLING_CHARGE_TEXT           tr("Выключение режима заряда АКБ")

    V764_2_19_QUESTION *question = (V764_2_19_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_19_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_19: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_1A(void)
{
    //#define CMD_REQUEST_SETTINGS_CHARGE         0x1A
    //#define CMD_REQUEST_SETTINGS_CHARGE_TEXT    tr("Запрос параметров заряда АКБ")

    V764_2_1A_QUESTION *question = (V764_2_1A_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_1A_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_1A: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_1B(void)
{
    //#define CMD_WRITE_TO_FLASH                  0x1B
    //#define CMD_WRITE_TO_FLASH_TEXT             tr("Запись коэффициентов во flash")

    V764_2_1B_QUESTION *question = (V764_2_1B_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_1B_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_1B: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_1C(void)
{
    //#define CMD_GET_ID                          0x1C
    //#define CMD_GET_ID_TEXT                     tr("Получение ID")

    V764_2_1C_QUESTION *question = (V764_2_1C_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_1C_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_1C: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_2_test::check_question_1D(void)
{
    //#define CMD_WIRED_OFF                       0x1D
    //#define CMD_WIRED_OFF_TEXT                  tr("Выключение проводного интерфейса")

    V764_2_1D_QUESTION *question = (V764_2_1D_QUESTION *)input.data();

    unsigned short crc = CRC::multimeter_crc16((unsigned char *)&question->buf, sizeof(V764_2_1D_QUESTION) - 3);
    unsigned short data_crc = question->body.crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_1D: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_01(void)
{
    if(!check_question_01()) return;

    V764_2_01_ANSWER answer;
    V764_2_01_QUESTION *question = (V764_2_01_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif
    answer.body.data.Range       = 0; // предел
    answer.body.data.Mode        = 0; // режим
    answer.body.data.Math_Hold   = 0;
    answer.body.data.BattVoltage = 0;
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_01_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_01_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_02(void)
{
    if(!check_question_02()) return;

    V764_2_02_ANSWER answer;
    V764_2_02_QUESTION *question = (V764_2_02_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_02_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_02_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_03(void)
{
    if(!check_question_03()) return;

    V764_2_03_ANSWER answer;
    V764_2_03_QUESTION *question = (V764_2_03_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_03_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_03_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_04(void)
{
    if(!check_question_04()) return;

    V764_2_04_ANSWER answer;
    V764_2_04_QUESTION *question = (V764_2_04_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_04_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_04_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_05(void)
{
    if(!check_question_05()) return;

    V764_2_05_ANSWER answer;
    V764_2_05_QUESTION *question = (V764_2_05_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_05_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_05_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_06(void)
{
    if(!check_question_06()) return;

    V764_2_06_QUESTION *question = (V764_2_06_QUESTION *)input.data();
    V764_2_06_ANSWER answer;

    measuring_value = Connection::get_uint16_value();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';

#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_06_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_06_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_07(void)
{
    if(!check_question_07()) return;

    V764_2_07_QUESTION *question = (V764_2_07_QUESTION *)input.data();
    V764_2_07_ANSWER answer;

    data_0 = question->body.data.data_0;

    measuring_value = Connection::get_uint16_value();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.data.limits = data_0 & 0x7;
    answer.body.data.deviation_retention = data_0 & 0xC0;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_07_ANSWER) - 2);

    output.clear();
    output.append((const char *)&answer.buf, sizeof(V764_2_07_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_08(void)
{
    if(!check_question_08()) return;

    V764_2_08_ANSWER answer;
    V764_2_08_QUESTION *question = (V764_2_08_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_08_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_08_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_09(void)
{
    if(!check_question_09()) return;

    V764_2_09_ANSWER answer;
    V764_2_09_QUESTION *question = (V764_2_09_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_09_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_09_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_0A(void)
{
    if(!check_question_0A()) return;

    V764_2_0A_ANSWER answer;
    V764_2_0A_QUESTION *question = (V764_2_0A_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_0A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_0A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_0B(void)
{
    if(!check_question_0B()) return;

    V764_2_0B_ANSWER answer;
    V764_2_0B_QUESTION *question = (V764_2_0B_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_0B_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_0B_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_0C(void)
{
    if(!check_question_0C()) return;

    V764_2_0C_ANSWER answer;
    V764_2_0C_QUESTION *question = (V764_2_0C_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_0C_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_0C_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_0D(void)
{
    if(!check_question_0D()) return;

    V764_2_0D_ANSWER answer;
    V764_2_0D_QUESTION *question = (V764_2_0D_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_0D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_0D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_0E(void)
{
    if(!check_question_0E()) return;

    V764_2_0E_ANSWER answer;
    V764_2_0E_QUESTION *question = (V764_2_0E_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_0E_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_0E_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_0F(void)
{
    if(!check_question_0F()) return;

    V764_2_0F_ANSWER answer;
    V764_2_0F_QUESTION *question = (V764_2_0F_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_0F_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_0F_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_10(void)
{
    if(!check_question_10()) return;

    V764_2_10_ANSWER answer;
    V764_2_10_QUESTION *question = (V764_2_10_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_10_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_10_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_11(void)
{
    if(!check_question_11()) return;

    V764_2_11_ANSWER answer;
    V764_2_11_QUESTION *question = (V764_2_11_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_11_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_11_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_12(void)
{
    if(!check_question_12()) return;

    V764_2_12_ANSWER answer;
    V764_2_12_QUESTION *question = (V764_2_12_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.deviation_retention = 0;
    answer.body.data.limits = calc_limit();
    for(int n=0; n<7; n++) answer.body.data.DisplaySymbols[n] = ' ';
#if 1
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp;
    tmp.clear();
    tmp.append(temp);
    //if(measuring_value < 0) answer.body.data.DisplaySymbols[0] = '-';
    memcpy(&answer.body.data.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
#endif

    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_12_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_12_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_13(void)
{
    if(!check_question_13()) return;

    V764_2_13_ANSWER answer;
    V764_2_13_QUESTION *question = (V764_2_13_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_13_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_13_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_14(void)
{
    if(!check_question_14()) return;

    V764_2_14_ANSWER answer;
    V764_2_14_QUESTION *question = (V764_2_14_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_14_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_14_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_15(void)
{
    if(!check_question_15()) return;

    V764_2_15_ANSWER answer;
    V764_2_15_QUESTION *question = (V764_2_15_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_15_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_15_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_16(void)
{
    if(!check_question_16()) return;

    V764_2_16_ANSWER answer;
    V764_2_16_QUESTION *question = (V764_2_16_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_16_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_16_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_17(void)
{
    if(!check_question_17()) return;

    V764_2_17_ANSWER answer;
    V764_2_17_QUESTION *question = (V764_2_17_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_17_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_17_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_18(void)
{
    if(!check_question_18()) return;

    V764_2_18_ANSWER answer;
    V764_2_18_QUESTION *question = (V764_2_18_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_18_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_18_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_19(void)
{
    if(!check_question_19()) return;

    V764_2_19_ANSWER answer;
    V764_2_19_QUESTION *question = (V764_2_19_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_19_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_19_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_1A(void)
{
    if(!check_question_1A()) return;

    V764_2_1A_ANSWER answer;
    V764_2_1A_QUESTION *question = (V764_2_1A_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_1A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_1A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_1B(void)
{
    if(!check_question_1B()) return;

    V764_2_1B_ANSWER answer;
    V764_2_1B_QUESTION *question = (V764_2_1B_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_1B_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_1B_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_1C(void)
{
    if(!check_question_1C()) return;

    V764_2_1C_ANSWER answer;
    V764_2_1C_QUESTION *question = (V764_2_1C_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_1C_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_1C_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::prepare_answer_1D(void)
{
    if(!check_question_1D()) return;

    V764_2_1D_ANSWER answer;
    V764_2_1D_QUESTION *question = (V764_2_1D_QUESTION *)input.data();

    answer.body.header.address = question->body.header.address;
    answer.body.header.cmd = question->body.header.cmd;
    answer.body.data.serial_year.SerialNo    = SerialNo;
    answer.body.data.serial_year.SerialYear  = SerialYear;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_1D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(V764_2_1D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_2_test::convert_ascii_to_data(QString input, bool *ok)
{
    QByteArray temp_ba;

    int begin = input.indexOf(':');
    if(begin < 0)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    int end = input.indexOf('\r');
    if(end < 0)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    if(end < begin)
    {
        temp_ba.clear();
        temp_ba.append(input);

        *ok = false;
        return temp_ba;
    }

    temp_ba = QByteArray::fromHex(input.toLocal8Bit());

    *ok = true;
    return temp_ba;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_2_test::convert_data_to_ascii(QByteArray data)
{
    QByteArray temp;

    temp.clear();
    temp.append(':');
    temp.append(data.toHex());
    temp.append((char)0x0D);

#ifdef QT_DEBUG
    qDebug() << "convert_data_to_ascii" << temp.data();
#endif
    return temp;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test::receive(const QVariant &data)
{
    bool ok = false;
    float temp = data.toFloat(&ok);
    if(ok)
    {
        measuring_value = temp * 1000.0f;
#ifdef QT_DEBUG
        qDebug() << "measuring_value" << measuring_value;
#endif
    }
}
//--------------------------------------------------------------------------------
unsigned char SerialPort_V786_2_test::calc_limit(void)
{
//    #define L_200mkA    0
//    #define L_2mA       1
//    #define L_20mA      2
//    #define L_200mA     3
//    #define L_2000mA    4
//    #define L_20A       5

    if(qAbs(measuring_value) < 200)
    {
        return L_200mkA;
    }
    if(qAbs(measuring_value) < 2000)
    {
        measuring_value /= 1000.0f;
        return L_2mA;
    }
    if(qAbs(measuring_value) < 20000)
    {
        measuring_value /= 1000.0f;
        return L_20mA;
    }
    if(qAbs(measuring_value) < 200000)
    {
        measuring_value /= 1000.0f;
        return L_200mA;
    }
    if(qAbs(measuring_value) < 2000000)
    {
        measuring_value /= 1000.0f;
        return L_2000mA;
    }

    measuring_value /= 1000000.0f;
    return L_20A;
}
//--------------------------------------------------------------------------------
