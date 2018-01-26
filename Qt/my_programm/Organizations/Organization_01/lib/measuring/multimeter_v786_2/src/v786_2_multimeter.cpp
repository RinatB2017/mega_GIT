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
#include <QApplication>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QByteArray>
#include <QSpinBox>
#include <QDialog>
#include <QStyle>
#include <QLabel>
#include <QTime>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
#include "v786_2_union.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
Multimeter_V786_2::Multimeter_V786_2(QObject *parent) :
    QObject(parent),
    is_silence(false)
{
    init();
}
//--------------------------------------------------------------------------------
Multimeter_V786_2::~Multimeter_V786_2()
{
#ifdef QT_DEBUG
    qDebug() << "~Multimeter_V786_2()";
#endif
    if(serial.isOpen())
    {
        serial.close();
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::init(void)
{
    last_error = E_NO_ERROR;

    multimeter_address = 0;
    multimeter_serno = 0;
    multimeter_year = 0;
    multimeter_channel = 0;

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

    //union U_SHORT temp_crc;
    //temp_crc.value = 0;
    //temp_crc.bytes.lo = 1;
    //qDebug() << temp_crc.value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::port_read(void)
{
    data_radiomodule.append(serial.readAll());
    is_ready = true;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
        //default: emit error(QString("Multimeter_V786_2::port_error Unknown error %1").arg(serial_error)); break;
    }
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::find_device(QString *port_name)
{
    int speed = 57600;
    if(serial.isOpen())
    {
        serial.close();
    }
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("Порт %1")).arg(port.portName()));
        serial.setPort(port);
        bool ok = serial.setBaudRate(speed);
        if(ok)
        {
            if(serial.open(QIODevice::ReadWrite))
            {
                bool ok = check_breloque();
                if(ok)
                {
                    emit info(QString(tr("USB приемо-передатчик В7-86/2 найден на порту %1"))
                              .arg(port.portName()));
                    *port_name = port.portName();
                    return true;
                }
                serial.close();
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::find_wired_device(QString *port_name)
{
    int speed = 57600;
    if(serial.isOpen())
    {
        serial.close();
    }
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("Порт %1")).arg(port.portName()));
        serial.setPort(port);
        bool ok = serial.setBaudRate(speed);
        if(ok)
        {
            if(serial.open(QIODevice::ReadWrite))
            {
                bool ok = check_wired_multimeter();
                if(ok)
                {
                    emit info(QString(tr("В7-86/2 найден на порту %1"))
                              .arg(port.portName()));
                    *port_name = port.portName();
                    return true;
                }
                serial.close();
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::check_wired_multimeter(void)
{
    if(serial.isOpen() == false)
    {
        emit error(tr("Порт мультиметра не открыт!"));
        return false;
    }

    emit info(V764_2_CMD_0x1C_TEXT);

    V764_2_1C_QUESTION get_ID;

    get_ID.body.header.address = multimeter_address;
    get_ID.body.channel = multimeter_channel;
    get_ID.body.header.cmd = V764_2_CMD_0x1C;
    get_ID.body.header.count_data = 0;
    get_ID.body.crc16 = CRC::multimeter_crc16((unsigned char *)&get_ID.buf, sizeof(V764_2_1C_QUESTION) - SIZE_DATA);

    QByteArray data;
    data.clear();
    char buff[sizeof(V764_2_1C_QUESTION)];
    memcpy(buff, (unsigned char *)&get_ID, sizeof(V764_2_1C_QUESTION));
    for(unsigned int n=0; n<sizeof(V764_2_1C_QUESTION); n++)
        data.append(buff[n]);

    emit debug(data.toHex().data());
    data_radiomodule.clear();
    is_ready = false;
#ifdef CONVERT_DATA_TO_ASCII
    serial.write(convert_data_to_ascii(data));
#else
    serial.write(data);
    emit debug(QString("%1").arg(data.data()));
#endif

    QTime time;
    time.start();
#ifndef FAST_COMMAND
    wait(1000);
#else
    wait(sizeof(V764_2_1C_ANSWER), 100, 1000);
#endif
    emit info(QString(tr("Прошло времени %1 ms")).arg(time.elapsed()));

    if(data_radiomodule.isEmpty())
    {
        last_error = E_ERROR_NO_ANSWER;
        print_last_error();
        return false;
    }

    int err = check_packet_answer(V764_2_CMD_0x1C, sizeof(V764_2_1C_ANSWER));
    if(err != E_NO_ERROR)
    {
        last_error = err;
        print_last_error();
        return false;
    }

    last_error = E_NO_ERROR;

    V764_2_1C_ANSWER *get_id = (V764_2_1C_ANSWER *)data_radiomodule.constData();
    if(get_id->body.header.address != multimeter_address) return false;
    if(get_id->body.header.cmd != V764_2_CMD_0x1C) return false;
    if(get_id->body.header.count_data != 3) return false;
    if(get_id->body.data.byte_C1 != 0xC1) return false;
    if(get_id->body.data.byte_C2 != 0xC2) return false;
    if(get_id->body.data.byte_C3 != 0xC3) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::check_breloque(void)
{
    if(serial.isOpen() == false)
    {
        emit error(tr("Порт USB приемо-передатчика не открыт!"));
        return false;
    }

    emit info(tr("Тест USB приемо-передатчика"));

    QByteArray temp;
    temp.clear();

    temp.append(":");
    temp.append("FF55AA55FF");
    temp.append((char)0x0D);

    data_radiomodule.clear();
    is_ready = false;
    serial.write(temp);

    QTime time;
    time.start();
#ifndef FAST_COMMAND
    wait(5000);
#else
    wait(sizeof(breloque_answer), 100, 5000);
#endif
    emit info(QString(tr("Прошло времени %1 ms")).arg(time.elapsed()));

    if(data_radiomodule.isEmpty())
    {
        emit error(tr("USB приемо-передатчик не отвечает!"));
        return false;
    }

    if(data_radiomodule.data() != breloque_answer)
    {
        emit error(tr("Это не USB приемо-передатчик!"));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::check_packet_answer(unsigned char cmd,
                                           int packet_len)
{
    if(data_radiomodule.isEmpty())
    {
        last_error = E_ERROR_NO_ANSWER;
        return last_error;
    }

    if(data_radiomodule.length() < 2)
    {
        emit debug(data_radiomodule.toHex());
        last_error = E_ERROR_SMALL_ANSWER;
        return last_error;
    }
    if(data_radiomodule.at(1) != cmd)
    {
        emit error(data_radiomodule.toHex());
        last_error = E_ERROR_BAD_CMD;
        return last_error;
    }

    if(data_radiomodule.length() > packet_len)
    {
        emit error(data_radiomodule.toHex());
        emit error(QString(tr("check_packet_answer: Длина пакета (пришло) %1 > (ожидалось) %2"))
                   .arg(data_radiomodule.length())
                   .arg(packet_len));
        last_error = E_ERROR_BIG_ANSWER;
        return last_error;
    }
    if(data_radiomodule.length() < packet_len)
    {
        emit error(data_radiomodule.toHex());
        emit error(QString(tr("check_packet_answer: Длина пакета (пришло) %1 < (ожидалось) %2"))
                   .arg(data_radiomodule.length())
                   .arg(packet_len));
        last_error = E_ERROR_SMALL_ANSWER;
        return last_error;
    }

    B7_86_HEADER *header = (B7_86_HEADER *)data_radiomodule.constData();
    int len = data_radiomodule.size() - sizeof(B7_86_HEADER) - 2;
    if(header->count_data != len)
    {
        emit error(data_radiomodule.toHex());
        emit error(QString(tr("check_packet_answer: size (пришло) %1 < (ожидалось) %2"))
                   .arg(header->count_data)
                   .arg(len));
        last_error = E_ERROR_SIZE_NOT_CORRECT;
        return last_error;
    }

    unsigned short calc_crc16 = CRC::multimeter_crc16((unsigned char *)data_radiomodule.constData(),
                                                      packet_len - 2);

    union U_SHORT temp_crc;
    temp_crc.bytes.lo = data_radiomodule.at(packet_len - 2);
    temp_crc.bytes.hi = data_radiomodule.at(packet_len - 1);
#ifdef USE_HTONS
    unsigned short data_crc16 = htons(temp_crc.value);
#else
    unsigned short data_crc16 = temp_crc.value;
#endif

    emit debug(data_radiomodule.toHex().data());
    if(data_crc16 != calc_crc16)
    {
        emit error(data_radiomodule.toHex());
        emit error(tr("Ошибка контрольной суммы data_crc16 [%1] calc_crc16 [%2]")
                   .arg(data_crc16, 0, 16)
                   .arg(calc_crc16, 0, 16));
        last_error = E_ERROR_BAD_CRC;
        return last_error;
    }
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::get_deviation(bool *ok)
{
    int err = check_packet_answer(V764_2_CMD_0x06, sizeof(V764_2_06_ANSWER));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return last_error;
    }

    V764_2_06_ANSWER *b786_measuring = (V764_2_06_ANSWER *)data_radiomodule.constData();
    BYTE_DEVIATION_RETENTION byte_options;
    byte_options.value = b786_measuring->body.data.deviation_retention;
    int deviation = byte_options.data.deviation;

    *ok = true;
    return deviation;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_deviation_retention(bool *ok)
{
    int err = check_packet_answer(V764_2_CMD_0x06, sizeof(V764_2_06_ANSWER));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return last_error;
    }

    V764_2_06_ANSWER *b786_measuring = (V764_2_06_ANSWER *)data_radiomodule.constData();

    *ok = true;
    return b786_measuring->body.data.deviation_retention;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_limit(unsigned char cmd, bool *ok)
{
    int err = check_packet_answer(cmd, sizeof(V764_2_MEASURING_ANSWER));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return last_error;
    }

    V764_2_MEASURING_ANSWER *b786_measuring = (V764_2_MEASURING_ANSWER *)data_radiomodule.constData();
    BYTE_LIMITS byte_limits;
    //qDebug() << "b786_measuring->limits" << b786_measuring->limits;
    byte_limits.value = b786_measuring->body.data.limits;
    unsigned char limit = byte_limits.data.limit;

    *ok = true;
    return limit;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_value(int cmd, double *value)
{
    int err = check_packet_answer(cmd, sizeof(V764_2_MEASURING_ANSWER));
    if(err != E_NO_ERROR)
    {
        return false;
    }

    bool temp_ok = false;
    V764_2_MEASURING_ANSWER *b786_measuring = (V764_2_MEASURING_ANSWER *)data_radiomodule.constData();

    QString temp;
    temp.clear();
    for(unsigned int n=0; n<sizeof(b786_measuring->body.DisplaySymbols); n++)
        temp.append(b786_measuring->body.DisplaySymbols[n]);
    float x = temp.toFloat(&temp_ok);
    if(temp_ok == false)
    {
        emit error(QString(tr("Что это? [%1]")).arg(temp));
        return false;
    }

    *value = x;
    return true;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_string_value(bool *ok)
{
    int err = check_packet_answer(V764_2_CMD_0x06, sizeof(V764_2_06_ANSWER));
    if(err != E_NO_ERROR)
    {
        *ok = false;
        last_error = err;
        return "";
    }

    V764_2_06_ANSWER *b786_measuring = (V764_2_06_ANSWER *)data_radiomodule.constData();

    QString temp;
    temp.clear();
    for(unsigned int n=0; n<sizeof(b786_measuring->body.data.DisplaySymbols); n++)
        temp.append(b786_measuring->body.data.DisplaySymbols[n]);

    *ok = true;
    return temp;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::service_on(int timeout)
{
    if(serial.isOpen() == false)
    {
        last_error = E_ERROR_PORT_NOT_OPEN;
        return false;
    }

    V764_2_04_QUESTION service;

    service.body.header.address = multimeter_address;
    service.body.channel = multimeter_channel;
    service.body.header.cmd = V764_2_CMD_0x04;
    service.body.header.count_data = 6;
    service.body.data.password[0] = '7';
    service.body.data.password[1] = 'e';
    service.body.data.password[2] = '2';
    service.body.data.password[3] = 'c';
    service.body.data.password[4] = 'd';
    service.body.data.password[5] = '5';
    service.body.crc16 = CRC::multimeter_crc16((unsigned char *)&service.buf, sizeof(V764_2_04_QUESTION) - SIZE_DATA);

#if 0
    qDebug() << service.data.header.address;
    qDebug() << service.data.header.cmd;
    qDebug() << sizeof(B7_86_SERVICE);
    qDebug() << "crc16" << QString("0x%1").arg(service.data.crc16, 0, 16);
#endif

    QByteArray data;
    data.clear();
    char buff[sizeof(V764_2_04_QUESTION)];
    memcpy(buff, (unsigned char *)&service, sizeof(V764_2_04_QUESTION));
    for(unsigned int n=0; n<sizeof(V764_2_04_QUESTION); n++)
        data.append(buff[n]);

    //emit info(data.toHex().data());
    data_radiomodule.clear();
    is_ready = false;
#ifdef CONVERT_DATA_TO_ASCII
    serial.write(convert_data_to_ascii(data));
    emit debug(QString("%1").arg(convert_data_to_ascii(data).data()));
#else
    serial.write(data);
    emit debug(QString("%1").arg(data.data()));
#endif

    QTime time;
    time.start();
#ifndef FAST_COMMAND
    wait(timeout);
#else
    wait(sizeof(V764_2_04_ANSWER), 100, timeout);
#endif
    emit info(QString(tr("Прошло времени %1 ms")).arg(time.elapsed()));

    if(data_radiomodule.isEmpty())
    {
        last_error = E_ERROR_NO_ANSWER;
        return false;
    }

    int err = check_packet_answer(V764_2_CMD_0x04, sizeof(V764_2_04_ANSWER));
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    // анализируем данные
    V764_2_04_ANSWER *output_packet = (V764_2_04_ANSWER *)data.data();
    V764_2_04_ANSWER *input_packet  = (V764_2_04_ANSWER *)data_radiomodule.constData();

    unsigned char cmd = input_packet->body.header.cmd;

    if(output_packet->body.header.address != input_packet->body.header.address)
    {
        emit debug(QString("receive %1").arg(data_radiomodule.toHex().data()));
        emit error(QString(tr("Неверный адрес (пришло) %1 != (должно было прийти) %2"))
                   .arg(input_packet->body.header.address)
                   .arg(output_packet->body.header.address));
        last_error = E_ERROR_BAD_ADDRESS;
        return false;
    }

    if((input_packet->body.data.serial_year.SerialNo != multimeter_serno) ||
            (input_packet->body.data.serial_year.SerialYear != multimeter_year))
    {
        // emit error(data_radiomodule.toHex());
        emit error(QString("len %1").arg(data_radiomodule.count()));
        emit error(tr("Пришло:"));
        emit error(QString(tr("   серийный номер %1")).arg(input_packet->body.data.serial_year.SerialNo));
        emit error(QString(tr("   год изготовления %1")).arg(input_packet->body.data.serial_year.SerialYear));
        emit error(tr("Ожидалось:"));
        emit error(QString(tr("   серийный номер %1")).arg(multimeter_serno));
        emit error(QString(tr("   год изготовления %1")).arg(multimeter_year));
        last_error = E_ERROR_BAD_SERNO;
        return false;
    }

    union U_BYTE acmd;
    acmd.value = cmd;
    if(acmd.bites.bit7 == 1)
    {
        // ошибка //???
        //B7_86_ERROR_ANSWER *b786_error = (B7_86_ERROR_ANSWER *)data_radiomodule.constData();
        //last_error = b786_error->data.err;
        return last_error;
    }

    if(cmd == V764_2_CMD_0x01)
    {
        V764_2_01_ANSWER *b786_state;
        b786_state = (V764_2_01_ANSWER *)data_radiomodule.constData();

        emit info(tr("Статус прибора:"));
        emit info(QString(tr("    адрес %1")).arg(b786_state->body.header.address));
        emit info(QString(tr("    серийный номер %1")).arg(b786_state->body.data.serial_year.SerialNo));
        emit info(QString(tr("    год изготовления %1")).arg(b786_state->body.data.serial_year.SerialYear));
        QString temp;
        temp.clear();
        for(unsigned int n=0; n<sizeof(b786_state->body.data.DisplaySymbols); n++)
            temp.append(b786_state->body.data.DisplaySymbols[n]);
        emit info(QString(tr("    отображаемое значение %1")).arg(temp));

        emit info(QString(tr("    предел %1")).arg(b786_state->body.data.Range));
        emit info(QString(tr("    режим %1")).arg(b786_state->body.data.Mode));

        U_BYTE_DECADES temp_decades;
        temp_decades.value = b786_state->body.data.Math_Hold;
        emit info(QString(tr("    math %1")).arg(temp_decades.decades.lo));
        emit info(QString(tr("    hold %1")).arg(temp_decades.decades.hi));

        emit info(QString(tr("    напряжение батареи %1")).arg(b786_state->body.data.BattVoltage));
    }

    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::convert_data_to_ascii(QByteArray data)
{
    QByteArray temp;

    temp.clear();
    temp.append(':');
    temp.append(data.toHex());
    temp.append((char)0x0D);

    emit debug(QString("convert_data_to_ascii %1").arg(temp.data()));
    return temp;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::send_raw_data(const QByteArray &data)
{
    if(serial.isOpen() == false)
    {
        last_error = E_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    serial.write(data);
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::send(int command,
                            unsigned char *data,
                            unsigned int len_data,
                            unsigned int serialno,
                            unsigned int year,
                            int timeout)
{
    if(serial.isOpen() == false)
    {
        last_error = E_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    if(len_data < sizeof(B7_86_HEADER))
    {
        emit error(QString(tr("Длина отправляемого пакета слишком мала %1 < %2"))
                   .arg(len_data)
                   .arg(sizeof(B7_86_HEADER)));
        last_error = E_ERROR_SMALL_ANSWER;
        return last_error;
    }

    emit debug("send");

    QByteArray temp;
    temp.clear();
    temp.append((const char *)data, len_data);
#ifdef QT_DEBUG
    qDebug() << "send" << temp.toHex();
#endif

    data_radiomodule.clear();
    is_ready = false;

#ifdef CONVERT_DATA_TO_ASCII
    serial.write(convert_data_to_ascii(temp));
#else
    serial.write(temp);
#endif

    QTime time;
    time.start();
#ifndef FAST_COMMAND
    wait(timeout);
#else
    wait(get_size_answer(command), 100, timeout);
#endif
    if(!is_silence)
    {
        emit info(QString(tr("Прошло времени %1 ms")).arg(time.elapsed()));
    }

    if(data_radiomodule.isEmpty())
    {
        last_error = E_ERROR_NO_ANSWER;
        return last_error;
    }

#ifdef QT_DEBUG
    qDebug() << "get" << data_radiomodule.toHex();
#endif
    switch(command)
    {
    case V764_2_CMD_0x01: check_packet_answer(command, sizeof(V764_2_01_ANSWER)); break;
    case V764_2_CMD_0x02: check_packet_answer(command, sizeof(V764_2_02_ANSWER)); break;
    case V764_2_CMD_0x03: check_packet_answer(command, sizeof(V764_2_03_ANSWER)); break;
    case V764_2_CMD_0x04: check_packet_answer(command, sizeof(V764_2_04_ANSWER)); break;
    case V764_2_CMD_0x05: check_packet_answer(command, sizeof(V764_2_05_ANSWER)); break;
    case V764_2_CMD_0x06: check_packet_answer(command, sizeof(V764_2_06_ANSWER)); break;
    case V764_2_CMD_0x07: check_packet_answer(command, sizeof(V764_2_07_ANSWER)); break;
    case V764_2_CMD_0x08: check_packet_answer(command, sizeof(V764_2_08_ANSWER)); break;
    case V764_2_CMD_0x09: check_packet_answer(command, sizeof(V764_2_09_ANSWER)); break;
    case V764_2_CMD_0x0A: check_packet_answer(command, sizeof(V764_2_0A_ANSWER)); break;
    case V764_2_CMD_0x0B: check_packet_answer(command, sizeof(V764_2_0B_ANSWER)); break;
    case V764_2_CMD_0x0C: check_packet_answer(command, sizeof(V764_2_0C_ANSWER)); break;
    case V764_2_CMD_0x0D: check_packet_answer(command, sizeof(V764_2_0D_ANSWER)); break;
    case V764_2_CMD_0x0E: check_packet_answer(command, sizeof(V764_2_0E_ANSWER)); break;
    case V764_2_CMD_0x0F: check_packet_answer(command, sizeof(V764_2_0F_ANSWER)); break;
    case V764_2_CMD_0x10: check_packet_answer(command, sizeof(V764_2_10_ANSWER)); break;
    case V764_2_CMD_0x11: check_packet_answer(command, sizeof(V764_2_11_ANSWER)); break;
    case V764_2_CMD_0x12: check_packet_answer(command, sizeof(V764_2_12_ANSWER)); break;
    case V764_2_CMD_0x13: check_packet_answer(command, sizeof(V764_2_13_ANSWER)); break;
    case V764_2_CMD_0x14: check_packet_answer(command, sizeof(V764_2_14_ANSWER)); break;
    case V764_2_CMD_0x15: check_packet_answer(command, sizeof(V764_2_15_ANSWER)); break;
    case V764_2_CMD_0x16: check_packet_answer(command, sizeof(V764_2_16_ANSWER)); break;
    case V764_2_CMD_0x17: check_packet_answer(command, sizeof(V764_2_17_ANSWER)); break;
    case V764_2_CMD_0x18: check_packet_answer(command, sizeof(V764_2_18_ANSWER)); break;
    case V764_2_CMD_0x19: check_packet_answer(command, sizeof(V764_2_19_ANSWER)); break;
    case V764_2_CMD_0x1A: check_packet_answer(command, sizeof(V764_2_1A_ANSWER)); break;
    case V764_2_CMD_0x1B: check_packet_answer(command, sizeof(V764_2_1B_ANSWER)); break;
    case V764_2_CMD_0x1C: check_packet_answer(command, sizeof(V764_2_1C_ANSWER)); break;
    case V764_2_CMD_0x1D: check_packet_answer(command, sizeof(V764_2_1D_ANSWER)); break;

    default:
        emit error(QString("unknown cmd (2) 0x%1").arg(command, 0, 16));
        last_error = E_ERROR_UNKNOWN_CMD;
        return last_error;
    }
    if(last_error == E_ERROR_BAD_CMD)
    {
        V764_2_ERROR_ANSWER *error_answer = (V764_2_ERROR_ANSWER *)data_radiomodule.constData();
        last_error = error_answer->body.error;
        return print_last_error();
    }
    if(last_error != E_NO_ERROR)
    {
        return last_error;
    }

    // анализируем данные
    V764_2_HEADER_ANSWER *output_packet = (V764_2_HEADER_ANSWER *)data;
    V764_2_HEADER_ANSWER *input_packet  = (V764_2_HEADER_ANSWER *)data_radiomodule.constData();

    emit debug(QString("multimeter send %1").arg(data_radiomodule.toHex().data()));
    if(output_packet->body.header.address != input_packet->body.header.address)
    {
        emit debug(QString("receive %1").arg(data_radiomodule.toHex().data()));
        emit error(QString(tr("Неверный адрес (пришло) %1 != (должно было прийти) %2"))
                   .arg(input_packet->body.header.address)
                   .arg(output_packet->body.header.address));
        last_error = E_ERROR_BAD_ADDRESS;
        return last_error;
    }

    if((input_packet->body.serial_year.SerialNo != serialno) || (input_packet->body.serial_year.SerialYear != year))
    {
        // emit error(data_radiomodule.toHex());
        emit error(QString("len %1").arg(data_radiomodule.count()));
        emit error(tr("Пришло:"));
        emit error(QString(tr("   серийный номер %1")).arg(input_packet->body.serial_year.SerialNo));
        emit error(QString(tr("   год изготовления %1")).arg(input_packet->body.serial_year.SerialYear));
        emit error(tr("Ожидалось:"));
        emit error(QString(tr("   серийный номер %1")).arg(serialno));
        emit error(QString(tr("   год изготовления %1")).arg(year));
        last_error = E_ERROR_BAD_SERNO;
        return last_error;
    }

    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::get_size_answer(int command)
{
    int res = sizeof(V764_2_01_ANSWER);
    switch(command)
    {
    case V764_2_CMD_0x01: res = sizeof(V764_2_01_ANSWER); break;
    case V764_2_CMD_0x02: res = sizeof(V764_2_02_ANSWER); break;
    case V764_2_CMD_0x03: res = sizeof(V764_2_03_ANSWER); break;
    case V764_2_CMD_0x04: res = sizeof(V764_2_04_ANSWER); break;
    case V764_2_CMD_0x05: res = sizeof(V764_2_05_ANSWER); break;
    case V764_2_CMD_0x06: res = sizeof(V764_2_06_ANSWER); break;
    case V764_2_CMD_0x07: res = sizeof(V764_2_07_ANSWER); break;
    case V764_2_CMD_0x08: res = sizeof(V764_2_08_ANSWER); break;
    case V764_2_CMD_0x09: res = sizeof(V764_2_09_ANSWER); break;
    case V764_2_CMD_0x0A: res = sizeof(V764_2_0A_ANSWER); break;
    case V764_2_CMD_0x0B: res = sizeof(V764_2_0B_ANSWER); break;
    case V764_2_CMD_0x0C: res = sizeof(V764_2_0C_ANSWER); break;
    case V764_2_CMD_0x0D: res = sizeof(V764_2_0D_ANSWER); break;
    case V764_2_CMD_0x0E: res = sizeof(V764_2_0E_ANSWER); break;
    case V764_2_CMD_0x0F: res = sizeof(V764_2_0F_ANSWER); break;
    case V764_2_CMD_0x10: res = sizeof(V764_2_10_ANSWER); break;
    case V764_2_CMD_0x11: res = sizeof(V764_2_11_ANSWER); break;
    case V764_2_CMD_0x12: res = sizeof(V764_2_12_ANSWER); break;
    case V764_2_CMD_0x13: res = sizeof(V764_2_13_ANSWER); break;
    case V764_2_CMD_0x14: res = sizeof(V764_2_14_ANSWER); break;
    case V764_2_CMD_0x15: res = sizeof(V764_2_15_ANSWER); break;
    case V764_2_CMD_0x16: res = sizeof(V764_2_16_ANSWER); break;
    case V764_2_CMD_0x17: res = sizeof(V764_2_17_ANSWER); break;
    case V764_2_CMD_0x18: res = sizeof(V764_2_18_ANSWER); break;
    case V764_2_CMD_0x19: res = sizeof(V764_2_19_ANSWER); break;
    case V764_2_CMD_0x1A: res = sizeof(V764_2_1A_ANSWER); break;
    case V764_2_CMD_0x1B: res = sizeof(V764_2_1B_ANSWER); break;
    case V764_2_CMD_0x1C: res = sizeof(V764_2_1C_ANSWER); break;
    case V764_2_CMD_0x1D: res = sizeof(V764_2_1D_ANSWER); break;

    default:
        emit error(QString("unknown COMMAND %1").arg(command, 0, 16));
        break;
    }
    return res;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_limit_string(unsigned int cmd,
                                            unsigned char byte_options,
                                            bool is_russian)
{
    QString temp;
    BYTE_LIMITS byte_limits;
    byte_limits.value = byte_options;

    //qDebug() << "byte_limits.data.reserved" << byte_limits.data.reserved;
    //qDebug() << "byte_limits.data.auto_limit" << byte_limits.data.auto_limit;
    //qDebug() << "byte_limits.data.limit" << byte_limits.data.limit;
    temp.clear();
    switch(cmd)
    {
    case V764_2_CMD_0x06:
    case V764_2_CMD_0x07:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("мВ")) : temp.append("mV");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append(tr("В")) : temp.append("V");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }
        break;

    case V764_2_CMD_0x08:
    case V764_2_CMD_0x09:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("мкА")) : temp.append("mkA");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append(tr("мА")) : temp.append("mA");
            break;

        case 5:
            is_russian ? temp.append(tr("А")) : temp.append("A");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }
        break;

    case V764_2_CMD_0x0D:
    case V764_2_CMD_0x0A:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("Ом")) : temp.append("Om");
            break;

        case 1:
        case 2:
        case 3:
            is_russian ? temp.append(tr("кОм")) : temp.append("kOm");
            break;

        case 4:
            is_russian ? temp.append(tr("МОм")) : temp.append("MOm");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }
        break;

    case V764_2_CMD_0x0B:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("пФ")) : temp.append("pF");
            break;

        case 1:
            is_russian ? temp.append(tr("мкФ")) : temp.append("mkF");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }

        break;

    case V764_2_CMD_0x0C:
        is_russian ? temp.append(tr("мГ")) : temp.append("mH");
        break;

    case V764_2_CMD_0x0E:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("Гц")) : temp.append("Hz");
            break;

        case 1:
            is_russian ? temp.append(tr("кГц")) : temp.append("kHz");
            break;

        case 2:
            is_russian ? temp.append(tr("МГц")) : temp.append("MHz");
            break;

        default:
            break;
        }
        break;

    case V764_2_CMD_0x0F:
        is_russian ? temp.append(tr("кВА")) : temp.append("kWA");
        break;

    case V764_2_CMD_0x10:
        is_russian ? temp.append(tr("кВАр")) : temp.append("kWAr");
        break;

    case V764_2_CMD_0x11:
        is_russian ? temp.append(tr("кВт")) : temp.append("kW");
        break;

    case V764_2_CMD_0x12:
        break;

    default:
        break;
    }

    //qDebug() << temp;
    return temp;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_limit_string(bool is_russian)
{
    QString temp;

    V764_2_06_ANSWER *b786_measuring;
    b786_measuring = (V764_2_06_ANSWER *)data_radiomodule.constData();
    unsigned int cmd = b786_measuring->body.header.cmd;
    BYTE_LIMITS byte_limits;
    byte_limits.value = b786_measuring->body.data.limits;

    temp.clear();
    switch(cmd)
    {
    case V764_2_CMD_0x06:
    case V764_2_CMD_0x07:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("мВ")) : temp.append("mV");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append(tr("В")) : temp.append("V");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }
        break;

    case V764_2_CMD_0x08:
    case V764_2_CMD_0x09:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("мкА")) : temp.append("mkA");
            break;

        case 1:
        case 2:
        case 3:
        case 4:
            is_russian ? temp.append(tr("мА")) : temp.append("mA");
            break;

        case 5:
            is_russian ? temp.append(tr("А")) : temp.append("A");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }
        break;

    case V764_2_CMD_0x0D:
    case V764_2_CMD_0x0A:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("Ом")) : temp.append("Om");
            break;

        case 1:
        case 2:
        case 3:
            is_russian ? temp.append(tr("кОм")) : temp.append("kOm");
            break;

        case 4:
            is_russian ? temp.append(tr("МОм")) : temp.append("MOm");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }
        break;

    case V764_2_CMD_0x0B:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("пФ")) : temp.append("pF");
            break;

        case 1:
            is_russian ? temp.append(tr("мкФ")) : temp.append("mkF");
            break;

        default:
            //temp.append(QString(tr("%1")).arg(limit));
            temp.append(tr("---"));
            break;
        }

        break;

    case V764_2_CMD_0x0C:
        is_russian ? temp.append(tr("мГ")) : temp.append("mH");
        break;

    case V764_2_CMD_0x0E:
        switch(byte_limits.data.limit)
        {
        case 0:
            is_russian ? temp.append(tr("Гц")) : temp.append("Hz");
            break;

        case 1:
            is_russian ? temp.append(tr("кГц")) : temp.append("kHz");
            break;

        case 2:
            is_russian ? temp.append(tr("МГц")) : temp.append("MHz");
            break;

        default:
            break;
        }
        break;

    case V764_2_CMD_0x0F:
        is_russian ? temp.append(tr("кВА")) : temp.append("kWA");
        break;

    case V764_2_CMD_0x10:
        is_russian ? temp.append(tr("кВАр")) : temp.append("kWAr");
        break;

    case V764_2_CMD_0x11:
        is_russian ? temp.append(tr("кВт")) : temp.append("kW");
        break;

    case V764_2_CMD_0x12:
        break;

    default:
        break;
    }

    //qDebug() << temp;
    return temp;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_deviation_string(void)
{
    QString temp;
    temp.clear();

    if(data_radiomodule.length() == sizeof(V764_2_06_ANSWER))
    {
        V764_2_06_ANSWER *b786_measuring;
        b786_measuring = (V764_2_06_ANSWER *)data_radiomodule.constData();

        BYTE_DEVIATION_RETENTION byte_deviation_retention;
        byte_deviation_retention.value = b786_measuring->body.data.deviation_retention;
        //qDebug() << "byte_deviation_retention.data.deviation" << byte_deviation_retention.data.data.deviation;
        switch(byte_deviation_retention.data.deviation)
        {
        case 1:
            temp.append("Δ");
            break;

        case 2:
            temp.append("%");
            break;

        default:
            break;
        }
    }
    //qDebug() << "deviation temp" << temp;
    return temp;
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_retention_string(void)
{
    QString temp;
    temp.clear();

    if(data_radiomodule.length() == sizeof(V764_2_06_ANSWER))
    {
        V764_2_06_ANSWER *b786_measuring;
        b786_measuring = (V764_2_06_ANSWER *)data_radiomodule.constData();

        BYTE_DEVIATION_RETENTION byte_deviation_retention;
        byte_deviation_retention.value = b786_measuring->body.data.deviation_retention;
        //qDebug() << "byte_deviation_retention.data.retention" << byte_deviation_retention.data.retention;
        switch(byte_deviation_retention.data.retention)
        {
        case 3:
            temp.append("max");
            break;

        case 2:
            temp.append("min");
            break;

        case 1:
            temp.append("hold");
            break;

        case 0:
            break;

        default:
            temp.append(QString("%1").arg(byte_deviation_retention.data.retention));
            break;
        }
    }
    return temp;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::convert_ascii_to_data(QString input, bool *ok)
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
#ifdef CALIBRATION_WIRED
void Multimeter_V786_2::wait(int timeout_msec)
{
#ifdef FAKE
    Q_UNUSED(timeout_msec)
#else
    QTime time;

    if(timeout_msec < 1)
        return;

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        bool ok = false;
        QByteArray temp = convert_ascii_to_data(data_radiomodule.data(), &ok);
        if(ok)
        {
            emit debug(QString("wait_msec: data [%1]").arg(temp.toHex().data()));
            data_radiomodule.clear();
            data_radiomodule.append(temp);
            return;
        }
    }
#endif
}
#else
void Multimeter_V786_2::wait(int timeout_msec)
{
#ifdef FAKE
    Q_UNUSED(timeout_msec)
#else
    QTime time;

    if(timeout_msec < 1)
        return;

    bool need_time = false;
    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        if(is_ready == true)
        {
            if(need_time == false)
            {
                timeout_msec = time.elapsed() + 500;
                need_time = true;
            }
        }
    }
    //qDebug() << "data_radiomodule" << data_radiomodule.length();
#endif
}
#endif
//--------------------------------------------------------------------------------
void Multimeter_V786_2::wait(int max_len,
                             int min_time_msec,
                             int max_time_msec)
{
#ifdef FAKE
    Q_UNUSED(max_len)
    Q_UNUSED(min_time_msec)
    Q_UNUSED(max_time_msec)
#else
    QTime time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > min_time_msec)
            break;
    }

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > max_time_msec)
            break;

        if(data_radiomodule.size() >= max_len)
            break;
    }
#endif
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_raw_data(void)
{
    return data_radiomodule;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::port_is_open(void)
{
    return serial.isOpen();
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::measuring(unsigned char cmd,
                                 double *value,
                                 bool *ok)
{
    BYTE_MEASURING byte_measuring;

    byte_measuring.data.retention = multimeter_retention;
    byte_measuring.data.deviation = multimeter_deviation;

    byte_measuring.data.auto_limit = multimeter_auto_limit;
    if(multimeter_auto_limit)
        byte_measuring.data.limit = 0;
    else
        byte_measuring.data.limit = multimeter_limit;

    unsigned char data_len = 1;
    unsigned char data_0 = byte_measuring.value;

    set_serno(multimeter_serno);
    set_year(multimeter_year);
    set_address(multimeter_address);
    set_channel(multimeter_channel);

    V764_2_MEASURING_QUESTION question;
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
    question.body.header.cmd = cmd;
    question.body.header.count_data = data_len;
    question.body.data_0 = data_0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_MEASURING_QUESTION) - SIZE_DATA);

    int err = send(cmd,
                   question.buf,
                   sizeof(V764_2_MEASURING_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   multimeter_timeout);
    if(err == ERROR_13)
    {
        emit change_retention("");
        emit change_deviation("");
        emit get_measuring_value("OL");
        emit change_limit("---");
        *ok = false;
        last_error = err;
        return last_error;
    }
    if(err == ERROR_14)
    {
        emit change_retention("");
        emit change_deviation("");
        emit get_measuring_value("- OL");
        emit change_limit("---");
        *ok = false;
        last_error = err;
        return last_error;
    }
    if(err == ERROR_15)
    {
        emit change_retention("");
        emit change_deviation("");
        emit get_measuring_value("LF");
        emit change_limit("---");
        *ok = false;
        last_error = err;
        return last_error;
    }
    if(err == E_NO_ERROR)
    {
        emit change_retention(get_retention_string());
        emit change_deviation(get_deviation_string());
        emit change_limit(get_limit_string(cmd, get_limit()));

        bool temp_ok = false;
        double measuring_value = 0;
        temp_ok = get_value(cmd, &measuring_value);
        if(temp_ok)
        {
            emit get_measuring_value(QString("%1").arg(measuring_value));
            *ok = true;
            *value = measuring_value;
            last_error = E_NO_ERROR;
            return last_error;
        }
        else
        {
            *ok = false;
            last_error = E_UNKNOWN_ERROR;
            return last_error;
        }
    }
    else
    {
        *ok = false;
        last_error = err;
        return last_error;
    }
    *ok = true;
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::measuring_battery(bool *ok)
{
    emit info(V764_2_CMD_0x01_TEXT);
    set_serno(multimeter_serno);
    set_year(multimeter_year);
    set_address(multimeter_address);
    set_channel(multimeter_channel);

    V764_2_01_QUESTION question;
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
    question.body.header.cmd = V764_2_CMD_0x01;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_01_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x01,
                   question.buf,
                   sizeof(V764_2_01_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   multimeter_timeout);
    if(err == E_NO_ERROR)
    {
        QByteArray ba;
        ba.clear();
        ba.append(get_raw_data());
        if(ba.length() == sizeof(V764_2_01_ANSWER))
        {
            V764_2_01_ANSWER *b786_state;
            b786_state = (V764_2_01_ANSWER *)ba.data();
            emit get_battery_value(b786_state->body.data.BattVoltage);
            *ok = true;
        }
    }
    else
    {
        *ok = false;
        last_error = err;
        return last_error;
    }
    *ok = true;
    last_error = E_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_retention(char value)
{
    if(value < 0) value = 0;
    emit debug(QString("set retention %1").arg((int)value));
    multimeter_retention = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_deviation(char value)
{
    if(value < 0) value = 0;
    emit debug(QString("set deviation %1").arg((int)value));
    multimeter_deviation = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_limit(char value)
{
    if(value < 0) value = 0;
    emit debug(QString("set limit %1").arg((int)value));
    multimeter_limit = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_auto_limit(bool state)
{
    emit debug(QString("set auto_limit %1").arg(state));
    multimeter_auto_limit = state;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_timeout(unsigned int value)
{
    emit debug(QString("set timeout %1").arg(value));
    multimeter_timeout = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_address(unsigned int value)
{
    emit debug(QString("set address %1").arg(value));
    multimeter_address = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_channel(unsigned int value)
{
    emit debug(QString("set channel %1").arg(value));
    multimeter_channel = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_serno(unsigned int value)
{
    emit debug(QString("set serno %1").arg(value));
    multimeter_serno = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_year(unsigned int value)
{
    emit debug(QString("set year %1").arg(value));
    multimeter_year = value;
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void Multimeter_V786_2::set_fake_address(uint8_t value)
{
    serial.setAddress(value);
}
#endif
//--------------------------------------------------------------------------------
#ifdef FAKE
void Multimeter_V786_2::set_fake_channel(uint8_t value)
{
    serial.setChannel(value);
}
#endif
//--------------------------------------------------------------------------------
#ifdef FAKE
void Multimeter_V786_2::set_fake_serno(uint16_t value)
{
    serial.setSerNo(value);
}
#endif
//--------------------------------------------------------------------------------
#ifdef FAKE
void Multimeter_V786_2::set_fake_year(uint16_t value)
{
    serial.setYear(value);
}
#endif
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_silence(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_retention(void)
{
    emit debug(QString("get retention %1").arg((int)multimeter_retention));
    return multimeter_retention;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_deviation(void)
{
    emit debug(QString("get deviation %1").arg((int)multimeter_deviation));
    return multimeter_deviation;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_limit(void)
{
    emit debug(QString("get limit %1").arg((int)multimeter_limit));
    return multimeter_limit;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_auto_limit(void)
{
    emit debug(QString("get auto_limit %1").arg((int)multimeter_auto_limit));
    return multimeter_auto_limit;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_timeout(void)
{
    emit debug(QString("get timeout %1").arg((int)multimeter_timeout));
    return multimeter_timeout;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_address(void)
{
    emit debug(QString("get address %1").arg((int)multimeter_address));
    return multimeter_address;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_channel(void)
{
    emit debug(QString("get channel %1").arg((int)multimeter_channel));
    return multimeter_channel;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_serno(void)
{
    emit debug(QString("get serno %1").arg((int)multimeter_serno));
    return multimeter_serno;
}
//--------------------------------------------------------------------------------
unsigned int Multimeter_V786_2::get_year(void)
{
    emit debug(QString("get year %1").arg((int)multimeter_year));
    return multimeter_year;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_step_a(unsigned char value)
{
    multimeter_step_a = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_step_b(unsigned char value)
{
    multimeter_step_b = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_step_c(unsigned char value)
{
    multimeter_step_c = value;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_step_a(void)
{
    return multimeter_step_a;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_step_b(void)
{
    return multimeter_step_b;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_step_c(void)
{
    return multimeter_step_c;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_data_len(unsigned char value)
{
    multimeter_data_len = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_data_0(unsigned char value)
{
    multimeter_data_0 = value;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::set_data_1(unsigned char value)
{
    multimeter_data_1 = value;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_data_len(void)
{
    return multimeter_data_len;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_data_0(void)
{
    return multimeter_data_0;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::get_data_1(void)
{
    return multimeter_data_1;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::print_last_error(void)
{
    switch (last_error)
    {
    case E_NO_ERROR:                break;
    case E_ERROR_MULTIMETR:         emit error(E_ERROR_MULTIMETR_TEXT); break;
    case E_ERROR_INDEX_NOT_CORRECT: emit error(E_ERROR_INDEX_NOT_CORRECT_TEXT); break;
    case E_ERROR_PORT_NOT_OPEN:     emit error(E_ERROR_PORT_NOT_OPEN_TEXT);     break;
    case E_ERROR_BREAK:             emit error(E_ERROR_BREAK_TEXT); break;
    case E_ERROR_MAX_REPEAT:        emit error(E_ERROR_MAX_REPEAT_TEXT); break;
    case E_ERROR_NO_ANSWER:         emit error(E_ERROR_NO_ANSWER_TEXT); break;
    case E_ERROR_BIG_ANSWER:        emit error(E_ERROR_BIG_ANSWER_TEXT); break;
    case E_ERROR_SMALL_ANSWER:      emit error(E_ERROR_SMALL_ANSWER_TEXT); break;
    case E_ERROR_SMALL_PACKET:      emit error(E_ERROR_SMALL_PACKET_TEXT); break;
    case E_ERROR_BAD_SERNO:         emit error(E_ERROR_BAD_SERNO_TEXT); break;
    case E_ERROR_BAD_ADDRESS:       emit error(E_ERROR_BAD_ADDRESS_TEXT); break;
    case E_ERROR_BAD_CRC:           emit error(E_ERROR_BAD_CRC_TEXT); break;
    case E_ERROR_BAD_CMD:           emit error(E_ERROR_BAD_CMD_TEXT); break;
    case E_ERROR_BAD_LEN:           emit error(E_ERROR_BAD_LEN_TEXT); break;
    case E_ERROR_UNKNOWN_CMD:       emit error(E_ERROR_UNKNOWN_CMD_TEXT); break;
    case E_ERROR_SIZE_NOT_CORRECT:  emit error(E_ERROR_SIZE_NOT_CORRECT_TEXT); break;
    case E_UNKNOWN_ERROR:           emit error(E_UNKNOWN_ERROR_TEXT); break;
    case E_INVALID_FLOAT:           emit error(E_INVALID_FLOAT_TEXT); break;
    case E_INVALID_LIMIT:           emit error(E_INVALID_LIMIT_TEXT); break;
    case E_INVALID_DELTA:           emit error(E_INVALID_DELTA_TEXT); break;
    case E_BREAK_MEASURING:         emit error(E_BREAK_MEASURING_TEXT); break;

    case ERROR_01: emit error(ERROR_01_TEXT); break;
    case ERROR_02: emit error(ERROR_02_TEXT); break;
    case ERROR_03: emit error(ERROR_03_TEXT); break;
    case ERROR_04: emit error(ERROR_04_TEXT); break;
    case ERROR_05: emit error(ERROR_05_TEXT); break;
    case ERROR_06: emit error(ERROR_06_TEXT); break;
    case ERROR_07: emit error(ERROR_07_TEXT); break;
    case ERROR_08: emit error(ERROR_08_TEXT); break;
    case ERROR_09: emit error(ERROR_09_TEXT); break;
    case ERROR_0A: emit error(ERROR_0A_TEXT); break;
    case ERROR_0B: emit error(ERROR_0B_TEXT); break;
    case ERROR_0C: emit error(ERROR_0C_TEXT); break;
    case ERROR_0D: emit error(ERROR_0D_TEXT); break;
    case ERROR_0E: emit error(ERROR_0E_TEXT); break;
    case ERROR_0F: emit error(ERROR_0F_TEXT); break;
    case ERROR_10: emit error(ERROR_10_TEXT); break;
    case ERROR_11: emit error(ERROR_11_TEXT); break;
    case ERROR_12: emit error(ERROR_12_TEXT); break;
    case ERROR_13: emit error(ERROR_13_TEXT); break;
    case ERROR_14: emit error(ERROR_14_TEXT); break;
    case ERROR_15: emit error(ERROR_15_TEXT); break;
    case ERROR_16: emit error(ERROR_16_TEXT); break;
    case ERROR_17: emit error(ERROR_17_TEXT); break;
    default: emit error(QString("Multimeter_V786_2::print_last_error Unknown error %1").arg(last_error)); break;
    }
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::test(void)
{
    QByteArray ba;

    ba.clear();
    ba.append((char)0x34);
    ba.append((char)0x12);
    for(int n=0; n<10; n++)
    {
        ba.append((char)n);
    }
    ba.append((char)26);

    serial.write(convert_data_to_ascii(ba));
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::remote_control_on(void)
{
    V764_2_02_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x02;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_02_QUESTION) - SIZE_DATA);

#ifdef QT_DEBUG
    qDebug() << "multimeter_address" << multimeter_address;
    qDebug() << "multimeter_channel" << multimeter_channel;
#endif
    int err = send(V764_2_CMD_0x02,
                   question.buf,
                   sizeof(V764_2_02_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::remote_control_off(void)
{
    V764_2_03_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x03;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_03_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x03,
                   question.buf,
                   sizeof(V764_2_03_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::power_off(void)
{
    V764_2_05_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x05;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_05_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x05,
                   question.buf,
                   sizeof(V764_2_05_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::continuity(void)
{
    V764_2_0D_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x0D;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_0D_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x0D,
                   question.buf,
                   sizeof(V764_2_0D_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_UDC(void)
{
    V764_2_13_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x13;
    question.body.header.count_data = multimeter_data_len;
    question.body.data.data_0 = multimeter_data_0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_13_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x13,
                   question.buf,
                   sizeof(V764_2_13_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        print_last_error();
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_UAC(void)
{
    V764_2_14_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x14;
    question.body.header.count_data = multimeter_data_len;
    question.body.data.data_0 = multimeter_data_0;
    question.body.data.data_1 = multimeter_data_1;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_14_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x14,
                   question.buf,
                   sizeof(V764_2_14_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_IDC(void)
{
    V764_2_15_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x15;
    question.body.header.count_data = multimeter_data_len;
    question.body.data.data_0 = multimeter_data_0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_15_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x15,
                   question.buf,
                   sizeof(V764_2_15_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_IAC(void)
{
    V764_2_16_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x16;
    question.body.header.count_data = multimeter_data_len;
    question.body.data.data_0 = multimeter_data_0;
    question.body.data.data_1 = multimeter_data_1;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_16_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x16,
                   question.buf,
                   sizeof(V764_2_16_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::calibration_R(void)
{
    V764_2_17_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x17;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_17_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x17,
                   question.buf,
                   sizeof(V764_2_17_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::enabling_charge(void)
{
    V764_2_18_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x18;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_18_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x18,
                   question.buf,
                   sizeof(V764_2_18_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        print_last_error();
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::disabling_charge(void)
{
    V764_2_19_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x19;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_19_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x19,
                   question.buf,
                   sizeof(V764_2_19_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::request_settings_charge(void)
{
    V764_2_1A_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x1A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_1A_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x1A,
                   question.buf,
                   sizeof(V764_2_1A_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    //---
    V764_2_01_ANSWER *b786_battery;
    b786_battery = (V764_2_01_ANSWER *)data_radiomodule.constData();
    emit info(QString(tr("напряжение %1 мВ")).arg(b786_battery->body.data.BattVoltage));
    //---

    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_ID(void)
{
    V764_2_1C_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x1C;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_1C_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x1C,
                   question.buf,
                   sizeof(V764_2_1C_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::wired_off(void)
{
    V764_2_1D_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x1D;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_1D_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x1D,
                   question.buf,
                   sizeof(V764_2_1D_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::get_state(void)
{
    V764_2_01_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x01;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_01_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x01,
                   question.buf,
                   sizeof(V764_2_01_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    if(!is_silence)
    {
        V764_2_01_ANSWER *b786_state;
        b786_state = (V764_2_01_ANSWER *)data_radiomodule.constData();

        emit info(tr("Статус прибора:"));
        emit info(QString(tr("    адрес %1")).arg(b786_state->body.header.address));
        emit info(QString(tr("    серийный номер %1")).arg(b786_state->body.data.serial_year.SerialNo));
        emit info(QString(tr("    год изготовления %1")).arg(b786_state->body.data.serial_year.SerialYear));
        QString temp;
        temp.clear();
        for(unsigned int n=0; n<sizeof(b786_state->body.data.DisplaySymbols); n++)
            temp.append(b786_state->body.data.DisplaySymbols[n]);
        emit info(QString(tr("    отображаемое значение %1")).arg(temp));
        emit info(QString(tr("    предел %1")).arg(b786_state->body.data.Range));
        emit info(QString(tr("    режим %1")).arg(b786_state->body.data.Mode));

        U_BYTE_DECADES temp_decades;
        temp_decades.value = b786_state->body.data.Math_Hold;
        emit info(QString(tr("    math %1")).arg(temp_decades.decades.lo));
        emit info(QString(tr("    hold %1")).arg(temp_decades.decades.hi));
        emit info(QString(tr("    напряжение батареи %1")).arg(b786_state->body.data.BattVoltage));
    }
    //---
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::write_to_flash(void)
{
    V764_2_1B_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x1B;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_1B_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x1B,
                   question.buf,
                   sizeof(V764_2_1B_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CALIBRATION);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    last_error = E_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::send_values(double measuring_value)
{
    emit get_measuring_value(QString("%1").arg(measuring_value));
    emit change_retention(get_retention_string());
    emit change_deviation(get_deviation_string());
    emit change_limit(get_limit_string());
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V786_2::prepare_byte_measuring(void)
{
    BYTE_MEASURING byte_measuring;

    byte_measuring.data.retention = multimeter_retention;
    byte_measuring.data.deviation = multimeter_deviation;

    byte_measuring.data.auto_limit = multimeter_auto_limit;
    byte_measuring.data.limit = multimeter_limit;

    return byte_measuring.value;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_UDC(double *value)
{
    V764_2_06_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x06;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_06_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x06,
                   question.buf,
                   sizeof(V764_2_06_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x06, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_UAC(double *value)
{
    V764_2_07_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x07;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_07_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x07,
                   question.buf,
                   sizeof(V764_2_07_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x07, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_IDC(double *value)
{
    V764_2_08_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x08;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_08_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x08,
                   question.buf,
                   sizeof(V764_2_08_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x08, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_IAC(double *value)
{
    V764_2_09_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x09;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_09_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x09,
                   question.buf,
                   sizeof(V764_2_09_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_UI);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x09, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_R(double *value)
{
    V764_2_0A_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x0A;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_0A_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x0A,
                   question.buf,
                   sizeof(V764_2_0A_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x0A, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_L(double *value)
{
    V764_2_0C_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x0C;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_0C_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x0C,
                   question.buf,
                   sizeof(V764_2_0C_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x0C, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_C(double *value)
{
    V764_2_0B_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x0B;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_0B_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x0B,
                   question.buf,
                   sizeof(V764_2_0B_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x0B, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_F(double *value)
{
    V764_2_0E_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x0E;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_0E_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x0E,
                   question.buf,
                   sizeof(V764_2_0E_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x0E, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        emit get_measuring_value(QString("%1").arg(measuring_value));
        emit change_retention(get_retention_string());
        emit change_deviation(get_deviation_string());
        emit change_limit(get_limit_string());
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_P(double *value)
{
    //активная
    V764_2_0F_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x0F;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_0F_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x0F,
                   question.buf,
                   sizeof(V764_2_0F_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x0F, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_Q(double *value)
{
    //реактивная
    V764_2_10_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x10;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_10_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x10,
                   question.buf,
                   sizeof(V764_2_10_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x10, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_S(double *value)
{
    //полная
    V764_2_11_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x11;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_11_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x11,
                   question.buf,
                   sizeof(V764_2_11_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x11, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::measuring_PF(double *value)
{
    //коэффициент мощности
    V764_2_12_QUESTION question;
#ifdef CALIBRATION_WIRED
    question.body.header.address = FIXED_ADDRESS;
    question.body.channel = FIXED_CHANNEL;
#else
    question.body.header.address = multimeter_address;
    question.body.channel = multimeter_channel;
#endif
    question.body.header.cmd = V764_2_CMD_0x12;
    question.body.header.count_data = 1;
    question.body.data.data_0 = prepare_byte_measuring();
    question.body.crc16 = CRC::multimeter_crc16((unsigned char *)question.buf,
                                                sizeof(V764_2_12_QUESTION) - SIZE_DATA);

    int err = send(V764_2_CMD_0x12,
                   question.buf,
                   sizeof(V764_2_12_QUESTION),
                   multimeter_serno,
                   multimeter_year,
                   MAX_TIMEOUT_MULTIMETER_CONTROL_RLC);
    if(err != E_NO_ERROR)
    {
        last_error = err;
        return false;
    }

    //---
    bool ok = false;
    double measuring_value = 0;
    ok = get_value(V764_2_CMD_0x12, &measuring_value);
    if(ok)
    {
        *value = measuring_value;
        send_values(measuring_value);
        last_error = E_NO_ERROR;
        return true;
    }
    last_error = E_INVALID_FLOAT;
    return false;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::find_cmd(int cmd)
{
    bool ok = false;
    double value = 0;
    switch(cmd)
    {
    case V764_2_CMD_0x01:   ok = get_state(); break;
    case V764_2_CMD_0x02:   ok = remote_control_on(); break;
    case V764_2_CMD_0x03:   ok = remote_control_off(); break;
    case V764_2_CMD_0x04:   ok = service_on(MAX_TIMEOUT_MULTIMETER_CALIBRATION); break;
    case V764_2_CMD_0x05:   ok = power_off(); break;
    case V764_2_CMD_0x06:   ok = measuring_UDC(&value); break;
    case V764_2_CMD_0x07:   ok = measuring_UAC(&value); break;
    case V764_2_CMD_0x08:   ok = measuring_IDC(&value); break;
    case V764_2_CMD_0x09:   ok = measuring_IAC(&value); break;
    case V764_2_CMD_0x0A:   ok = measuring_R(&value); break;
    case V764_2_CMD_0x0B:   ok = measuring_C(&value); break;
    case V764_2_CMD_0x0C:   ok = measuring_L(&value); break;
    case V764_2_CMD_0x0D:   ok = continuity(); break;
    case V764_2_CMD_0x0E:   ok = measuring_F(&value); break;
    case V764_2_CMD_0x0F:   ok = measuring_P(&value); break;
    case V764_2_CMD_0x10:   ok = measuring_Q(&value); break;
    case V764_2_CMD_0x11:   ok = measuring_S(&value); break;
    case V764_2_CMD_0x12:   ok = measuring_PF(&value); break;
    case V764_2_CMD_0x13:   ok = calibration_UDC(); break;
    case V764_2_CMD_0x14:   ok = calibration_UAC(); break;
    case V764_2_CMD_0x15:   ok = calibration_IDC(); break;
    case V764_2_CMD_0x16:   ok = calibration_IAC(); break;
    case V764_2_CMD_0x17:   ok = calibration_R(); break;
    case V764_2_CMD_0x18:   ok = enabling_charge(); break;
    case V764_2_CMD_0x19:   ok = disabling_charge(); break;
    case V764_2_CMD_0x1A:   ok = request_settings_charge(); break;
    case V764_2_CMD_0x1B:   ok = write_to_flash(); break;
    case V764_2_CMD_0x1C:   ok = get_ID(); break;
    case V764_2_CMD_0x1D:   ok = wired_off(); break;
    }
    return ok;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_test_state_answer(int cmd,
                                                    int count_data,
                                                    int serial_no,
                                                    int serial_year,
                                                    int range,
                                                    int mode,
                                                    int math_hold,
                                                    int batt_voltage)
{
    V764_2_01_ANSWER answer;
    answer.body.header.address = multimeter_address;
    answer.body.header.cmd = cmd;
    answer.body.header.count_data = count_data;
    answer.body.data.serial_year.SerialNo = serial_no;
    answer.body.data.serial_year.SerialYear = serial_year;
    for(int n=0; n<7; n++)
        answer.body.data.DisplaySymbols[n] = '0';
    answer.body.data.Range = range;
    answer.body.data.Mode = mode;
    answer.body.data.Math_Hold = math_hold;
    answer.body.data.BattVoltage = batt_voltage;
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_01_ANSWER) - SIZE_DATA);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_01_ANSWER); n++)
        ba.append(answer.buf[n]);

    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_test_service_on_answer(int cmd,
                                                         int count_data,
                                                         int serial_no,
                                                         int serial_year)
{
    V764_2_04_ANSWER answer;
    answer.body.header.address = multimeter_address;
    answer.body.header.cmd = cmd;
    answer.body.header.count_data = count_data;
    answer.body.data.serial_year.SerialNo = serial_no;
    answer.body.data.serial_year.SerialYear = serial_year;
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_04_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_04_QUESTION); n++)
        ba.append(answer.buf[n]);

#ifdef QT_DEBUG
    qDebug() << ba.toHex();
#endif
    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_test_power_off_answer(int cmd,
                                                        int count_data,
                                                        int serial_no,
                                                        int serial_year)
{
    V764_2_05_ANSWER answer;
    answer.body.header.address = multimeter_address;
    answer.body.header.cmd = cmd;
    answer.body.header.count_data = count_data;
    answer.body.data.serial_year.SerialNo = serial_no;
    answer.body.data.serial_year.SerialYear = serial_year;
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_05_QUESTION) - SIZE_DATA);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_05_QUESTION); n++)
        ba.append(answer.buf[n]);

    return ba;
}
//--------------------------------------------------------------------------------
QByteArray Multimeter_V786_2::get_get_id_answer(int id,
                                                int count_data,
                                                int c1,
                                                int c2,
                                                int c3)
{
    V764_2_1C_ANSWER answer;
    answer.body.header.address = multimeter_address;
    answer.body.header.cmd = id;
    answer.body.header.count_data = count_data;
    answer.body.data.byte_C1 = c1;
    answer.body.data.byte_C2 = c2;
    answer.body.data.byte_C3 = c3;
    answer.body.crc16 = CRC::multimeter_crc16((unsigned char *)&answer.buf, sizeof(V764_2_1C_ANSWER) - SIZE_DATA);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(V764_2_1C_ANSWER); n++)
        ba.append(answer.buf[n]);

    return ba;
}
//--------------------------------------------------------------------------------
void Multimeter_V786_2::print_cmd_text(int cmd)
{
    switch(cmd)
    {
    case V764_2_CMD_0x01:   emit info(V764_2_CMD_0x01_TEXT);   break;
    case V764_2_CMD_0x02:   emit info(V764_2_CMD_0x02_TEXT);   break;
    case V764_2_CMD_0x03:   emit info(V764_2_CMD_0x03_TEXT);   break;
    case V764_2_CMD_0x04:   emit info(V764_2_CMD_0x04_TEXT);   break;
    case V764_2_CMD_0x05:   emit info(V764_2_CMD_0x05_TEXT);   break;
    case V764_2_CMD_0x06:   emit info(V764_2_CMD_0x06_TEXT);   break;
    case V764_2_CMD_0x07:   emit info(V764_2_CMD_0x07_TEXT);   break;
    case V764_2_CMD_0x08:   emit info(V764_2_CMD_0x08_TEXT);   break;
    case V764_2_CMD_0x09:   emit info(V764_2_CMD_0x09_TEXT);   break;
    case V764_2_CMD_0x0A:   emit info(V764_2_CMD_0x0A_TEXT);   break;
    case V764_2_CMD_0x0B:   emit info(V764_2_CMD_0x0B_TEXT);   break;
    case V764_2_CMD_0x0C:   emit info(V764_2_CMD_0x0C_TEXT);   break;
    case V764_2_CMD_0x0D:   emit info(V764_2_CMD_0x0D_TEXT);   break;
    case V764_2_CMD_0x0E:   emit info(V764_2_CMD_0x0E_TEXT);   break;
    case V764_2_CMD_0x0F:   emit info(V764_2_CMD_0x0F_TEXT);   break;
    case V764_2_CMD_0x10:   emit info(V764_2_CMD_0x10_TEXT);   break;
    case V764_2_CMD_0x11:   emit info(V764_2_CMD_0x11_TEXT);   break;
    case V764_2_CMD_0x12:   emit info(V764_2_CMD_0x12_TEXT);   break;
    case V764_2_CMD_0x13:   emit info(V764_2_CMD_0x13_TEXT);   break;
    case V764_2_CMD_0x14:   emit info(V764_2_CMD_0x14_TEXT);   break;
    case V764_2_CMD_0x15:   emit info(V764_2_CMD_0x15_TEXT);   break;
    case V764_2_CMD_0x16:   emit info(V764_2_CMD_0x16_TEXT);   break;
    case V764_2_CMD_0x17:   emit info(V764_2_CMD_0x17_TEXT);   break;
    case V764_2_CMD_0x18:   emit info(V764_2_CMD_0x18_TEXT);   break;
    case V764_2_CMD_0x19:   emit info(V764_2_CMD_0x19_TEXT);   break;
    case V764_2_CMD_0x1A:   emit info(V764_2_CMD_0x1A_TEXT);   break;
    case V764_2_CMD_0x1B:   emit info(V764_2_CMD_0x1B_TEXT);   break;
    case V764_2_CMD_0x1C:   emit info(V764_2_CMD_0x1C_TEXT);   break;
    case V764_2_CMD_0x1D:   emit info(V764_2_CMD_0x1D_TEXT);   break;

    default:
#ifdef QT_DEBUG
        qDebug() << "unknown cmd" << cmd;
#endif
        break;
    }
}
//--------------------------------------------------------------------------------
QString Multimeter_V786_2::get_cmd_text(int cmd)
{
    QString temp;
    switch(cmd)
    {
    case V764_2_CMD_0x01:   temp = V764_2_CMD_0x01_TEXT;   break;
    case V764_2_CMD_0x02:   temp = V764_2_CMD_0x02_TEXT;   break;
    case V764_2_CMD_0x03:   temp = V764_2_CMD_0x03_TEXT;   break;
    case V764_2_CMD_0x04:   temp = V764_2_CMD_0x04_TEXT;   break;
    case V764_2_CMD_0x05:   temp = V764_2_CMD_0x05_TEXT;   break;
    case V764_2_CMD_0x06:   temp = V764_2_CMD_0x06_TEXT;   break;
    case V764_2_CMD_0x07:   temp = V764_2_CMD_0x07_TEXT;   break;
    case V764_2_CMD_0x08:   temp = V764_2_CMD_0x08_TEXT;   break;
    case V764_2_CMD_0x09:   temp = V764_2_CMD_0x09_TEXT;   break;
    case V764_2_CMD_0x0A:   temp = V764_2_CMD_0x0A_TEXT;   break;
    case V764_2_CMD_0x0B:   temp = V764_2_CMD_0x0B_TEXT;   break;
    case V764_2_CMD_0x0C:   temp = V764_2_CMD_0x0C_TEXT;   break;
    case V764_2_CMD_0x0D:   temp = V764_2_CMD_0x0D_TEXT;   break;
    case V764_2_CMD_0x0E:   temp = V764_2_CMD_0x0E_TEXT;   break;
    case V764_2_CMD_0x0F:   temp = V764_2_CMD_0x0F_TEXT;   break;
    case V764_2_CMD_0x10:   temp = V764_2_CMD_0x10_TEXT;   break;
    case V764_2_CMD_0x11:   temp = V764_2_CMD_0x11_TEXT;   break;
    case V764_2_CMD_0x12:   temp = V764_2_CMD_0x12_TEXT;   break;
    case V764_2_CMD_0x13:   temp = V764_2_CMD_0x13_TEXT;   break;
    case V764_2_CMD_0x14:   temp = V764_2_CMD_0x14_TEXT;   break;
    case V764_2_CMD_0x15:   temp = V764_2_CMD_0x15_TEXT;   break;
    case V764_2_CMD_0x16:   temp = V764_2_CMD_0x16_TEXT;   break;
    case V764_2_CMD_0x17:   temp = V764_2_CMD_0x17_TEXT;   break;
    case V764_2_CMD_0x18:   temp = V764_2_CMD_0x18_TEXT;   break;
    case V764_2_CMD_0x19:   temp = V764_2_CMD_0x19_TEXT;   break;
    case V764_2_CMD_0x1A:   temp = V764_2_CMD_0x1A_TEXT;   break;
    case V764_2_CMD_0x1B:   temp = V764_2_CMD_0x1B_TEXT;   break;
    case V764_2_CMD_0x1C:   temp = V764_2_CMD_0x1C_TEXT;   break;
    case V764_2_CMD_0x1D:   temp = V764_2_CMD_0x1D_TEXT;   break;

    default:
        temp = QString("unknown cmd %1").arg(cmd);
        break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void Multimeter_V786_2::receive(const QVariant &data)
{
    serial.receive(data);
}
#endif
//--------------------------------------------------------------------------------
QMenu *Multimeter_V786_2::add_menu(void)
{
    QMenu *menu = new QMenu(tr("Мультиметр В786/2"));
    QAction *action_m_0x01 = new QAction(menu);
    QAction *action_m_0x02 = new QAction(menu);
    QAction *action_m_0x03 = new QAction(menu);
    QAction *action_m_0x04 = new QAction(menu);
    QAction *action_m_0x05 = new QAction(menu);
    QAction *action_m_0x06 = new QAction(menu);
    QAction *action_m_0x07 = new QAction(menu);
    QAction *action_m_0x08 = new QAction(menu);
    QAction *action_m_0x09 = new QAction(menu);
    QAction *action_m_0x0A = new QAction(menu);
    QAction *action_m_0x0B = new QAction(menu);
    QAction *action_m_0x0C = new QAction(menu);
    QAction *action_m_0x0D = new QAction(menu);
    QAction *action_m_0x0E = new QAction(menu);
    QAction *action_m_0x0F = new QAction(menu);
    QAction *action_m_0x10 = new QAction(menu);
    QAction *action_m_0x11 = new QAction(menu);
    QAction *action_m_0x12 = new QAction(menu);
    QAction *action_m_0x13 = new QAction(menu);
    QAction *action_m_0x14 = new QAction(menu);
    QAction *action_m_0x15 = new QAction(menu);
    QAction *action_m_0x16 = new QAction(menu);
    QAction *action_m_0x17 = new QAction(menu);
    QAction *action_m_0x18 = new QAction(menu);
    QAction *action_m_0x19 = new QAction(menu);
    QAction *action_m_0x1A = new QAction(menu);
    QAction *action_m_0x1B = new QAction(menu);
    QAction *action_m_0x1C = new QAction(menu);
    QAction *action_m_0x1D = new QAction(menu);

    action_m_0x01->setText(V764_2_CMD_0x01_TEXT);
    action_m_0x02->setText(V764_2_CMD_0x02_TEXT);
    action_m_0x03->setText(V764_2_CMD_0x03_TEXT);
    action_m_0x04->setText(V764_2_CMD_0x04_TEXT);
    action_m_0x05->setText(V764_2_CMD_0x05_TEXT);
    action_m_0x06->setText(V764_2_CMD_0x06_TEXT);
    action_m_0x07->setText(V764_2_CMD_0x07_TEXT);
    action_m_0x08->setText(V764_2_CMD_0x08_TEXT);
    action_m_0x09->setText(V764_2_CMD_0x09_TEXT);
    action_m_0x0A->setText(V764_2_CMD_0x0A_TEXT);
    action_m_0x0B->setText(V764_2_CMD_0x0B_TEXT);
    action_m_0x0C->setText(V764_2_CMD_0x0C_TEXT);
    action_m_0x0D->setText(V764_2_CMD_0x0D_TEXT);
    action_m_0x0E->setText(V764_2_CMD_0x0E_TEXT);
    action_m_0x0F->setText(V764_2_CMD_0x0F_TEXT);
    action_m_0x10->setText(V764_2_CMD_0x10_TEXT);
    action_m_0x11->setText(V764_2_CMD_0x11_TEXT);
    action_m_0x12->setText(V764_2_CMD_0x12_TEXT);
    action_m_0x13->setText(V764_2_CMD_0x13_TEXT);
    action_m_0x14->setText(V764_2_CMD_0x14_TEXT);
    action_m_0x15->setText(V764_2_CMD_0x15_TEXT);
    action_m_0x16->setText(V764_2_CMD_0x16_TEXT);
    action_m_0x17->setText(V764_2_CMD_0x17_TEXT);
    action_m_0x18->setText(V764_2_CMD_0x18_TEXT);
    action_m_0x19->setText(V764_2_CMD_0x19_TEXT);
    action_m_0x1A->setText(V764_2_CMD_0x1A_TEXT);
    action_m_0x1B->setText(V764_2_CMD_0x1B_TEXT);
    action_m_0x1C->setText(V764_2_CMD_0x1C_TEXT);
    action_m_0x1D->setText(V764_2_CMD_0x1D_TEXT);

    menu->addAction(action_m_0x01);
    menu->addAction(action_m_0x02);
    menu->addAction(action_m_0x03);
    menu->addAction(action_m_0x04);
    menu->addAction(action_m_0x05);
    menu->addAction(action_m_0x06);
    menu->addAction(action_m_0x07);
    menu->addAction(action_m_0x08);
    menu->addAction(action_m_0x09);
    menu->addAction(action_m_0x0A);
    menu->addAction(action_m_0x0B);
    menu->addAction(action_m_0x0C);
    menu->addAction(action_m_0x0D);
    menu->addAction(action_m_0x0E);
    menu->addAction(action_m_0x0F);
    menu->addAction(action_m_0x10);
    menu->addAction(action_m_0x11);
    menu->addAction(action_m_0x12);
    menu->addAction(action_m_0x13);
    menu->addAction(action_m_0x14);
    menu->addAction(action_m_0x15);
    menu->addAction(action_m_0x16);
    menu->addAction(action_m_0x17);
    menu->addAction(action_m_0x18);
    menu->addAction(action_m_0x19);
    menu->addAction(action_m_0x1A);
    menu->addAction(action_m_0x1B);
    menu->addAction(action_m_0x1C);
    menu->addAction(action_m_0x1D);

    connect(action_m_0x01, SIGNAL(triggered()), this, SLOT(run_dialog_0x01()));
    connect(action_m_0x02, SIGNAL(triggered()), this, SLOT(run_dialog_0x02()));
    connect(action_m_0x03, SIGNAL(triggered()), this, SLOT(run_dialog_0x03()));
    connect(action_m_0x04, SIGNAL(triggered()), this, SLOT(run_dialog_0x04()));
    connect(action_m_0x05, SIGNAL(triggered()), this, SLOT(run_dialog_0x05()));
    connect(action_m_0x06, SIGNAL(triggered()), this, SLOT(run_dialog_0x06()));
    connect(action_m_0x07, SIGNAL(triggered()), this, SLOT(run_dialog_0x07()));
    connect(action_m_0x08, SIGNAL(triggered()), this, SLOT(run_dialog_0x08()));
    connect(action_m_0x09, SIGNAL(triggered()), this, SLOT(run_dialog_0x09()));
    connect(action_m_0x0A, SIGNAL(triggered()), this, SLOT(run_dialog_0x0A()));
    connect(action_m_0x0B, SIGNAL(triggered()), this, SLOT(run_dialog_0x0B()));
    connect(action_m_0x0C, SIGNAL(triggered()), this, SLOT(run_dialog_0x0C()));
    connect(action_m_0x0D, SIGNAL(triggered()), this, SLOT(run_dialog_0x0D()));
    connect(action_m_0x0E, SIGNAL(triggered()), this, SLOT(run_dialog_0x0E()));
    connect(action_m_0x0F, SIGNAL(triggered()), this, SLOT(run_dialog_0x0F()));
    connect(action_m_0x10, SIGNAL(triggered()), this, SLOT(run_dialog_0x10()));
    connect(action_m_0x11, SIGNAL(triggered()), this, SLOT(run_dialog_0x11()));
    connect(action_m_0x12, SIGNAL(triggered()), this, SLOT(run_dialog_0x12()));
    connect(action_m_0x13, SIGNAL(triggered()), this, SLOT(run_dialog_0x13()));
    connect(action_m_0x14, SIGNAL(triggered()), this, SLOT(run_dialog_0x14()));
    connect(action_m_0x15, SIGNAL(triggered()), this, SLOT(run_dialog_0x15()));
    connect(action_m_0x16, SIGNAL(triggered()), this, SLOT(run_dialog_0x16()));
    connect(action_m_0x17, SIGNAL(triggered()), this, SLOT(run_dialog_0x17()));
    connect(action_m_0x18, SIGNAL(triggered()), this, SLOT(run_dialog_0x18()));
    connect(action_m_0x19, SIGNAL(triggered()), this, SLOT(run_dialog_0x19()));
    connect(action_m_0x1A, SIGNAL(triggered()), this, SLOT(run_dialog_0x1A()));
    connect(action_m_0x1B, SIGNAL(triggered()), this, SLOT(run_dialog_0x1B()));
    connect(action_m_0x1C, SIGNAL(triggered()), this, SLOT(run_dialog_0x1C()));
    connect(action_m_0x1D, SIGNAL(triggered()), this, SLOT(run_dialog_0x1D()));

    return menu;
}
//--------------------------------------------------------------------------------
bool Multimeter_V786_2::choice_options(int cmd,
                                       int *serno_value,
                                       int *year_value,
                                       int *address_value,
                                       int *channel_value)
{
    QSpinBox *serno   = new QSpinBox();
    QSpinBox *year    = new QSpinBox();
    QSpinBox *address = new QSpinBox();
    QSpinBox *channel = new QSpinBox();

    address->setMinimum(0);
    address->setMaximum(255);

    serno->setMinimum(0);
    serno->setMaximum(9999);

    year->setMinimum(2013);
    year->setMaximum(2100);

    channel->setMinimum(1);
    channel->setMaximum(15);

    set_serno(serno->value());
    set_year(year->value());
    set_address(address->value());
    set_channel(channel->value());
    set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);

    QPushButton *btn_ok = new QPushButton("OK");
    QPushButton *btn_cancel = new QPushButton("Cancel");

    btn_ok->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOkButton));
    btn_cancel->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogCancelButton));

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("serno"),   0, 0);
    grid->addWidget(serno,                 0, 1);
    grid->addWidget(new QLabel("year"),    1, 0);
    grid->addWidget(year,                  1, 1);
    grid->addWidget(new QLabel("address"), 2, 0);
    grid->addWidget(address,               2, 1);
    grid->addWidget(new QLabel("channel"), 3, 0);
    grid->addWidget(channel,               3, 1);
    grid->addWidget(btn_ok,                4, 0);
    grid->addWidget(btn_cancel,            4, 1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(new QLabel(get_cmd_text(cmd)));
    vbox->addLayout(grid);

    QDialog *dlg = new QDialog();
    dlg->setLayout(vbox);
    //dlg->setFixedSize(dlg->sizeHint());

    connect(btn_ok,     SIGNAL(clicked()), dlg, SLOT(accept()));
    connect(btn_cancel, SIGNAL(clicked()), dlg, SLOT(reject()));

    int button = dlg->exec();
    if(button == QDialog::Accepted)
    {
        *serno_value = serno->value();
        *year_value  = year->value();
        *address_value = address->value();
        *channel_value = channel->value();
    }
    return (button == QDialog::Accepted);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::send_cmd(int cmd)
{
    if(!serial.isOpen())
    {
        last_error = E_ERROR_PORT_NOT_OPEN;
        return print_last_error();
    }

    bool ok = false;

    int serno_value = 0;
    int year_value = 0;
    int address_value = 0;
    int channel_value = 0;

    ok = choice_options(cmd, &serno_value, &year_value, &address_value, &channel_value);
    if(ok)
    {
        set_serno(serno_value);
        set_year(year_value);
        set_address(address_value);
        set_channel(channel_value);

        print_cmd_text(cmd);
        ok = find_cmd(cmd);
        if(!ok)
        {
            return print_last_error();
        }
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x01(void)
{
    return send_cmd(0x01);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x02(void)
{
    return send_cmd(0x02);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x03(void)
{
    return send_cmd(0x03);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x04(void)
{
    return send_cmd(0x04);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x05(void)
{
    return send_cmd(0x05);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x06(void)
{
    return send_cmd(0x06);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x07(void)
{
    return send_cmd(0x07);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x08(void)
{
    return send_cmd(0x08);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x09(void)
{
    return send_cmd(0x09);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0A(void)
{
    return send_cmd(0x0A);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0B(void)
{
    return send_cmd(0x0B);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0C(void)
{
    return send_cmd(0x0C);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0D(void)
{
    return send_cmd(0x0D);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0E(void)
{
    return send_cmd(0x0E);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x0F(void)
{
    return send_cmd(0x0F);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x10(void)
{
    return send_cmd(0x10);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x11(void)
{
    return send_cmd(0x11);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x12(void)
{
    return send_cmd(0x12);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x13(void)
{
    return send_cmd(0x13);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x14(void)
{
    return send_cmd(0x14);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x15(void)
{
    return send_cmd(0x15);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x16(void)
{
    return send_cmd(0x16);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x17(void)
{
    return send_cmd(0x17);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x18(void)
{
    return send_cmd(0x18);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x19(void)
{
    return send_cmd(0x19);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1A(void)
{
    return send_cmd(0x1A);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1B(void)
{
    return send_cmd(0x1B);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1C(void)
{
    return send_cmd(0x1C);
}
//--------------------------------------------------------------------------------
int Multimeter_V786_2::run_dialog_0x1D(void)
{
    return send_cmd(0x1D);
}
//--------------------------------------------------------------------------------
