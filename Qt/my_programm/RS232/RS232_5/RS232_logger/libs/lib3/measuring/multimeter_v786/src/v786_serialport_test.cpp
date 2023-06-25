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
#include <QVariant>
#include <QtMath>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "v786_multimeter.hpp"
#include "v786_serialport_test.hpp"
#include "v786_packets.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
SerialPort_V786_test::SerialPort_V786_test(QObject *parent) :
    QObject(parent)
{
    ba_input.clear();
    ba_output.clear();

    is_open = false;
    measuring_value = 0;
}
//--------------------------------------------------------------------------------
SerialPort_V786_test::~SerialPort_V786_test()
{
#ifdef QT_DEBUG
    qDebug() << "~SerialPort_V786_test()";
#endif
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::setSerNo(uint16_t serno)
{
    SerialNo = serno;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::setYear(uint16_t year)
{
    SerialYear = year;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::setAddress(uint8_t address)
{
    Address = address;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::setChannel(uint8_t channel)
{
    Channel = channel;
    return true;
}
//--------------------------------------------------------------------------------
uint16_t SerialPort_V786_test::getSerNo(void)
{
    return SerialNo;
}
//--------------------------------------------------------------------------------
uint16_t SerialPort_V786_test::getYear(void)
{
    return SerialYear;
}
//--------------------------------------------------------------------------------
uint8_t SerialPort_V786_test::getAddress(void)
{
    return Address;
}
//--------------------------------------------------------------------------------
uint8_t SerialPort_V786_test::getChannel(void)
{
    return Channel;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_test::readAll(void)
{
#ifdef QT_DEBUG
    qDebug() << "output" << ba_output.toHex();
#endif
    return ba_output;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::write(const QByteArray &data)
{
    if(data.isEmpty()) return;

    if(data == ":FF55AA55FF\r")
    {
        ba_output.clear();
        ba_output.append("don't worry, be happy");
        emit readyRead();
        return;
    }

    bool ok = false;
    ba_input.clear();
    ba_input.append(convert_ascii_to_data(data, &ok));
    if(!ok)
    {
#ifdef QT_DEBUG
        qDebug() << "error data" << data.toHex();
#endif
        return;
    }

    switch(ba_input.at(1))
    {
    case V764_STATUS_REQUEST: prepare_answer_01(); break;
    case V764_REMOTE_CONTROL_ENABLE: prepare_answer_02(); break;
    case V764_REMOTE_CONTROL_DISABLE: prepare_answer_03(); break;
    case V764_SERVICE_ENABLE: prepare_answer_04(); break;
    case V764_POWER_OFF: prepare_answer_05(); break;
    case V764_DC_VOLTAGE_MEASUREMENT: prepare_answer_06(); break;
    case V764_AC_VOLTAGE_MEASUREMENT: prepare_answer_07(); break;
    case V764_DC_CURRENT_MEASUREMENT: prepare_answer_08(); break;
    case V764_AC_CURRENT_MEASUREMENT: prepare_answer_09(); break;
    case V764_RESISTANCE_MEASUREMENT: prepare_answer_0A(); break;
    case V764_CAPACITANCE_MEASUREMENT: prepare_answer_0B(); break;
    case V764_INDUCTANCE_MEASUREMENT: prepare_answer_0C(); break;
    case V764_BUZZER_TEST: prepare_answer_0D(); break;
    case V764_FREQUENCY_MEASUREMENT: prepare_answer_0E(); break;
    case V764_ACTIVE_POWER_MEASUREMENT: prepare_answer_0F(); break;
    case V764_REACTIVE_POWER_MEASUREMENT: prepare_answer_10(); break;
    case V764_FULL_POWER_MEASUREMENT: prepare_answer_11(); break;
    case V764_COS_FI_MEASUREMENT: prepare_answer_12(); break;
    case V764_UDC_CAL: prepare_answer_13(); break;
    case V764_UAC_CAL: prepare_answer_14(); break;
    case V764_IDC_CAL: prepare_answer_15(); break;
    case V764_IAC_CAL: prepare_answer_16(); break;
    case V764_RES_CAL: prepare_answer_17(); break;
    case V764_FLASH_WRITE: prepare_answer_1B(); break;
    case V764_SERVICE_DISABLE: prepare_answer_1C(); break;
    case V764_DIODE_TEST: prepare_answer_1D(); break;

    default:
#ifdef QT_DEBUG
        qDebug() << "unknown cmd" << data.at(1);
#endif
        break;
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::write(const char *data, int len)
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
void SerialPort_V786_test::print_log(const QString &text)
{
    QString temp;
    temp.append("### Б7-86: ");
    temp.append(text);

    emit info(temp);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::print_err_log(const QString &text)
{
    QString temp;
    temp.append("### Б7-86: ");
    temp.append(text);

    emit error(temp);
}
//--------------------------------------------------------------------------------
template<typename T1>
bool SerialPort_V786_test::check_packet(void)
{
    T1 *question = reinterpret_cast<T1 *>(ba_input.data());
    Q_ASSERT(question);

    uint16_t crc = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(question),
                                         sizeof(*question) - SIZE_CRC);
    uint16_t data_crc = question->crc16;
    if(crc != data_crc)
    {
#ifdef QT_DEBUG
        qDebug() << "check_packet: crc" << QString::number(crc, 16) << "crc16" << QString::number(data_crc, 16);
#endif
        print_err_log(QString("  crc: %1").arg(crc, 2, 16, QChar('0')));
        print_err_log(QString("crc16: %1").arg(data_crc, 2, 16, QChar('0')));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_01(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_STATUS_REQUEST_TEXT));
    bool ok = check_packet<V764_STATUS_REQUEST_Q>();
    if(ok)
    {
        V764_STATUS_REQUEST_Q *question = reinterpret_cast<V764_STATUS_REQUEST_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;

}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_02(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_REMOTE_CONTROL_ENABLE_TEXT));
    bool ok = check_packet<V764_REMOTE_CONTROL_ENABLE_Q>();
    if(ok)
    {
        V764_REMOTE_CONTROL_ENABLE_Q *question = reinterpret_cast<V764_REMOTE_CONTROL_ENABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_03(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_REMOTE_CONTROL_DISABLE_TEXT));
    bool ok = check_packet<V764_REMOTE_CONTROL_DISABLE_Q>();
    if(ok)
    {
        V764_REMOTE_CONTROL_DISABLE_Q *question = reinterpret_cast<V764_REMOTE_CONTROL_DISABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_04(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_SERVICE_ENABLE_TEXT));
    bool ok = check_packet<V764_SERVICE_ENABLE_Q>();
    if(ok)
    {
        V764_SERVICE_ENABLE_Q *question = reinterpret_cast<V764_SERVICE_ENABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_05(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_POWER_OFF_TEXT));
    bool ok = check_packet<V764_POWER_OFF_Q>();
    if(ok)
    {
        V764_POWER_OFF_Q *question = reinterpret_cast<V764_POWER_OFF_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_06(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_DC_VOLTAGE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_DC_VOLTAGE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_DC_VOLTAGE_MEASUREMENT_Q *question = reinterpret_cast<V764_DC_VOLTAGE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_07(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_AC_VOLTAGE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_AC_VOLTAGE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_AC_VOLTAGE_MEASUREMENT_Q *question = reinterpret_cast<V764_AC_VOLTAGE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_08(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_DC_CURRENT_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_DC_CURRENT_MEASUREMENT_Q>();
    if(ok)
    {
        V764_DC_CURRENT_MEASUREMENT_Q *question = reinterpret_cast<V764_DC_CURRENT_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_09(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_AC_CURRENT_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_AC_CURRENT_MEASUREMENT_Q>();
    if(ok)
    {
        V764_AC_CURRENT_MEASUREMENT_Q *question = reinterpret_cast<V764_AC_CURRENT_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_0A(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_RESISTANCE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_RESISTANCE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_RESISTANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_RESISTANCE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_0B(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_CAPACITANCE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_CAPACITANCE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_CAPACITANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_CAPACITANCE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_0C(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_INDUCTANCE_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_INDUCTANCE_MEASUREMENT_Q>();
    if(ok)
    {
        V764_INDUCTANCE_MEASUREMENT_Q *question = reinterpret_cast<V764_INDUCTANCE_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_0D(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_BUZZER_TEST_TEXT));
    bool ok = check_packet<V764_BUZZER_TEST_Q>();
    if(ok)
    {
        V764_BUZZER_TEST_Q *question = reinterpret_cast<V764_BUZZER_TEST_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_0E(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_FREQUENCY_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_FREQUENCY_MEASUREMENT_Q>();
    if(ok)
    {
        V764_FREQUENCY_MEASUREMENT_Q *question = reinterpret_cast<V764_FREQUENCY_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_0F(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_ACTIVE_POWER_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_ACTIVE_POWER_MEASUREMENT_Q>();
    if(ok)
    {
        V764_ACTIVE_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_ACTIVE_POWER_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_10(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_REACTIVE_POWER_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_REACTIVE_POWER_MEASUREMENT_Q>();
    if(ok)
    {
        V764_REACTIVE_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_REACTIVE_POWER_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_11(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_FULL_POWER_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_FULL_POWER_MEASUREMENT_Q>();
    if(ok)
    {
        V764_FULL_POWER_MEASUREMENT_Q *question = reinterpret_cast<V764_FULL_POWER_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_12(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_COS_FI_MEASUREMENT_TEXT));
    bool ok = check_packet<V764_COS_FI_MEASUREMENT_Q>();
    if(ok)
    {
        V764_COS_FI_MEASUREMENT_Q *question = reinterpret_cast<V764_COS_FI_MEASUREMENT_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_13(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_UDC_CAL_TEXT));
    bool ok = check_packet<V764_UDC_CAL_Q>();
    if(ok)
    {
        V764_UDC_CAL_Q *question = reinterpret_cast<V764_UDC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_14(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_UAC_CAL_TEXT));
    bool ok = check_packet<V764_UAC_CAL_Q>();
    if(ok)
    {
        V764_UAC_CAL_Q *question = reinterpret_cast<V764_UAC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_15(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_IDC_CAL_TEXT));
    bool ok = check_packet<V764_IDC_CAL_Q>();
    if(ok)
    {
        V764_IDC_CAL_Q *question = reinterpret_cast<V764_IDC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_16(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_IAC_CAL_TEXT));
    bool ok = check_packet<V764_IAC_CAL_Q>();
    if(ok)
    {
        V764_IAC_CAL_Q *question = reinterpret_cast<V764_IAC_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_17(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_RES_CAL_TEXT));
    bool ok = check_packet<V764_RES_CAL_Q>();
    if(ok)
    {
        V764_RES_CAL_Q *question = reinterpret_cast<V764_RES_CAL_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_1B(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_FLASH_WRITE_TEXT));
    bool ok = check_packet<V764_FLASH_WRITE_Q>();
    if(ok)
    {
        V764_FLASH_WRITE_Q *question = reinterpret_cast<V764_FLASH_WRITE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_1C(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_SERVICE_DISABLE_TEXT));
    bool ok = check_packet<V764_SERVICE_DISABLE_Q>();
    if(ok)
    {
        V764_SERVICE_DISABLE_Q *question = reinterpret_cast<V764_SERVICE_DISABLE_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool SerialPort_V786_test::check_question_1D(void)
{
    print_log(QString("Проверяем пакет %1").arg(V764_DIODE_TEST_TEXT));
    bool ok = check_packet<V764_DIODE_TEST_Q>();
    if(ok)
    {
        V764_DIODE_TEST_Q *question = reinterpret_cast<V764_DIODE_TEST_Q *>(ba_input.data());
        Q_ASSERT(question);

        print_log("Пришел пакет");
        print_log(QString("   address: %1").arg(question->header.address));
        print_log(QString("   cmd: %1").arg(question->header.cmd));
        print_log(QString("   cnt: %1").arg(question->header.count_data));
        print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
        print_log(QString("   channel: %1").arg(question->channel));
    }
    return ok;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_01(void)
{
    if(!check_question_01()) return;

    V764_STATUS_REQUEST_Q *question = (V764_STATUS_REQUEST_Q *)ba_input.data();
    V764_STATUS_REQUEST_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());
    answer.Range       = 0; // предел
    answer.Mode        = 0; // режим
    answer.Math_Hold   = 0;
    answer.BattVoltage = 0;
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_02(void)
{
    if(!check_question_02()) return;

    V764_REMOTE_CONTROL_ENABLE_Q *question = (V764_REMOTE_CONTROL_ENABLE_Q *)ba_input.data();
    V764_REMOTE_CONTROL_ENABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_03(void)
{
    if(!check_question_03()) return;

    V764_REMOTE_CONTROL_DISABLE_Q *question = (V764_REMOTE_CONTROL_DISABLE_Q *)ba_input.data();
    V764_REMOTE_CONTROL_DISABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_04(void)
{
    if(!check_question_04()) return;

    V764_SERVICE_ENABLE_Q *question = (V764_SERVICE_ENABLE_Q *)ba_input.data();
    V764_SERVICE_ENABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_05(void)
{
    if(!check_question_05()) return;

    V764_POWER_OFF_Q *question = (V764_POWER_OFF_Q *)ba_input.data();
    V764_POWER_OFF_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_06(void)
{
    if(!check_question_06()) return;

    V764_DC_VOLTAGE_MEASUREMENT_Q *question = (V764_DC_VOLTAGE_MEASUREMENT_Q *)ba_input.data();
    V764_DC_VOLTAGE_MEASUREMENT_A answer;

    measuring_value = Connection::get_uint16_value();

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';

    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_07(void)
{
    if(!check_question_07()) return;

    V764_AC_VOLTAGE_MEASUREMENT_Q *question = (V764_AC_VOLTAGE_MEASUREMENT_Q *)ba_input.data();
    V764_AC_VOLTAGE_MEASUREMENT_A answer;

    data_0 = question->data_0;

    measuring_value = Connection::get_uint16_value();

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.limits = data_0 & 0x7;
    answer.deviation_retention = data_0 & 0xC0;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_08(void)
{
    if(!check_question_08()) return;

    V764_DC_CURRENT_MEASUREMENT_Q *question = (V764_DC_CURRENT_MEASUREMENT_Q *)ba_input.data();
    V764_DC_CURRENT_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_09(void)
{
    if(!check_question_09()) return;

    V764_AC_CURRENT_MEASUREMENT_Q *question = (V764_AC_CURRENT_MEASUREMENT_Q *)ba_input.data();
    V764_AC_CURRENT_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_0A(void)
{
    if(!check_question_0A()) return;

    V764_RESISTANCE_MEASUREMENT_Q *question = (V764_RESISTANCE_MEASUREMENT_Q *)ba_input.data();
    V764_RESISTANCE_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_0B(void)
{
    if(!check_question_0B()) return;

    V764_CAPACITANCE_MEASUREMENT_Q *question = (V764_CAPACITANCE_MEASUREMENT_Q *)ba_input.data();
    V764_CAPACITANCE_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_0C(void)
{
    if(!check_question_0C()) return;

    V764_INDUCTANCE_MEASUREMENT_Q *question = (V764_INDUCTANCE_MEASUREMENT_Q *)ba_input.data();
    V764_INDUCTANCE_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_0D(void)
{
    if(!check_question_0D()) return;

    V764_BUZZER_TEST_Q *question = (V764_BUZZER_TEST_Q *)ba_input.data();
    V764_BUZZER_TEST_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_0E(void)
{
    if(!check_question_0E()) return;

    V764_FREQUENCY_MEASUREMENT_Q *question = (V764_FREQUENCY_MEASUREMENT_Q *)ba_input.data();
    V764_FREQUENCY_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_0F(void)
{
    if(!check_question_0F()) return;

    V764_ACTIVE_POWER_MEASUREMENT_Q *question = (V764_ACTIVE_POWER_MEASUREMENT_Q *)ba_input.data();
    V764_ACTIVE_POWER_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_10(void)
{
    if(!check_question_10()) return;

    V764_REACTIVE_POWER_MEASUREMENT_Q *question = (V764_REACTIVE_POWER_MEASUREMENT_Q *)ba_input.data();
    V764_REACTIVE_POWER_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_11(void)
{
    if(!check_question_11()) return;

    V764_FULL_POWER_MEASUREMENT_Q *question = (V764_FULL_POWER_MEASUREMENT_Q *)ba_input.data();
    V764_FULL_POWER_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_12(void)
{
    if(!check_question_12()) return;

    V764_COS_FI_MEASUREMENT_Q *question = (V764_COS_FI_MEASUREMENT_Q *)ba_input.data();
    V764_COS_FI_MEASUREMENT_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);

    answer.deviation_retention = 0;
    answer.limits = calc_limit();
    for(int n=0; n<7; n++) answer.DisplaySymbols[n] = ' ';
    QString temp = QString("%1").arg(measuring_value);
    QByteArray tmp = temp.toUtf8();
    //if(measuring_value < 0) answer.DisplaySymbols[0] = '-';
    memcpy(&answer.DisplaySymbols[7-tmp.size()], tmp.constData(), tmp.size());

    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_13(void)
{
    if(!check_question_13()) return;

    V764_UDC_CAL_Q *question = (V764_UDC_CAL_Q *)ba_input.data();
    V764_UDC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_14(void)
{
    if(!check_question_14()) return;

    V764_UAC_CAL_Q *question = (V764_UAC_CAL_Q *)ba_input.data();
    V764_UAC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_15(void)
{
    if(!check_question_15()) return;

    V764_IDC_CAL_Q *question = (V764_IDC_CAL_Q *)ba_input.data();
    V764_IDC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_16(void)
{
    if(!check_question_16()) return;

    V764_IAC_CAL_Q *question = (V764_IAC_CAL_Q *)ba_input.data();
    V764_IAC_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_17(void)
{
    if(!check_question_17()) return;

    V764_RES_CAL_Q *question = (V764_RES_CAL_Q *)ba_input.data();
    V764_RES_CAL_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_1B(void)
{
    if(!check_question_1B()) return;

    V764_FLASH_WRITE_Q *question = (V764_FLASH_WRITE_Q *)ba_input.data();
    V764_FLASH_WRITE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_1C(void)
{
    if(!check_question_1C()) return;

    V764_SERVICE_DISABLE_Q *question = (V764_SERVICE_DISABLE_Q *)ba_input.data();
    V764_SERVICE_DISABLE_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V786_test::prepare_answer_1D(void)
{
    if(!check_question_1D()) return;

    V764_DIODE_TEST_Q *question = (V764_DIODE_TEST_Q *)ba_input.data();
    V764_DIODE_TEST_A answer;

    answer.header.address = question->header.address;
    answer.header.cmd = question->header.cmd;
    answer.serial_year.SerialNo    = SerialNo;
    answer.serial_year.SerialYear  = SerialYear;
    answer.header.count_data = sizeof(answer);
    answer.crc16 = CRC::multimeter_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit readyRead();
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_test::convert_ascii_to_data(QString input, bool *ok)
{
    int begin = input.indexOf(':');
    if(begin < 0)
    {
        *ok = false;
        return input.toUtf8();
    }

    int end = input.indexOf('\r');
    if(end < 0)
    {
        *ok = false;
        return input.toUtf8();
    }

    if(end < begin)
    {
        *ok = false;
        return input.toUtf8();
    }

    QByteArray temp_ba = QByteArray::fromHex(input.toLocal8Bit());

    *ok = true;
    return temp_ba;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V786_test::convert_data_to_ascii(QByteArray data)
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
void SerialPort_V786_test::receive(const QVariant &data)
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
unsigned char SerialPort_V786_test::calc_limit(void)
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
