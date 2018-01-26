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
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QSerialPort>
#include <QGridLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QObject>
#include <QTime>
#include <QMenu>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "b590_powersupply.hpp"
#include "b590_packets.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "crc.h"

#include "find_powersupply.hpp"

#ifdef NEEDED_PAUSE
#include "sleeper.h"
#endif
//--------------------------------------------------------------------------------
union U_BYTE
{
    uint8_t value;
    struct
    {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
    } bites;
};

union U_SHORT
{
    uint16_t value;
    struct
    {
        uint16_t bit0:1;
        uint16_t bit1:1;
        uint16_t bit2:1;
        uint16_t bit3:1;
        uint16_t bit4:1;
        uint16_t bit5:1;
        uint16_t bit6:1;
        uint16_t bit7:1;
        uint16_t bit8:1;
        uint16_t bit9:1;
        uint16_t bit10:1;
        uint16_t bit11:1;
        uint16_t bit12:1;
        uint16_t bit13:1;
        uint16_t bit14:1;
        uint16_t bit15:1;
    } bites;
};

union U_INT
{
    uint32_t value;
    struct
    {
        uint32_t bit0:1;
        uint32_t bit1:1;
        uint32_t bit2:1;
        uint32_t bit3:1;
        uint32_t bit4:1;
        uint32_t bit5:1;
        uint32_t bit6:1;
        uint32_t bit7:1;
        uint32_t bit8:1;
        uint32_t bit9:1;
        uint32_t bit10:1;
        uint32_t bit11:1;
        uint32_t bit12:1;
        uint32_t bit13:1;
        uint32_t bit14:1;
        uint32_t bit15:1;
        uint32_t bit16:1;
        uint32_t bit17:1;
        uint32_t bit18:1;
        uint32_t bit19:1;
        uint32_t bit20:1;
        uint32_t bit21:1;
        uint32_t control:2;
        uint32_t bit24:1;
        uint32_t bit25:1;
        uint32_t bit26:1;
        uint32_t bit27:1;
        uint32_t bit28:1;
        uint32_t bit29:1;
        uint32_t bit30:1;
        uint32_t bit31:1;
    } bites;
};

union U_SHORT_BYTES
{
    uint16_t value;
    struct
    {
        uint8_t byte_0;
        uint8_t byte_1;
    } bytes;
};

union INT_BYTES
{
    int32_t value;
    struct
    {
        uint8_t byte_0;
        uint8_t byte_1;
        uint8_t byte_2;
        uint8_t byte_3;
    } bytes;
};

union FLOAT_BYTES
{
    float value;
    struct
    {
        uint8_t byte_0;
        uint8_t byte_1;
        uint8_t byte_2;
        uint8_t byte_3;
    } bytes;
};
//--------------------------------------------------------------------------------
#define MAX_NUM_SPEED 8
//--------------------------------------------------------------------------------
Powersupply_B590::Powersupply_B590(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Powersupply_B590::~Powersupply_B590()
{
#ifdef QT_DEBUG
    qDebug() << "~Powersupply_B590()";
#endif
#if 0
    if(serial.isOpen())
    {
        serial.close();
    }
#endif
}
//--------------------------------------------------------------------------------
void Powersupply_B590::init(void)
{
    last_error = E_B590_NO_ERROR;
    busy = false;

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

#ifdef FAKE
    connect(&serial, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(&serial, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(&serial, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(&serial, SIGNAL(trace(QString)),    this, SIGNAL(trace(QString)));
#endif
}
//--------------------------------------------------------------------------------
void Powersupply_B590::set_ignore_bad_cmd(bool state)
{
    ignore_bad_cmd = state;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::test(void)
{
    return false;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::find_device(void)
{
    QList<int> speeds;

    speeds.clear();

    //TODO speeds
    speeds.append(4800);
    speeds.append(9600);
    speeds.append(19200);
    speeds.append(28800);
    speeds.append(38400);
    speeds.append(57600);
    speeds.append(76800);
    speeds.append(115200);

    if(serial.isOpen())
    {
        serial.close();
    }

    flag_closed = false;
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("ищем источник питания на порту %1")).arg(port.portName()));
        foreach (int speed, speeds)
        {
            if(flag_closed) return false;

            serial.setPort(port);
            if(serial.open(QIODevice::ReadWrite))
            {
                emit info(QString("BaudRate %1").arg(speed));
                bool ok = serial.setBaudRate(speed);
                if(ok)
                {
#ifndef FAKE
                    if(serial.baudRate() != speed)
                    {
                        emit error(QString("ERROR: %1 != %2")
                                   .arg(serial.baudRate())
                                   .arg(speed));
                    }
#endif
                    uint8_t  type  = 0;
                    uint16_t year  = 0;
                    uint8_t  month = 0;
                    uint16_t serno = 0;
                    int err = send_cmd_0x46(&type, &year, &month, &serno);
                    if(err == E_B590_NO_ERROR)
                    {
                        emit info(tr("Проверяем идентификатор источника"));
                        if(type == 4)
                        {
                            emit info(QString(tr("Источник питания найден на скорости %1")).arg(speed));
                            return true;
                        }
                        else
                        {
                            emit error(QString("Найдено неизвестное устройство: type == %1").arg(type));
                        }
                    }
                    if((err != E_B590_ERROR_NO_ANSWER) && (err != E_B590_NO_ERROR))
                    {
                        print_last_error();
                    }
                }
                serial.close();
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::port_is_open(void)
{
    return  serial.isOpen();
}
//--------------------------------------------------------------------------------
void Powersupply_B590::port_read(void)
{
    data_powersupply.append(serial.readAll());
    is_ready = true;
}
//--------------------------------------------------------------------------------
void Powersupply_B590::port_error(QSerialPort::SerialPortError serial_error)
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
    default:
        emit error(QString("Unknown error %1").arg(serial_error));
        break;
    }
}
//--------------------------------------------------------------------------------
void Powersupply_B590::set_delay_ms(int delay_ms)
{
    emit info(QString("new delay %1 ms").arg(delay_ms));
    this->delay_ms = delay_ms;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::get_delay_ms(void)
{
    return delay_ms;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::check_dirty_packet(const QByteArray &data)
{
    if(data.length() < 2) return PACKET_B590_FAIL;

    unsigned char cmd = data.at(1) | 0x7F;
    //unsigned char err = data.at(1) | 0x80;

    int result = PACKET_B590_OK;
    switch(cmd)
    {
    case B590_CMD_0x41: result = PACKET_B590_OK; break;
    case B590_CMD_0x42: result = PACKET_B590_OK; break;
    case B590_CMD_0x46: result = check_packet_answer(B590_CMD_0x46, sizeof(B590_CMD_46_ANSWER)); break;
    case B590_CMD_0x47: result = check_packet_answer(B590_CMD_0x47, sizeof(B590_CMD_47_ANSWER)); break;
    case B590_CMD_0x48: result = check_packet_answer(B590_CMD_0x48, sizeof(B590_CMD_48_ANSWER)); break;
    case B590_CMD_0x49: result = check_packet_answer(B590_CMD_0x49, sizeof(B590_CMD_49_ANSWER)); break;
    case B590_CMD_0x4A: result = check_packet_answer(B590_CMD_0x4A, sizeof(B590_CMD_4A_ANSWER)); break;
    case B590_CMD_0x54: result = check_packet_answer(B590_CMD_0x54, sizeof(B590_CMD_54_ANSWER)); break;
    case B590_CMD_0x55: result = check_packet_answer(B590_CMD_0x55, sizeof(B590_CMD_55_ANSWER)); break;
    case B590_CMD_0x56: result = check_packet_answer(B590_CMD_0x56, sizeof(B590_CMD_56_ANSWER)); break;
    case B590_CMD_0x57: result = check_packet_answer(B590_CMD_0x57, sizeof(B590_CMD_57_ANSWER)); break;
    case B590_CMD_0x5A: result = check_packet_answer(B590_CMD_0x5A, sizeof(B590_CMD_5A_ANSWER)); break;
    case B590_CMD_0x5B: result = check_packet_answer(B590_CMD_0x5B, sizeof(B590_CMD_5B_ANSWER)); break;
    case B590_CMD_0x5C: result = check_packet_answer(B590_CMD_0x5C, sizeof(B590_CMD_5C_ANSWER)); break;
    case B590_CMD_0x5D: result = check_packet_answer(B590_CMD_0x5D, sizeof(B590_CMD_5D_ANSWER)); break;
    case B590_CMD_0x5E: result = check_packet_answer(B590_CMD_0x5E, sizeof(B590_CMD_5E_ANSWER)); break;
    case B590_CMD_0x5F: result = check_packet_answer(B590_CMD_0x5F, sizeof(B590_CMD_5F_ANSWER)); break;
    case B590_CMD_0x60: result = check_packet_answer(B590_CMD_0x60, sizeof(B590_CMD_60_ANSWER)); break;
    case B590_CMD_0x63: result = check_packet_answer(B590_CMD_0x63, sizeof(B590_CMD_63_ANSWER)); break;
    case B590_CMD_0x64: result = check_packet_answer(B590_CMD_0x64, sizeof(B590_CMD_64_ANSWER)); break;
    case B590_CMD_0x65: result = check_packet_answer(B590_CMD_0x65, sizeof(B590_CMD_65_ANSWER)); break;
    case B590_CMD_0x66: result = check_packet_answer(B590_CMD_0x66, sizeof(B590_CMD_66_ANSWER)); break;
    case B590_CMD_0x67: result = check_packet_answer(B590_CMD_0x67, sizeof(B590_CMD_67_ANSWER)); break;
    case B590_CMD_0x68: result = check_packet_answer(B590_CMD_0x68, sizeof(B590_CMD_68_ANSWER)); break;
    case B590_CMD_0x69: result = check_packet_answer(B590_CMD_0x69, sizeof(B590_CMD_69_ANSWER)); break;
    case B590_CMD_0x6A: result = check_packet_answer(B590_CMD_0x6A, sizeof(B590_CMD_6A_ANSWER)); break;
    case B590_CMD_0x6B: result = check_packet_answer(B590_CMD_0x6B, sizeof(B590_CMD_6B_ANSWER)); break;
    case B590_CMD_0x6D: result = check_packet_answer(B590_CMD_0x6D, sizeof(B590_CMD_6D_ANSWER)); break;
    case B590_CMD_0x6E: result = check_packet_answer(B590_CMD_0x6E, sizeof(B590_CMD_6E_ANSWER)); break;
    case B590_CMD_0x6F: result = check_packet_answer(B590_CMD_0x6F, sizeof(B590_CMD_6F_ANSWER)); break;
    case B590_CMD_0x71: result = check_packet_answer(B590_CMD_0x71, sizeof(B590_CMD_71_ANSWER)); break;
    case B590_CMD_0x72: result = check_packet_answer(B590_CMD_0x72, sizeof(B590_CMD_72_ANSWER)); break;
    case B590_CMD_0x73: result = check_packet_answer(B590_CMD_0x73, sizeof(B590_CMD_73_ANSWER)); break;
    case B590_CMD_0x75: result = check_packet_answer(B590_CMD_0x75, sizeof(B590_CMD_75_ANSWER)); break;

    default:
        result = PACKET_B590_FAIL;
        break;

    }

    return result;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x11(void)
{
    emit trace("send_cmd_0x11");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x11_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_11_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x11;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_11_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_11_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_11_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x11, sizeof(B590_CMD_11_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x12(void)
{
    emit trace("send_cmd_0x12");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x12_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_12_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x12;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_12_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_12_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_12_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x12, sizeof(B590_CMD_12_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x15(void)
{
    emit trace("send_cmd_0x15");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x15_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_15_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x15;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_15_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_15_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_15_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x15, sizeof(B590_CMD_15_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x16(void)
{
    emit trace("send_cmd_0x16");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x16_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_16_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x16;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_16_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_16_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_16_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x16, sizeof(B590_CMD_16_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x17(void)
{
    emit trace("send_cmd_0x17");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x17_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_17_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x17;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_17_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_17_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_17_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x17, sizeof(B590_CMD_17_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x41(void)
{
    emit trace("send_cmd_0x41");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x41_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_41_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x41;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_41_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_41_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_41_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x41, sizeof(B590_CMD_41_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x42(unsigned char code_speed)
{
    emit trace("send_cmd_0x42");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x42_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_42_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x42;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.code_speed = code_speed;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_42_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_42_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_42_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x42, sizeof(B590_CMD_42_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::check_packet_answer(unsigned char cmd,
                                          int packet_len)
{
    emit trace("check_packet_answer");
    if(data_powersupply.isEmpty())
    {
        emit debug("E_B590_ERROR_NO_ANSWER");
        last_error = E_B590_ERROR_NO_ANSWER;
        return last_error;
    }

    emit debug(QString("check_packet_answer: [%1]").arg(data_powersupply.toHex().data()));

    if(data_powersupply.length() < 2)
    {
        emit debug(QString("E_B590_ERROR_SMALL_ANSWER: [%1]").arg(data_powersupply.toHex().data()));
        last_error = E_B590_ERROR_SMALL_ANSWER;
        return last_error;
    }

    if(!ignore_bad_cmd)
    {
        unsigned char temp = data_powersupply.at(1) & 0x80;
        unsigned char data_cmd = data_powersupply.at(1) & 0x7F;
        if(data_cmd != cmd)
        {
            emit debug("E_B590_ERROR_UNKNOWN_COMMAND");
            last_error = E_B590_ERROR_UNKNOWN_COMMAND;
            return last_error;
        }
        if(temp)
        {
            emit debug("E_B590_ERROR_BAD_COMMAND");
            last_error = E_B590_ERROR_BAD_COMMAND;
            return last_error;
        }
    }

    if(data_powersupply.length() > packet_len)
    {
        emit debug(QString("E_B590_ERROR_BIG_ANSWER: [%1]").arg(data_powersupply.toHex().data()));
#ifdef QT_DEBUG
        qDebug() << "data_powersupply.length()" << data_powersupply.length() << "packet_len" << packet_len;
        qDebug() << "data" << data_powersupply.toHex();
#endif
        last_error = E_B590_ERROR_BIG_ANSWER;
        return last_error;
    }
    if(data_powersupply.length() < packet_len)
    {
        emit debug(QString("E_B590_ERROR_SMALL_ANSWER: [%1]").arg(data_powersupply.toHex().data()));
#ifdef QT_DEBUG
        qDebug() << "data_powersupply.length()" << data_powersupply.length() << "packet_len" << packet_len;
        qDebug() << "data" << data_powersupply.toHex();
#endif
        last_error = E_B590_ERROR_SMALL_ANSWER;
        return last_error;
    }

    unsigned short calc_crc16 = CRC::powersupply_crc16((unsigned char *)data_powersupply.constData(),
                                                       packet_len - 2);

    union U_SHORT_BYTES temp_crc;
    temp_crc.bytes.byte_0 = data_powersupply.at(packet_len - 2);
    temp_crc.bytes.byte_1 = data_powersupply.at(packet_len - 1);

    unsigned short data_crc16 = temp_crc.value;
    if(data_crc16 != calc_crc16)
    {
        emit debug("E_B590_ERROR_BAD_CRC");
        last_error = E_B590_ERROR_BAD_CRC;
        return last_error;
    }

    emit debug("E_B590_NO_ERROR");
    last_error = E_B590_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x46(uint8_t  *type,
                                    uint16_t *year,
                                    uint8_t  *month,
                                    uint16_t *serno)
{
    emit trace("send_cmd_0x46");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x46_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_46_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x46;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_46_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf, sizeof(B590_CMD_46_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_46_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x46, sizeof(B590_CMD_46_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        busy = false;
        last_error = res;
        return last_error;
    }
    //---

    B590_CMD_46_ANSWER *answer = (B590_CMD_46_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *type = answer->body.data.type;
    *year = answer->body.data.year;
    *month = answer->body.data.month;
    *serno = answer->body.data.serno;

    busy = false;
    last_error = E_B590_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x47(int32_t  *in_voltage,
                                    uint16_t *in_current,
                                    int32_t  *out_voltage,
                                    uint16_t *out_current,
                                    int8_t   *temp,
                                    uint8_t  *active_profile,
                                    bool *flag_zummer)
{
    emit trace("send_cmd_0x47");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x47_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_47_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x47;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_47_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_47_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_47_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x47, sizeof(B590_CMD_47_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_47_ANSWER *answer = (B590_CMD_47_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *in_voltage = answer->body.data.in_voltage;
    *in_current = answer->body.data.in_current;
    *out_voltage = answer->body.data.out_voltage;
    *out_current = answer->body.data.out_current;
    *temp = answer->body.data.temperature;
    *active_profile = answer->body.data.active_profile;
    *flag_zummer = answer->body.data.flag_zummer;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x48(unsigned char new_address_MODBUS,
                                    unsigned char *r_new_address_MODBUS)
{
    emit trace("send_cmd_0x48");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x48_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_48_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x48;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.new_address_modbus = new_address_MODBUS;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_48_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_48_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_48_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x48, sizeof(B590_CMD_48_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_48_ANSWER *answer = (B590_CMD_48_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_new_address_MODBUS = answer->body.data.new_address_modbus;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x49(int32_t  voltage,
                                    uint16_t current)
{
    emit trace("send_cmd_0x49");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x49_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_49_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x49;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.current = current;
    question.body.data.voltage = voltage;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_49_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_49_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_49_ANSWER), 200, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x49, sizeof(B590_CMD_49_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_49_ANSWER *answer = (B590_CMD_49_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    if(answer->body.data.error != 0)
    {
        U_BYTE error_byte;
        error_byte.value = answer->body.data.error;
        last_error = E_B590_NO_ERROR;
        if(error_byte.bites.bit0) last_error = E_B590_LO_VOLTAGE;
        if(error_byte.bites.bit1) last_error = E_B590_HI_VOLTAGE;
        if(error_byte.bites.bit2) last_error = E_B590_LO_CURRENT;
        if(error_byte.bites.bit3) last_error = E_B590_HI_CURRENT;
        if(error_byte.bites.bit4) last_error = E_B590_NO_REVERSE;
        if(error_byte.bites.bit5) last_error = E_B590_MAX_POWER;
        if(error_byte.bites.bit6) last_error = E_B590_CALIBRATION_RUNNING;
        if(error_byte.bites.bit7) last_error = E_B590_NO_RC;
        busy = false;
        return last_error;
    }

#ifdef NEEDED_PAUSE
    pause();
#endif
    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x4A(uint32_t *bits,
                                    uint16_t *code)
{
    emit trace("send_cmd_0x4A");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x4A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_4A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x4A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_4A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_4A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_4A_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x4A, sizeof(B590_CMD_4A_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_4A_ANSWER *answer = (B590_CMD_4A_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *bits = answer->body.data.error_bits;
    *code = answer->body.data.error_code;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x54(unsigned char profile,
                                    unsigned char *count_point,
                                    unsigned char *count_repeat_profile)
{
    emit trace("send_cmd_0x54");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x54_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_54_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x54;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_54_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_54_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_54_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x54, sizeof(B590_CMD_54_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    //---
    if(data_powersupply.length() == sizeof(B590_CMD_54_ANSWER_ERROR))
    {
        B590_CMD_54_ANSWER_ERROR *answer_error = (B590_CMD_54_ANSWER_ERROR *)data_powersupply.data();
        Q_CHECK_PTR(answer_error);

        if(answer_error->body.data.error == 1)
        {
            last_error = E_B590_UNKNOWN_PROFILE;
            busy = false;
            return last_error;
        }
        if(answer_error->body.data.error == 2)
        {
            last_error = E_B590_UNKNOWN_POINT;
            busy = false;
            return last_error;
        }
        if(answer_error->body.data.error == 3)
        {
            last_error = E_B590_PROFILE_EMPTY;
            busy = false;
            return last_error;
        }
    }
    //---

    B590_CMD_54_ANSWER *answer = (B590_CMD_54_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    if(answer->body.data.error == 1)
    {
        last_error = E_B590_NUMBER_PROFILE_ERROR;
        busy = false;
        return last_error;
    }

    *count_point = answer->body.data.count_point;
    *count_repeat_profile = answer->body.data.count_repeat_profile;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x55(unsigned int *mototime_min)
{
    emit trace("send_cmd_0x55");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x55_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B590_CMD_55_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x55;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_55_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_55_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_55_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x55, sizeof(B590_CMD_55_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_55_ANSWER *answer = (B590_CMD_55_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *mototime_min = answer->body.data.mototime_min;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x56(uint8_t  profile,
                                    uint8_t  point,
                                    uint8_t  *r_profile,
                                    uint8_t  *r_point,
                                    uint16_t *r_voltage,
                                    uint16_t *r_current,
                                    uint16_t *r_time)
{
    emit trace("send_cmd_0x56");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x56_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_56_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x56;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.data.point = point;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_56_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_56_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_56_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B590_CMD_0x56, sizeof(B590_CMD_56_ANSWER));
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    if(data_powersupply.length() == sizeof(B590_CMD_56_ANSWER_ERROR))
    {
        B590_CMD_56_ANSWER_ERROR *answer_error = (B590_CMD_56_ANSWER_ERROR *)data_powersupply.data();
        Q_CHECK_PTR(answer_error);

        if(answer_error->body.data.error == 1)
        {
            last_error = E_B590_UNKNOWN_PROFILE;
            busy = false;
            return last_error;
        }
        if(answer_error->body.data.error == 2)
        {
            last_error = E_B590_UNKNOWN_POINT;
            busy = false;
            return last_error;
        }
        if(answer_error->body.data.error == 3)
        {
            last_error = E_B590_PROFILE_EMPTY;
            busy = false;
            return last_error;
        }
    }

    int res = check_packet_answer(B590_CMD_0x56, sizeof(B590_CMD_56_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    B590_CMD_56_ANSWER *answer = (B590_CMD_56_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_profile = answer->body.data.profile;
    *r_point = answer->body.data.point;
    *r_voltage = answer->body.data.voltage;
    *r_current = answer->body.data.current;
    *r_time = answer->body.data.time;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x57(uint16_t U,
                                    uint16_t I)
{
    emit trace("send_cmd_0x57");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x57_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }
    B590_CMD_57_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x57;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.U = U;
    question.body.data.I = I;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_57_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_57_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_57_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B590_CMD_0x57, sizeof(B590_CMD_57_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

#ifdef NEEDED_PAUSE
    pause();
#endif
    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x5A(uint16_t *setting_U,
                                    uint16_t *setting_I,
                                    uint16_t *current_U,
                                    uint16_t *current_I,
                                    unsigned char  *temperature,
                                    unsigned char  *active_profile,
                                    unsigned char  *point)
{
    emit trace("send_cmd_0x5A");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x5A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_5A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x5A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_5A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_5A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_5A_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B590_CMD_0x5A, sizeof(B590_CMD_5A_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    B590_CMD_5A_ANSWER *answer = (B590_CMD_5A_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *setting_U = answer->body.data.setting_DAC_U;
    *setting_I = answer->body.data.setting_DAC_I;
    *current_U = answer->body.data.current_ADC_U;
    *current_I = answer->body.data.current_ADC_I;
    *temperature = answer->body.data.temperature;
    *active_profile = answer->body.data.active_profile;
    *point = answer->body.data.active_point;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x5B(uint32_t *U,
                                    uint32_t *I,
                                    uint32_t *U_input)
{
    emit trace("send_cmd_0x5B");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x5B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_5B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x5B;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_5B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_5B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_5B_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B590_CMD_0x5B, sizeof(B590_CMD_5B_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    B590_CMD_5B_ANSWER *answer = (B590_CMD_5B_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *U = answer->body.data.ADC_U;
    *I = answer->body.data.ADC_I;
    *U_input = answer->body.data.ADC_U_input;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x5C(s_zero_b590_U zero_U,
                                    s_zero_b590_I zero_I,
                                    uint8_t       *r_error)
{
    emit trace("send_cmd_0x5C");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x5C_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_5C_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x5C;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.zero_U = zero_U;
    question.body.data.zero_I = zero_I;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_5C_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_5C_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_5C_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x5C, sizeof(B590_CMD_5C_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_5C_ANSWER *answer = (B590_CMD_5C_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_error  = answer->body.data.error;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x5D(s_zero_b590_U *zero_U,
                                    s_zero_b590_I *zero_I)
{
    emit trace("send_cmd_0x5D");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x5D_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_5D_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x5D;
    question.body.header.count_data = 0;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_5D_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_5D_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_5D_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x5D, sizeof(B590_CMD_5D_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_5D_ANSWER *answer = (B590_CMD_5D_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *zero_U  = answer->body.data.zero_U;
    *zero_I  = answer->body.data.zero_I;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x5E(uint8_t  profile,
                                    uint16_t voltage_mV,
                                    uint16_t current_mA,
                                    uint16_t time,
                                    uint16_t *r_error,
                                    uint8_t  *r_profile,
                                    uint16_t *r_voltage,
                                    uint16_t *r_current,
                                    uint16_t *r_time)
{
    emit trace("send_cmd_0x5E");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x5E_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_5E_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x5E;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.data.voltage = voltage_mV;
    question.body.data.current = current_mA;
    question.body.data.time = time;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_5E_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_5E_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_5E_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x5E, sizeof(B590_CMD_5E_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_5E_ANSWER *answer = (B590_CMD_5E_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    if(answer->body.data.error)
    {
        U_SHORT error_short;
        error_short.value = answer->body.data.error;
        last_error = E_B590_NO_ERROR;
        if(error_short.bites.bit0)  last_error = E_B590_LO_VOLTAGE;
        if(error_short.bites.bit1)  last_error = E_B590_HI_VOLTAGE;
        if(error_short.bites.bit2)  last_error = E_B590_LO_CURRENT;
        if(error_short.bites.bit3)  last_error = E_B590_HI_CURRENT;
        if(error_short.bites.bit4)  last_error = E_B590_NO_REVERSE;
        if(error_short.bites.bit5)  last_error = E_B590_MAX_POWER;
        if(error_short.bites.bit6)  last_error = E_B590_LO_TIME;
        if(error_short.bites.bit7)  last_error = E_B590_HI_TIME;
        if(error_short.bites.bit8)  last_error = E_B590_NUMBER_PROFILE_ERROR;
        if(error_short.bites.bit9)  last_error = E_B590_NUMBER_POINT_ERROR;
        if(error_short.bites.bit11) last_error = E_B590_ERROR_POINT;
        if(error_short.bites.bit12) last_error = E_B590_PROFILE_IS_RUN;
        if(error_short.bites.bit14) last_error = E_B590_CALIBRATION_RUNNING;
        if(error_short.bites.bit15) last_error = E_B590_NO_RC;
        return last_error;
    }

    *r_error = answer->body.data.error;
    *r_profile = answer->body.data.profile;
    *r_voltage = answer->body.data.voltage;
    *r_current = answer->body.data.current;
    *r_time = answer->body.data.time;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x5F(unsigned char profile,
                                    unsigned char *number_profile)
{
    emit trace("send_cmd_0x5F");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x5F_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_5F_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x5F;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.number_profile = profile;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_5F_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_5F_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_5F_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x5F, sizeof(B590_CMD_5F_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_5F_ANSWER *answer = (B590_CMD_5F_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    if(answer->body.data.error)
    {
        last_error = E_B590_NO_ERROR;
        if(answer->body.data.error == 1) last_error = E_B590_NUMBER_POINT_ERROR;
        busy = false;
        return last_error;
    }
    *number_profile = answer->body.data.number_profile;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x60(void)
{
    emit trace("send_cmd_0x60");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x60_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_60_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x60;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_60_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_60_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_60_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x60, sizeof(B590_CMD_60_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x61(int32_t *Value_ADC_Zero_U,
                                    double  *factor_k_DAC,
                                    double  *factor_k_ADC)
{
    emit trace("send_cmd_0x61");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x61_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_61_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x61;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_61_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_61_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_61_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x61, sizeof(B590_CMD_61_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_61_ANSWER *answer = (B590_CMD_61_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *Value_ADC_Zero_U = answer->body.data.Value_ADC_U_0;
    *factor_k_DAC     = answer->body.data.factor_k_DAC;
    *factor_k_ADC     = answer->body.data.factor_k_ADC;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x62(int32_t *Value_ADC_Zero_I,
                                    double  *factor_k_DAC,
                                    double  *factor_k_ADC)
{
    emit trace("send_cmd_0x62");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x62_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_62_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x62;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_62_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_62_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_62_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x62, sizeof(B590_CMD_62_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_62_ANSWER *answer = (B590_CMD_62_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *Value_ADC_Zero_I = answer->body.data.Value_ADC_I_0;
    *factor_k_DAC     = answer->body.data.factor_k_DAC;
    *factor_k_ADC     = answer->body.data.factor_k_ADC;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x63(uint8_t  code,
                                    uint8_t  number_point,
                                    uint16_t data_point,
                                    uint8_t *r_number_point,
                                    uint16_t *r_data_point)
{
    emit trace("send_cmd_0x63");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x63_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_63_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x63;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.type_calibration = code;
    question.body.data.number_point = number_point;
    question.body.data.data_point = data_point;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_63_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_63_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_63_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x63, sizeof(B590_CMD_63_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_63_ANSWER *answer = (B590_CMD_63_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_number_point = answer->body.data.number_point;
    *r_data_point = answer->body.data.data_point;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
/*
 * 1 - запуск калибровки АЦП по напряжению
 * 2 - запуск калибровки АЦП по току
 * 3 - запуск калибровки ЦАП по напряжению
 * 4 - запуск калибровки ЦАП по току
 * 5 - переход к следующей точке (при калибровке АЦП)
 */
int Powersupply_B590::send_cmd_0x64(char code)
{
    emit trace("send_cmd_0x64");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x64_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_64_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x64;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.code = code;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_64_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_64_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_64_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B590_CMD_0x64, sizeof(B590_CMD_64_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    //B590_CMD_64_ANSWER *answer = (B590_CMD_64_ANSWER *)data_powersupply.data();
    //Q_CHECK_PTR(answer);
    //emit info(QString("code %1").arg(answer->body.data.code));

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x65(uint8_t *number_current_point,
                                    uint8_t *state)
{
    emit trace("send_cmd_0x65");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x65_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_65_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x65;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_65_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_65_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_65_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x65, sizeof(B590_CMD_65_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_65_ANSWER *answer = (B590_CMD_65_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *number_current_point = answer->body.data.number_current_point;
    *state = answer->body.data.state;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x66(uint16_t code,
                                    uint16_t index,
                                    uint16_t *value)
{
    emit trace("send_cmd_0x66");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x66_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_66_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x66;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.code = code;
    question.body.data.index_point = index;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_66_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_66_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_66_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x66, sizeof(B590_CMD_66_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_SMALL_ANSWER)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_66_ANSWER *answer = (B590_CMD_66_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *value = answer->body.data.point_value;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x67(unsigned short speed_cooler,
                                    unsigned short *r_speed_cooler)
{
    emit trace("send_cmd_0x67");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x67_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_67_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x67;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.speed_cooler = speed_cooler;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_67_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_67_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_67_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x67, sizeof(B590_CMD_67_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_67_ANSWER *answer = (B590_CMD_67_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_speed_cooler = answer->body.data.speed_cooler;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x68(unsigned short data_PWM,
                                    unsigned short *r_data_PWM)
{
    emit trace("send_cmd_0x68");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x68_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_68_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x68;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.data_PWM = data_PWM;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_68_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_68_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_68_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x68, sizeof(B590_CMD_68_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        emit error(QString("Unknown command [%1]").arg(data_powersupply.toHex().data()));
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        emit error(QString("Bad command [%1]").arg(data_powersupply.toHex().data()));
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_68_ANSWER *answer = (B590_CMD_68_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_data_PWM = answer->body.data.data_PWM;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x69(unsigned short *OCR3A,
                                    unsigned short *OCR3B)
{
    emit trace("send_cmd_0x69");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x69_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_69_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x69;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_69_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_69_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_69_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x69, sizeof(B590_CMD_69_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_69_ANSWER *answer = (B590_CMD_69_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *OCR3A = answer->body.data.OCR3A;
    *OCR3B = answer->body.data.OCR3B;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x6A(void)
{
    emit trace("send_cmd_0x6A");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x6A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_6A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x6A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_6A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_6A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_6A_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x6A, sizeof(B590_CMD_6A_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_6A_ANSWER *answer = (B590_CMD_6A_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    U_BYTE data_byte;
    data_byte.value = answer->body.data.current_driver;
    if(data_byte.bites.bit0) emit info(tr("Захват произведен."));
    if(data_byte.bites.bit1) emit error(tr("Захват невозможен, т.к. ручное управление."));
    if(data_byte.bites.bit2) emit error(tr("Захват невозможен, т.к. RS232 сейчас используется."));
    if(data_byte.bites.bit4) emit error(tr("Захват невозможен, т.к. USB сейчас используется."));

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x6B(void)
{
    emit trace("send_cmd_0x6B");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x6B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_6B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x6B;
    question.body.header.count_data = 0;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_6B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_6B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_6B_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x6B, sizeof(B590_CMD_6B_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_6B_ANSWER *answer = (B590_CMD_6B_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    U_BYTE data_byte;
    data_byte.value = answer->body.data.current_driver;
    if(data_byte.bites.bit0) emit info(tr("Снятие произведено."));
    if(data_byte.bites.bit1) emit error(tr("Снятие невозможено, т.к. ручное управление."));
    if(data_byte.bites.bit2) emit error(tr("Снятие невозможено, т.к. RS232 сейчас используется."));
    if(data_byte.bites.bit4) emit error(tr("Снятие невозможено, т.к. USB сейчас используется."));

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x6C(unsigned int number_string)
{
    emit trace("send_cmd_0x6C");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x6C_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    if(number_string > 2)
    {
        emit error(QString(tr("Недопустимый номер строки %1")).arg(number_string));
        last_error = E_B590_NUMBER_STRING_ERROR;
        busy = false;
        return last_error;
    }

    B590_CMD_6C_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x6C;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_6C_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_6C_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_6C_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x6C, sizeof(B590_CMD_6C_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

#if 0
    QString temp_data;
    temp_data.clear();

    B590_CMD_6C_ANSWER_DATA11 *answer11 = (B590_CMD_6C_ANSWER_DATA11 *)data_powersupply.data();
    B590_CMD_6C_ANSWER_DATA8  *answer8 = (B590_CMD_6C_ANSWER_DATA8 *)data_powersupply.data();
    switch(number_string)
    {
    case 0:
        if(data_powersupply.length() != sizeof(B590_CMD_6C_ANSWER_DATA11))
        {
            last_error = E_B590_ERROR_BIG_ANSWER;
            busy = false;
            return last_error;
        }

        for(unsigned int n=0; n<sizeof(answer11->body.data); n++)
            temp_data.append(answer11->body.data.data[n]);
        emit info(QString("%1").arg(temp_data));
        break;

    case 1:
        if(data_powersupply.length() != sizeof(B590_CMD_6C_ANSWER_DATA8))
        {
            last_error = E_B590_ERROR_BIG_ANSWER;
            busy = false;
            return last_error;
        }

        for(unsigned int n=0; n<sizeof(answer8->body.data); n++)
            temp_data.append(answer8->body.data.data[n]);
        emit info(QString("%1").arg(temp_data));
        break;

    case 2:
        if(data_powersupply.length() != sizeof(B590_CMD_6C_ANSWER_DATA8))
        {
            last_error =  E_B590_ERROR_BIG_ANSWER;
            busy = false;
            return last_error;
        }

        B590_CMD_6C_ANSWER_DATA8 *answer = (B590_CMD_6C_ANSWER_DATA8 *)data_powersupply.data();
        Q_CHECK_PTR(answer);
        for(unsigned int n=0; n<sizeof(answer8->body.data); n++)
            temp_data.append(answer->body.data.data[n]);

        /*
        8 байт: версия компилятора (2 байта),
                текущая версия программы (2 байта),
                количество перезаписей программы (4 байта)
        */
        U_SHORT_BYTES temp_version_comp;
        U_SHORT_BYTES temp_version_proc;
        INT_BYTES temp_sum_build_proc;

        temp_version_comp.bytes.byte_0 = answer->body.data.data[0];
        temp_version_comp.bytes.byte_1 = answer->body.data.data[1];

        temp_version_proc.bytes.byte_0 = answer->body.data.data[2];
        temp_version_proc.bytes.byte_1 = answer->body.data.data[3];

        temp_sum_build_proc.bytes.byte_0 = answer->body.data.data[4];
        temp_sum_build_proc.bytes.byte_1 = answer->body.data.data[5];
        temp_sum_build_proc.bytes.byte_2 = answer->body.data.data[6];
        temp_sum_build_proc.bytes.byte_3 = answer->body.data.data[7];

        unsigned short version_comp = temp_version_comp.value;
        unsigned short version_proc = temp_version_proc.value;
        unsigned int sum_build_proc = temp_sum_build_proc.value;
        emit info(QString("version_comp %1").arg(version_comp));
        emit info(QString("version_proc %1").arg(version_proc));
        emit info(QString("sum_build_proc %1").arg(sum_build_proc));
        break;
    }
#endif

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x6D(uint8_t secret_cmd,
                                    uint8_t input_data[],
                                    uint8_t *r_secret_cmd,
                                    uint8_t *result,
                                    QByteArray *output_data)
{
    emit trace("send_cmd_0x6D");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x6D_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_6D_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x6D;
    question.body.header.count_data = sizeof(question.body.data);

    question.body.data.secret_cmd = secret_cmd;
    question.body.data.signature.byte_0 = 0xAA;
    question.body.data.signature.byte_1 = 0xBB;
    question.body.data.signature.byte_2 = 0xCC;
    question.body.data.signature.byte_3 = 0xDD;
    question.body.data.signature.byte_4 = 0xEE;
    question.body.data.signature.byte_5 = 0x44;
    for(int n=0; n<128; n++)
    {
        question.body.data.secret_data[n] = input_data[n];
    }
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_6D_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_6D_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_6D_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x6D, sizeof(B590_CMD_6D_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_6D_ANSWER *answer = (B590_CMD_6D_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_secret_cmd = answer->body.data.secret_cmd;
    *result = answer->body.data.result;
    for(int n=0; n<128; n++)
    {
        (*output_data).append(answer->body.data.secret_data[n]);
    }

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x6E(unsigned char profile,
                                    unsigned char repeat,
                                    unsigned char *r_profile,
                                    unsigned char *r_repeat)
{
    emit trace("send_cmd_0x6E");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x6E_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_6E_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x6E;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.data.repeat = repeat;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_6E_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_6E_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_6E_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x6E, sizeof(B590_CMD_6E_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_6E_ANSWER *answer = (B590_CMD_6E_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    if(answer->body.data.error)
    {
        U_BYTE error_byte;
        error_byte.value = answer->body.data.error;
        last_error = E_B590_NO_ERROR;
        if(error_byte.bites.bit0) last_error = E_B590_NUMBER_PROFILE_ERROR;
        if(error_byte.bites.bit1) last_error = E_B590_NUMBER_REPEAT_ERROR;
        if(error_byte.bites.bit4) last_error = E_B590_PROFILE_IS_RUN;
        if(error_byte.bites.bit7) last_error = E_B590_NO_RC;
        busy = false;
        return last_error;
    }
    *r_profile = answer->body.data.profile;
    *r_repeat  = answer->body.data.repeat;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x6F(unsigned char profile,
                                    unsigned char *r_profile)
{
    emit trace("send_cmd_0x6F");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x6F_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_6F_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x6F;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_6F_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_6F_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_6F_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x6F, sizeof(B590_CMD_6F_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_6F_ANSWER *answer = (B590_CMD_6F_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    if(answer->body.data.error)
    {
        U_BYTE error_byte;
        error_byte.value = answer->body.data.error;
        last_error = E_B590_NO_ERROR;
        if(error_byte.bites.bit0) last_error = E_B590_NUMBER_PROFILE_ERROR;
        if(error_byte.bites.bit4) last_error = E_B590_PROFILE_IS_RUN;
        if(error_byte.bites.bit7) last_error = E_B590_NO_RC;
        busy = false;
        return last_error;
    }
    *r_profile = answer->body.data.profile;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x71(unsigned char *state_ADC_U,
                                    unsigned char *state_ADC_I,
                                    unsigned char *state_ADC_Ui)
{
    emit trace("send_cmd_0x71");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x71_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_71_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x71;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_71_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_71_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_71_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x71, sizeof(B590_CMD_71_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_71_ANSWER *answer = (B590_CMD_71_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *state_ADC_U = answer->body.data.state_ADC_U;
    *state_ADC_I = answer->body.data.state_ADC_I;
    *state_ADC_Ui = answer->body.data.state_ADC_Ui;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x72(unsigned short conf_ADC,
                                    unsigned short *r_conf_ADC)
{
    emit trace("send_cmd_0x72");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x72_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_72_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x72;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.conf_ADC = conf_ADC;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_72_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_72_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_72_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x72, sizeof(B590_CMD_72_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_72_ANSWER *answer = (B590_CMD_72_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *r_conf_ADC = answer->body.data.conf_ADC;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x73(unsigned short *real_DAC_U,
                                    unsigned short *real_DAC_I,
                                    unsigned char  *revers,
                                    unsigned short *temperature)
{
    emit trace("send_cmd_0x73");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x73_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_73_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x73;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_73_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_73_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_73_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x73, sizeof(B590_CMD_73_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B590_CMD_73_ANSWER *answer = (B590_CMD_73_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *real_DAC_U = answer->body.data.real_DAC_U;
    *real_DAC_I = answer->body.data.real_DAC_I;
    *revers = answer->body.data.revers;
    *temperature = answer->body.data.temperature;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::send_cmd_0x75(unsigned char *count_ReStart_ADC_1,
                                    unsigned char *count_ReStart_ADC_0,
                                    unsigned char *count_ReStart_ADC_2,
                                    unsigned short *count_Error_Answer_ADC,
                                    unsigned int   *count_Error_ADC,
                                    unsigned short *MCUCSR)
{
    emit trace("send_cmd_0x75");
    if(busy)
    {
        last_error = E_B590_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B590_CMD_0x75_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B590_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B590_CMD_75_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B590_CMD_0x75;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B590_CMD_75_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial_send((const char *)question.buf,sizeof(B590_CMD_75_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B590_CMD_75_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B590_CMD_0x75, sizeof(B590_CMD_75_ANSWER));
    if(res == E_B590_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B590_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B590_ERROR_BAD_COMMAND)
    {
        last_error = E_B590_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B590_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---
    B590_CMD_75_ANSWER *answer = (B590_CMD_75_ANSWER *)data_powersupply.data();
    Q_CHECK_PTR(answer);

    *count_ReStart_ADC_1 = answer->body.data.count_ReStart_ADC_1;
    *count_ReStart_ADC_0 = answer->body.data.count_ReStart_ADC_0;
    *count_ReStart_ADC_2 = answer->body.data.count_ReStart_ADC_2;
    *count_Error_Answer_ADC = answer->body.data.count_Error_Answer_ADC;
    *count_Error_ADC = answer->body.data.count_Error_ADC;
    *MCUCSR = answer->body.data.MCUCSR;

    last_error = E_B590_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::rc_on(void)
{
    int err = send_cmd_0x6A();
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::rc_off(void)
{
    int err = send_cmd_0x6B();
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Powersupply_B590::set_address(int new_address)
{
    emit debug(QString("new_address: %1").arg(new_address));
    address = new_address;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::get_address(void)
{
    return address;
}
//--------------------------------------------------------------------------------
void Powersupply_B590::close_connect(void)
{
    if(serial.isOpen()) serial.close();
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_temperature(unsigned char *temp,
                                       bool *flag_zummer)
{
    int32_t  in_voltage = 0;
    uint16_t in_current = 0;
    int32_t  out_voltage = 0;
    uint16_t out_current = 0;
    int8_t   powersupply_temp = 0;
    uint8_t  active_profile = 0;
    bool zummer = false;

    set_ignore_bad_cmd(true);
    int err = send_cmd_0x47(&in_voltage,
                            &in_current,
                            &out_voltage,
                            &out_current,
                            &powersupply_temp,
                            &active_profile,
                            &zummer);
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    *temp = powersupply_temp;
    *flag_zummer = zummer;
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_accurate_temperature(double *temp)
{
    unsigned short real_DAC_U = 0;
    unsigned short real_DAC_I = 0;
    unsigned char  revers = 0;
    unsigned short temperature = 0;

    set_ignore_bad_cmd(true);
    int err = send_cmd_0x73(&real_DAC_U,
                            &real_DAC_I,
                            &revers,
                            &temperature);
    if(err != E_B590_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    *temp = (float)((float)temperature / (float)16.0f - 200.0f) / (float)4.0f;
    return true;
}
//--------------------------------------------------------------------------------
void Powersupply_B590::set_state_silence(bool state)
{
    emit trace(QString("set_state_silence(%1)").arg(state ? "true" : "false"));
    is_silence = state;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::set_UI(int32_t  voltage_mV,
                              uint16_t current_mA)
{
    int err = send_cmd_0x49(voltage_mV,
                            current_mA);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::set_UI_parrot(uint16_t voltage,
                                     uint16_t current)
{
    int err = send_cmd_0x57(voltage,
                            current);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::set_vent_speed(unsigned short speed)
{
    unsigned short temp = 0;
    int err = send_cmd_0x67(speed, &temp);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_UI(int32_t  *in_voltage,
                              uint16_t *in_current,
                              int32_t  *out_voltage,
                              uint16_t *out_current,
                              int8_t   *temp,
                              uint8_t  *active_profile)
{
    emit info(B590_CMD_0x47_TEXT);
    bool flag_zummer = false;
    int err = send_cmd_0x47(in_voltage,
                            in_current,
                            out_voltage,
                            out_current,
                            temp,
                            active_profile,
                            &flag_zummer);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_ADC(uint32_t *U_parrot,
                               uint32_t *I_parrot,
                               uint32_t *U_input_parrot)
{
    int err = send_cmd_0x5B(U_parrot,
                            I_parrot,
                            U_input_parrot);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::run_calibration(char code)
{
    int err = send_cmd_0x64(code);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_calibration_state(uint8_t *state)
{
    uint8_t point = 0;
    int err = send_cmd_0x65(&point, state);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_current_calibration_point(uint8_t *point,
                                                     uint8_t *state)
{
    int err = send_cmd_0x65(point, state);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_calibration_point_U(uint8_t number_point,
                                               uint16_t *value)
{
    int err = send_cmd_0x66(0,
                            number_point,
                            value);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_calibration_point_I(uint8_t number_point,
                                               uint16_t *value)
{
    int err = send_cmd_0x66(1,
                            number_point,
                            value);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::set_calibration_point_U(uint8_t number_point,
                                               uint16_t value)
{
    unsigned char  r_number_point = 0;
    unsigned short r_value = 0;
    int err = send_cmd_0x63(0,
                            number_point,
                            value,
                            &r_number_point,
                            &r_value);
    if(err != E_B590_NO_ERROR) return false;
    if(r_number_point != number_point) return false;
    if(r_value != value) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::set_calibration_point_I(uint8_t number_point,
                                               uint16_t value)
{
    unsigned char  r_number_point = 0;
    unsigned short r_value = 0;
    int err = send_cmd_0x63(1,
                            number_point,
                            value,
                            &r_number_point,
                            &r_value);
    if(err != E_B590_NO_ERROR) return false;
    if(r_number_point != number_point) return false;
    if(r_value != value) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_information(uint8_t  *type,
                                       uint16_t *year,
                                       uint8_t  *month,
                                       uint16_t *serno)
{
    int err = send_cmd_0x46(type,
                            year,
                            month,
                            serno);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_calibration_zero(s_zero_b590_U *zero_U,
                                            s_zero_b590_I *zero_I)
{
    int err = send_cmd_0x5D(zero_U,
                            zero_I);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Powersupply_B590::wait(int time_msec)
{
#ifdef FAKE
    Q_UNUSED(time_msec)
#else
    QTime time;

    //emit debug(QString("wait: time_msec = %1").arg(time_msec));

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > time_msec)
            break;
    }
#endif
}
//--------------------------------------------------------------------------------
void Powersupply_B590::wait(int max_len,
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

        if(data_powersupply.size() >= max_len)
            break;
    }
#endif
}
//--------------------------------------------------------------------------------
void Powersupply_B590::msgError(const QString &message)
{
    QMessageBox::critical(0, tr("Ошибка"), message);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Set_DAC_U_I(signed int U,signed int I)
{
    //0x57
    int err = send_cmd_0x57(U, I);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Set_Auto_Send_Power_Unit_Get_real_U(void)
{
    uint16_t setting_U = 0;
    uint16_t setting_I = 0;
    uint16_t current_U = 0;
    uint16_t   current_I = 0;
    unsigned char  temperature = 0;
    unsigned char  active_profile = 0;
    unsigned char  point = 0;
    int err = send_cmd_0x5A(&setting_U,
                            &setting_I,
                            &current_U,
                            &current_I,
                            &temperature,
                            &active_profile,
                            &point);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Set_Auto_Send_Power_Unit_Get_ADC_U(void)
{
    uint32_t U = 0;
    uint32_t I = 0;
    uint32_t U_input = 0;
    int err = send_cmd_0x5B(&U,
                            &I,
                            &U_input);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Save_Calib_Point_DAC(unsigned char number_point,
                                            unsigned short data_point)
{
    unsigned char  r_number_point = 0;
    unsigned short r_data_point = 0;
    int err = send_cmd_0x63(0,
                            number_point,
                            data_point,
                            &r_number_point,
                            &r_data_point);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Send_Command_To_Calib_ADC_U(void)
{
    char code = TASK_CALIBRATION_ADC_U; //Запуск калибровки АЦП по напряжению
    int err = send_cmd_0x64(code);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Send_Command_To_Calib_ADC_I(void)
{
    char code = TASK_CALIBRATION_ADC_I; //Запуск калибровки АЦП по току
    int err = send_cmd_0x64(code);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Send_Command_To_Calib_DAC_U(void)
{
    char code = TASK_CALIBRATION_DAC_U; //Запуск калибровки ЦАП по напряжению
    int err = send_cmd_0x64(code);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Send_Command_To_Calib_DAC_I(void)
{
    char code = TASK_CALIBRATION_DAC_I; //Запуск калибровки ЦАП по току
    int err = send_cmd_0x64(code);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Send_Command_To_Next_Point(void)
{
    char code = 5; //Переход к следующей точке - при калибровке АЦП
    int err = send_cmd_0x64(code);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Test_Calib_Point_DAC(unsigned char number_point)
{
    unsigned short value = 0;
    int err = send_cmd_0x66(0, number_point, &value);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Set_Auto_Send_Power_Unit_Get_Setting_DAC_U(void)
{
    unsigned short real_DAC_U = 0;
    unsigned short real_DAC_I = 0;
    unsigned char  revers = 0;
    unsigned short temperature = 0;
    int err = send_cmd_0x73(&real_DAC_U,
                            &real_DAC_I,
                            &revers,
                            &temperature);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::Set_Ventilator_MAX_Speed(void)
{
    unsigned short speed_cooler = 0;
    unsigned short r_speed_cooler = 0;
    int err = send_cmd_0x67(speed_cooler,
                            &r_speed_cooler);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog(int index)
{
    switch(index)
    {
    case B590_CMD_0x11: run_dialog_0x11(); break;
    case B590_CMD_0x12: run_dialog_0x12(); break;
    case B590_CMD_0x15: run_dialog_0x15(); break;
    case B590_CMD_0x16: run_dialog_0x16(); break;
    case B590_CMD_0x17: run_dialog_0x17(); break;

    case B590_CMD_0x41: run_dialog_0x41(); break;
    case B590_CMD_0x42: run_dialog_0x42(); break;
    case B590_CMD_0x46: run_dialog_0x46(); break;
    case B590_CMD_0x47: run_dialog_0x47(); break;
    case B590_CMD_0x48: run_dialog_0x48(); break;
    case B590_CMD_0x49: run_dialog_0x49(); break;
    case B590_CMD_0x4A: run_dialog_0x4A(); break;

    case B590_CMD_0x54: run_dialog_0x54(); break;
    case B590_CMD_0x55: run_dialog_0x55(); break;
    case B590_CMD_0x56: run_dialog_0x56(); break;
    case B590_CMD_0x57: run_dialog_0x57(); break;
    case B590_CMD_0x5A: run_dialog_0x5A(); break;
    case B590_CMD_0x5B: run_dialog_0x5B(); break;
    case B590_CMD_0x5C: run_dialog_0x5C(); break;
    case B590_CMD_0x5D: run_dialog_0x5D(); break;
    case B590_CMD_0x5E: run_dialog_0x5E(); break;
    case B590_CMD_0x5F: run_dialog_0x5F(); break;

    case B590_CMD_0x60: run_dialog_0x60(); break;
    case B590_CMD_0x61: run_dialog_0x61(); break;
    case B590_CMD_0x62: run_dialog_0x62(); break;
    case B590_CMD_0x63: run_dialog_0x63(); break;
    case B590_CMD_0x64: run_dialog_0x64(); break;
    case B590_CMD_0x65: run_dialog_0x65(); break;
    case B590_CMD_0x66: run_dialog_0x66(); break;
    case B590_CMD_0x67: run_dialog_0x67(); break;
    case B590_CMD_0x68: run_dialog_0x68(); break;
    case B590_CMD_0x69: run_dialog_0x69(); break;
    case B590_CMD_0x6A: run_dialog_0x6A(); break;
    case B590_CMD_0x6B: run_dialog_0x6B(); break;
    case B590_CMD_0x6C: run_dialog_0x6C(); break;
    case B590_CMD_0x6D: run_dialog_0x6D(); break;
    case B590_CMD_0x6E: run_dialog_0x6E(); break;
    case B590_CMD_0x6F: run_dialog_0x6F(); break;

    case B590_CMD_0x71: run_dialog_0x71(); break;
    case B590_CMD_0x72: run_dialog_0x72(); break;
    case B590_CMD_0x73: run_dialog_0x73(); break;
    case B590_CMD_0x75: run_dialog_0x75(); break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x11(void)
{
    int err = send_cmd_0x11();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x12(void)
{
    int err = send_cmd_0x12();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x15(void)
{
    int err = send_cmd_0x15();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x16(void)
{
    int err = send_cmd_0x16();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x17(void)
{
    int err = send_cmd_0x17();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x41(void)
{
    int err = send_cmd_0x41();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x42(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x42_TEXT);

    QComboBox *cb_speed = new QComboBox();
    cb_speed->addItem("4800");
    cb_speed->addItem("9600");
    cb_speed->addItem("19200");
    cb_speed->addItem("28800");
    cb_speed->addItem("38400");
    cb_speed->addItem("57600");
    cb_speed->addItem("76800");
    cb_speed->addItem("115200");

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Скорость")), 0, 0);
    grid->addWidget(cb_speed,                   0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        int err = send_cmd_0x42(cb_speed->currentIndex());
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x46(void)
{
    uint8_t  type  = 0;
    uint16_t year = 0;
    uint8_t  month = 0;
    uint16_t serno = 0;
    int err = send_cmd_0x46(&type, &year, &month, &serno);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("type %1").arg(type));
    emit info(QString("year %1").arg(year));
    emit info(QString("month %1").arg(month));
    emit info(QString("serno %1").arg(serno));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x47(void)
{
    int32_t  in_voltage = 0;
    uint16_t in_current = 0;
    int32_t  out_voltage = 0;
    uint16_t out_current = 0;
    int8_t   temp = 0;
    uint8_t  active_profile = 0;
    bool flag_zummer = false;

    int err = send_cmd_0x47(&in_voltage,
                            &in_current,
                            &out_voltage,
                            &out_current,
                            &temp,
                            &active_profile,
                            &flag_zummer);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString(tr("in_voltage: %1")).arg(in_voltage));
    emit info(QString(tr("in_current: %1")).arg(in_current));
    emit info(QString(tr("out_voltage: %1")).arg(out_voltage));
    emit info(QString(tr("out_current: %1")).arg(out_current));
    emit info(QString(tr("temp: %1")).arg(temp));
    emit info(QString(tr("active profile: %1")).arg(active_profile));
    emit info(QString(tr("flag_zummer: %1")).arg(flag_zummer));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x48(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x48_TEXT);

    QSpinBox *sb_address = new QSpinBox();
    sb_address->setMinimum(MIN_ADDRESS_B590);
    sb_address->setMaximum(MAX_ADDRESS_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Новый адрес MODBUS")),   0, 0);
    grid->addWidget(sb_address,                             0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned char r_new_address_MODBUS = 0;
        int err = send_cmd_0x48(sb_address->value(),
                                &r_new_address_MODBUS);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_new_address_MODBUS %1").arg(r_new_address_MODBUS));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x49(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x49_TEXT);

    QSpinBox *sb_voltage = new QSpinBox();
    sb_voltage->setMinimum(MIN_VOLTAGE_B590);
    sb_voltage->setMaximum(MAX_VOLTAGE_B590);

    QSpinBox *sb_current = new QSpinBox();
    sb_current->setMinimum(MIN_CURRENT_B590);
    sb_current->setMaximum(MAX_CURRENT_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Напряжение")),   0, 0);
    grid->addWidget(sb_voltage,                     0, 1);
    grid->addWidget(new QLabel(tr("Ток")),          1, 0);
    grid->addWidget(sb_current,                     1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        int err = send_cmd_0x49(sb_voltage->value(),
                                sb_current->value());
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x4A(void)
{
    uint32_t bits = 0;
    uint16_t code = 0;

    int err = send_cmd_0x4A(&bits, &code);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    U_INT error_int;
    error_int.value = bits;
    if(error_int.bites.bit0)  emit error(ERROR_E00_B590_TEXT);
    if(error_int.bites.bit1)  emit error(ERROR_E01_B590_TEXT);
    if(error_int.bites.bit2)  emit error(ERROR_E02_B590_TEXT);
    if(error_int.bites.bit3)  emit error(ERROR_E03_B590_TEXT);
    if(error_int.bites.bit4)  emit error(ERROR_E04_B590_TEXT);
    if(error_int.bites.bit5)  emit error(ERROR_E05_B590_TEXT);
    if(error_int.bites.bit6)  emit error(ERROR_E06_B590_TEXT);
    if(error_int.bites.bit7)  emit error(ERROR_E07_B590_TEXT);
    if(error_int.bites.bit8)  emit error(ERROR_E08_B590_TEXT);
    if(error_int.bites.bit9)  emit error(ERROR_E09_B590_TEXT);
    if(error_int.bites.bit10) emit error(ERROR_E10_B590_TEXT);
    if(error_int.bites.bit11) emit error(ERROR_E11_B590_TEXT);
    if(error_int.bites.bit12) emit error(ERROR_E12_B590_TEXT);
    if(error_int.bites.bit13) emit error(ERROR_E13_B590_TEXT);
    if(error_int.bites.bit14) emit error(ERROR_E14_B590_TEXT);
    if(error_int.bites.bit15) emit error(ERROR_E15_B590_TEXT);
    if(error_int.bites.bit16) emit error(ERROR_E16_B590_TEXT);
    if(error_int.bites.bit17) emit error(ERROR_E17_B590_TEXT);
    if(error_int.bites.bit18) emit error(ERROR_E18_B590_TEXT);
    if(error_int.bites.bit19) emit error(ERROR_E19_B590_TEXT);
    if(error_int.bites.bit20) emit error(ERROR_E20_B590_TEXT);
    if(error_int.bites.bit21) emit error(ERROR_E21_B590_TEXT);
    if(error_int.bites.bit24) emit error(ERROR_E24_B590_TEXT);
    if(error_int.bites.bit25) emit error(ERROR_E25_B590_TEXT);
    if(error_int.bites.bit26) emit error(ERROR_E26_B590_TEXT);
    if(error_int.bites.bit27) emit error(ERROR_E27_B590_TEXT);
    if(error_int.bites.bit28) emit error(ERROR_E28_B590_TEXT);
    if(error_int.bites.bit29) emit error(ERROR_E29_B590_TEXT);
    if(error_int.bites.bit30) emit error(ERROR_E30_B590_TEXT);
    if(error_int.bites.bit31) emit error(ERROR_E31_B590_TEXT);

    if(code)
    {
        emit error(QString("Error code: %1").arg(code));
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x54(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x54_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B590);
    sb_profile->setMaximum(MAX_PROFILE_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Профиль")),   0, 0);
    grid->addWidget(sb_profile,                  0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned char count_point = 0;
        unsigned char count_repeat_profile = 0;
        int err = send_cmd_0x54(sb_profile->value(),
                                &count_point,
                                &count_repeat_profile);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("count_point %1").arg(count_point));
        emit info(QString("count_repeat_profile %1").arg(count_repeat_profile));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x55(void)
{
    unsigned int mototime_min = 0;
    int err = send_cmd_0x55(&mototime_min);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("mototime_min %1").arg(mototime_min));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x56(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x56_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B590);
    sb_profile->setMaximum(MAX_PROFILE_B590);

    QSpinBox *sb_point = new QSpinBox();
    sb_point->setMinimum(MIN_POINT_B590);
    sb_point->setMaximum(MAX_POINT_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Профиль")),   0, 0);
    grid->addWidget(sb_profile,                  0, 1);
    grid->addWidget(new QLabel(tr("Точка")),     1, 0);
    grid->addWidget(sb_point,                    1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        uint8_t  r_profile = 0;
        uint8_t  r_point = 0;
        uint16_t r_voltage = 0;
        uint16_t r_current = 0;
        uint16_t r_time = 0;
        int err = send_cmd_0x56(sb_profile->value(),
                                sb_point->value(),
                                &r_profile,
                                &r_point,
                                &r_voltage,
                                &r_current,
                                &r_time);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_profile %1").arg(r_profile));
        emit info(QString("r_point %1").arg(r_point));
        emit info(QString("r_voltage %1").arg(r_voltage));
        emit info(QString("r_current %1").arg(r_current));
        emit info(QString("r_time %1").arg(r_time));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x57(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x57_TEXT);

    QSpinBox *sb_U = new QSpinBox();
    sb_U->setMinimum(0);
    sb_U->setMaximum(0xFFFF);

    QSpinBox *sb_I = new QSpinBox();
    sb_I->setMinimum(0);
    sb_I->setMaximum(0xFFFF);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Напряжение (0-65535)")), 0, 0);
    grid->addWidget(sb_U,                                   0, 1);
    grid->addWidget(new QLabel(tr("Ток (0-65535)")),        1, 0);
    grid->addWidget(sb_I,                                   1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        int err = send_cmd_0x57(sb_U->value(),
                                sb_I->value());
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x5A(void)
{
    uint16_t setting_U = 0;
    uint16_t setting_I = 0;
    uint16_t current_U = 0;
    uint16_t current_I = 0;
    unsigned char   temperature = 0;
    unsigned char   active_profile = 0;
    unsigned char   point = 0;

    int err = send_cmd_0x5A(&setting_U,
                            &setting_I,
                            &current_U,
                            &current_I,
                            &temperature,
                            &active_profile,
                            &point);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("setting U %1").arg(setting_U));
    emit info(QString("setting I %1").arg(setting_I));
    emit info(QString("current U %1").arg(current_U));
    emit info(QString("current I %1").arg(current_I));
    emit info(QString("temperature %1").arg(temperature));
    emit info(QString("active_profile %1").arg(active_profile));
    emit info(QString("point %1").arg(point));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x5B(void)
{
    uint32_t U = 0;
    uint32_t I = 0;
    uint32_t U_input = 0;
    int err = send_cmd_0x5B(&U,
                            &I,
                            &U_input);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("U %1").arg(U));
    emit info(QString("I %1").arg(I));
    emit info(QString("Ui %1").arg(U_input));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x5C(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x5C_TEXT);

    s_zero_b590_U zero_U;
    s_zero_b590_I zero_I;

    QSpinBox *sb_Value_ADC_U_0 = new QSpinBox();
    QSpinBox *sb_Value_ADC_U_1 = new QSpinBox();
    QSpinBox *sb_Value_ADC_U_2 = new QSpinBox();
    QSpinBox *sb_Value_ADC_U_3 = new QSpinBox();

    QSpinBox *sb_Value_DAC_U_0 = new QSpinBox();
    QSpinBox *sb_Value_DAC_U_1 = new QSpinBox();
    QSpinBox *sb_Value_DAC_U_2 = new QSpinBox();
    QSpinBox *sb_Value_DAC_U_3 = new QSpinBox();

    QSpinBox *sb_parrot_in_point = new QSpinBox();

    QSpinBox *sb_Value_ADC_I_0 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_1 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_2 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_3 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_4 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_5 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_6 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_7 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_8 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_9 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_10 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_11 = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_11_HOT = new QSpinBox();
    QSpinBox *sb_Value_ADC_I_12 = new QSpinBox();

    QSpinBox *sb_Value_DAC_I_0 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_1 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_2 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_3 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_4 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_5 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_6 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_7 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_8 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_9 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_10 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_11 = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_11_HOT = new QSpinBox();
    QSpinBox *sb_Value_DAC_I_12 = new QSpinBox();

    //---
    sb_Value_ADC_U_0->setMinimum(0);    sb_Value_ADC_U_0->setMaximum(INT_MAX);
    sb_Value_ADC_U_1->setMinimum(0);    sb_Value_ADC_U_1->setMaximum(INT_MAX);
    sb_Value_ADC_U_2->setMinimum(0);    sb_Value_ADC_U_2->setMaximum(INT_MAX);
    sb_Value_ADC_U_3->setMinimum(0);    sb_Value_ADC_U_3->setMaximum(INT_MAX);

    sb_Value_DAC_U_0->setMinimum(0);    sb_Value_DAC_U_0->setMaximum(0xFFFF);
    sb_Value_DAC_U_1->setMinimum(0);    sb_Value_DAC_U_1->setMaximum(0xFFFF);
    sb_Value_DAC_U_2->setMinimum(0);    sb_Value_DAC_U_2->setMaximum(0xFFFF);
    sb_Value_DAC_U_3->setMinimum(0);    sb_Value_DAC_U_3->setMaximum(0xFFFF);

    sb_parrot_in_point->setMinimum(1);  sb_parrot_in_point->setMaximum(0xFFFF);

    sb_Value_ADC_I_0->setMinimum(0);    sb_Value_ADC_I_0->setMaximum(INT_MAX);
    sb_Value_ADC_I_1->setMinimum(0);    sb_Value_ADC_I_1->setMaximum(INT_MAX);
    sb_Value_ADC_I_2->setMinimum(0);    sb_Value_ADC_I_2->setMaximum(INT_MAX);
    sb_Value_ADC_I_3->setMinimum(0);    sb_Value_ADC_I_3->setMaximum(INT_MAX);
    sb_Value_ADC_I_4->setMinimum(0);    sb_Value_ADC_I_4->setMaximum(INT_MAX);
    sb_Value_ADC_I_5->setMinimum(0);    sb_Value_ADC_I_5->setMaximum(INT_MAX);
    sb_Value_ADC_I_6->setMinimum(0);    sb_Value_ADC_I_6->setMaximum(INT_MAX);
    sb_Value_ADC_I_7->setMinimum(0);    sb_Value_ADC_I_7->setMaximum(INT_MAX);
    sb_Value_ADC_I_8->setMinimum(0);    sb_Value_ADC_I_8->setMaximum(INT_MAX);
    sb_Value_ADC_I_9->setMinimum(0);    sb_Value_ADC_I_9->setMaximum(INT_MAX);
    sb_Value_ADC_I_10->setMinimum(0);    sb_Value_ADC_I_10->setMaximum(INT_MAX);
    sb_Value_ADC_I_11->setMinimum(0);    sb_Value_ADC_I_11->setMaximum(INT_MAX);
    sb_Value_ADC_I_11_HOT->setMinimum(0);    sb_Value_ADC_I_11_HOT->setMaximum(INT_MAX);
    sb_Value_ADC_I_12->setMinimum(0);    sb_Value_ADC_I_12->setMaximum(INT_MAX);

    sb_Value_DAC_I_0->setMinimum(0);    sb_Value_DAC_I_0->setMaximum(0xFFFF);
    sb_Value_DAC_I_1->setMinimum(0);    sb_Value_DAC_I_1->setMaximum(0xFFFF);
    sb_Value_DAC_I_2->setMinimum(0);    sb_Value_DAC_I_2->setMaximum(0xFFFF);
    sb_Value_DAC_I_3->setMinimum(0);    sb_Value_DAC_I_3->setMaximum(0xFFFF);
    sb_Value_DAC_I_4->setMinimum(0);    sb_Value_DAC_I_4->setMaximum(0xFFFF);
    sb_Value_DAC_I_5->setMinimum(0);    sb_Value_DAC_I_5->setMaximum(0xFFFF);
    sb_Value_DAC_I_6->setMinimum(0);    sb_Value_DAC_I_6->setMaximum(0xFFFF);
    sb_Value_DAC_I_7->setMinimum(0);    sb_Value_DAC_I_7->setMaximum(0xFFFF);
    sb_Value_DAC_I_8->setMinimum(0);    sb_Value_DAC_I_8->setMaximum(0xFFFF);
    sb_Value_DAC_I_9->setMinimum(0);    sb_Value_DAC_I_9->setMaximum(0xFFFF);
    sb_Value_DAC_I_10->setMinimum(0);   sb_Value_DAC_I_10->setMaximum(0xFFFF);
    sb_Value_DAC_I_11->setMinimum(0);   sb_Value_DAC_I_11->setMaximum(0xFFFF);
    sb_Value_DAC_I_11_HOT->setMinimum(0);    sb_Value_DAC_I_11_HOT->setMaximum(0xFFFF);
    sb_Value_DAC_I_12->setMinimum(0);   sb_Value_DAC_I_12->setMaximum(0xFFFF);
    //---

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Value_ADC_U_0")),            0, 0); grid->addWidget(sb_Value_ADC_U_0,             0, 1);
    grid->addWidget(new QLabel(tr("Value_ADC_U_1")),            1, 0); grid->addWidget(sb_Value_ADC_U_1,             1, 1);
    grid->addWidget(new QLabel(tr("Value_ADC_U_2")),            2, 0); grid->addWidget(sb_Value_ADC_U_2,             2, 1);
    grid->addWidget(new QLabel(tr("Value_ADC_U_3")),            3, 0); grid->addWidget(sb_Value_ADC_U_3,             3, 1);

    grid->addWidget(new QLabel(tr("Value_DAC_U_0")),            4, 0); grid->addWidget(sb_Value_DAC_U_0,             4, 1);
    grid->addWidget(new QLabel(tr("Value_DAC_U_1")),            5, 0); grid->addWidget(sb_Value_DAC_U_1,             5, 1);
    grid->addWidget(new QLabel(tr("Value_DAC_U_2")),            6, 0); grid->addWidget(sb_Value_DAC_U_2,             6, 1);
    grid->addWidget(new QLabel(tr("Value_DAC_U_3")),            7, 0); grid->addWidget(sb_Value_DAC_U_3,             7, 1);

    grid->addWidget(new QLabel(tr("parrot_in_point")),          8, 0); grid->addWidget(sb_parrot_in_point,           8, 1);

    //---
    grid->addWidget(new QLabel(tr("Value_ADC_I_0")),            0, 2); grid->addWidget(sb_Value_ADC_I_0,             0, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_1")),            1, 2); grid->addWidget(sb_Value_ADC_I_1,             1, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_2")),            2, 2); grid->addWidget(sb_Value_ADC_I_2,             2, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_3")),            3, 2); grid->addWidget(sb_Value_ADC_I_3,             3, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_4")),            4, 2); grid->addWidget(sb_Value_ADC_I_4,             4, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_5")),            5, 2); grid->addWidget(sb_Value_ADC_I_5,             5, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_6")),            6, 2); grid->addWidget(sb_Value_ADC_I_6,             6, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_7")),            7, 2); grid->addWidget(sb_Value_ADC_I_7,             7, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_8")),            8, 2); grid->addWidget(sb_Value_ADC_I_8,             8, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_9")),            9, 2); grid->addWidget(sb_Value_ADC_I_9,             9, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_10")),          10, 2); grid->addWidget(sb_Value_ADC_I_10,           10, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_11")),          11, 2); grid->addWidget(sb_Value_ADC_I_11,           11, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_11_HOT")),      12, 2); grid->addWidget(sb_Value_ADC_I_11_HOT,       12, 3);
    grid->addWidget(new QLabel(tr("Value_ADC_I_12")),          13, 2); grid->addWidget(sb_Value_ADC_I_12,           13, 3);

    grid->addWidget(new QLabel(tr("Value_DAC_I_0")),            0, 6); grid->addWidget(sb_Value_DAC_I_0,             0, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_1")),            1, 6); grid->addWidget(sb_Value_DAC_I_1,             1, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_2")),            2, 6); grid->addWidget(sb_Value_DAC_I_2,             2, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_3")),            3, 6); grid->addWidget(sb_Value_DAC_I_3,             3, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_4")),            4, 6); grid->addWidget(sb_Value_DAC_I_4,             4, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_5")),            5, 6); grid->addWidget(sb_Value_DAC_I_5,             5, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_6")),            6, 6); grid->addWidget(sb_Value_DAC_I_6,             6, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_7")),            7, 6); grid->addWidget(sb_Value_DAC_I_7,             7, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_8")),            8, 6); grid->addWidget(sb_Value_DAC_I_8,             8, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_9")),            9, 6); grid->addWidget(sb_Value_DAC_I_9,             9, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_10")),          10, 6); grid->addWidget(sb_Value_DAC_I_10,           10, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_11")),          11, 6); grid->addWidget(sb_Value_DAC_I_11,           11, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_11_HOT")),      12, 6); grid->addWidget(sb_Value_DAC_I_11_HOT,       12, 7);
    grid->addWidget(new QLabel(tr("Value_DAC_I_12")),          13, 6); grid->addWidget(sb_Value_DAC_I_12,           13, 7);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    //dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        zero_U.Value_ADC_U[0] = sb_Value_ADC_U_0->value();
        zero_U.Value_ADC_U[1] = sb_Value_ADC_U_1->value();
        zero_U.Value_ADC_U[2] = sb_Value_ADC_U_2->value();
        zero_U.Value_ADC_U[3] = sb_Value_ADC_U_3->value();

        zero_U.parrot_in_point = sb_parrot_in_point->value();

        zero_U.Value_DAC_U[0] = sb_Value_DAC_U_0->value();
        zero_U.Value_DAC_U[1] = sb_Value_DAC_U_1->value();
        zero_U.Value_DAC_U[2] = sb_Value_DAC_U_2->value();
        zero_U.Value_DAC_U[3] = sb_Value_DAC_U_3->value();

        zero_I.Value_ADC_I[0] = sb_Value_ADC_I_0->value();
        zero_I.Value_ADC_I[1] = sb_Value_ADC_I_1->value();
        zero_I.Value_ADC_I[2] = sb_Value_ADC_I_2->value();
        zero_I.Value_ADC_I[3] = sb_Value_ADC_I_3->value();
        zero_I.Value_ADC_I[4] = sb_Value_ADC_I_4->value();
        zero_I.Value_ADC_I[5] = sb_Value_ADC_I_5->value();
        zero_I.Value_ADC_I[6] = sb_Value_ADC_I_6->value();
        zero_I.Value_ADC_I[7] = sb_Value_ADC_I_7->value();
        zero_I.Value_ADC_I[8] = sb_Value_ADC_I_8->value();
        zero_I.Value_ADC_I[9] = sb_Value_ADC_I_9->value();
        zero_I.Value_ADC_I[10] = sb_Value_ADC_I_10->value();
        zero_I.Value_ADC_I[11] = sb_Value_ADC_I_11->value();
        zero_I.Value_ADC_I[12] = sb_Value_ADC_I_12->value();

        zero_I.Value_DAC_I[0] = sb_Value_DAC_I_0->value();
        zero_I.Value_DAC_I[1] = sb_Value_DAC_I_1->value();
        zero_I.Value_DAC_I[2] = sb_Value_DAC_I_2->value();
        zero_I.Value_DAC_I[3] = sb_Value_DAC_I_3->value();
        zero_I.Value_DAC_I[4] = sb_Value_DAC_I_4->value();
        zero_I.Value_DAC_I[5] = sb_Value_DAC_I_5->value();
        zero_I.Value_DAC_I[6] = sb_Value_DAC_I_6->value();
        zero_I.Value_DAC_I[7] = sb_Value_DAC_I_7->value();
        zero_I.Value_DAC_I[8] = sb_Value_DAC_I_8->value();
        zero_I.Value_DAC_I[9] = sb_Value_DAC_I_9->value();
        zero_I.Value_DAC_I[10] = sb_Value_DAC_I_10->value();
        zero_I.Value_DAC_I[11] = sb_Value_DAC_I_11->value();
        zero_I.Value_DAC_I[12] = sb_Value_DAC_I_12->value();

        uint8_t r_error = 0;
        int err = send_cmd_0x5C(zero_U,
                                zero_I,
                                &r_error);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_error %1").arg(r_error));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x5D(void)
{
    s_zero_b590_U zero_U;
    s_zero_b590_I zero_I;
    int err = send_cmd_0x5D(&zero_U,
                            &zero_I);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    //---
    emit info(QString("Value_ADC_U[0] %1").arg(zero_U.Value_ADC_U[0]));
    emit info(QString("Value_ADC_U[1] %1").arg(zero_U.Value_ADC_U[1]));
    emit info(QString("Value_ADC_U[2] %1").arg(zero_U.Value_ADC_U[2]));
    emit info(QString("Value_ADC_U[3] %1").arg(zero_U.Value_ADC_U[3]));

    emit info(QString("Value_DAC_U[0] %1").arg(zero_U.Value_DAC_U[0]));
    emit info(QString("Value_DAC_U[1] %1").arg(zero_U.Value_DAC_U[1]));
    emit info(QString("Value_DAC_U[2] %1").arg(zero_U.Value_DAC_U[2]));
    emit info(QString("Value_DAC_U[3] %1").arg(zero_U.Value_DAC_U[3]));

    emit info(QString("parrot_in_point %1").arg(zero_U.parrot_in_point));
    //---
    emit info(QString("Value_ADC_I[0] %1").arg(zero_I.Value_ADC_I[0]));
    emit info(QString("Value_ADC_I[1] %1").arg(zero_I.Value_ADC_I[1]));
    emit info(QString("Value_ADC_I[2] %1").arg(zero_I.Value_ADC_I[2]));
    emit info(QString("Value_ADC_I[3] %1").arg(zero_I.Value_ADC_I[3]));
    emit info(QString("Value_ADC_I[4] %1").arg(zero_I.Value_ADC_I[4]));
    emit info(QString("Value_ADC_I[5] %1").arg(zero_I.Value_ADC_I[5]));
    emit info(QString("Value_ADC_I[6] %1").arg(zero_I.Value_ADC_I[6]));
    emit info(QString("Value_ADC_I[7] %1").arg(zero_I.Value_ADC_I[7]));
    emit info(QString("Value_ADC_I[8] %1").arg(zero_I.Value_ADC_I[8]));
    emit info(QString("Value_ADC_I[9] %1").arg(zero_I.Value_ADC_I[9]));
    emit info(QString("Value_ADC_I[10] %1").arg(zero_I.Value_ADC_I[10]));
    emit info(QString("Value_ADC_I[11] %1").arg(zero_I.Value_ADC_I[11]));
    emit info(QString("Value_ADC_I[12] %1").arg(zero_I.Value_ADC_I[12]));

    emit info(QString("Value_DAC_I[0] %1").arg(zero_I.Value_DAC_I[0]));
    emit info(QString("Value_DAC_I[1] %1").arg(zero_I.Value_DAC_I[1]));
    emit info(QString("Value_DAC_I[2] %1").arg(zero_I.Value_DAC_I[2]));
    emit info(QString("Value_DAC_I[3] %1").arg(zero_I.Value_DAC_I[3]));
    emit info(QString("Value_DAC_I[4] %1").arg(zero_I.Value_DAC_I[4]));
    emit info(QString("Value_DAC_I[5] %1").arg(zero_I.Value_DAC_I[5]));
    emit info(QString("Value_DAC_I[6] %1").arg(zero_I.Value_DAC_I[6]));
    emit info(QString("Value_DAC_I[7] %1").arg(zero_I.Value_DAC_I[7]));
    emit info(QString("Value_DAC_I[8] %1").arg(zero_I.Value_DAC_I[8]));
    emit info(QString("Value_DAC_I[9] %1").arg(zero_I.Value_DAC_I[9]));
    emit info(QString("Value_DAC_I[10] %1").arg(zero_I.Value_DAC_I[10]));
    emit info(QString("Value_DAC_I[11] %1").arg(zero_I.Value_DAC_I[11]));
    emit info(QString("Value_DAC_I[12] %1").arg(zero_I.Value_DAC_I[12]));

    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x5E(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x5E_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B590);
    sb_profile->setMaximum(MAX_PROFILE_B590);

    QSpinBox *sb_voltage_mV = new QSpinBox();
    sb_voltage_mV->setMinimum(MIN_VOLTAGE_B590);
    sb_voltage_mV->setMaximum(MAX_VOLTAGE_B590);

    QSpinBox *sb_current_mA = new QSpinBox();
    sb_current_mA->setMinimum(MIN_CURRENT_B590);
    sb_current_mA->setMaximum(MAX_CURRENT_B590);

    QSpinBox *sb_time = new QSpinBox();
    sb_time->setMinimum(MIN_TIME_B590);
    sb_time->setMaximum(MAX_TIME_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Профиль")),      0, 0);
    grid->addWidget(sb_profile,                     0, 1);
    grid->addWidget(new QLabel(tr("Напряжение")),   1, 0);
    grid->addWidget(sb_voltage_mV,                  1, 1);
    grid->addWidget(new QLabel(tr("Ток")),          2, 0);
    grid->addWidget(sb_current_mA,                  3, 1);
    grid->addWidget(new QLabel(tr("Время")),        3, 0);
    grid->addWidget(sb_time,                        3, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned short r_error = 0;
        unsigned char  r_profile = 0;
        unsigned char  r_point = 0;
        uint16_t r_voltage = 0;
        uint16_t r_current = 0;
        unsigned short r_time = 0;
        int err = send_cmd_0x5E(sb_profile->value(),
                                sb_voltage_mV->value(),
                                sb_current_mA->value(),
                                sb_time->value(),
                                &r_error,
                                &r_profile,
                                &r_voltage,
                                &r_current,
                                &r_time);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_error %1").arg(r_error));
        emit info(QString("r_profile %1").arg(r_profile));
        emit info(QString("r_point %1").arg(r_point));
        emit info(QString("r_voltage %1").arg(r_voltage));
        emit info(QString("r_current %1").arg(r_current));
        emit info(QString("r_time %1").arg(r_time));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x5F(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x5F_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B590);
    sb_profile->setMaximum(MAX_PROFILE_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Профиль")),   0, 0);
    grid->addWidget(sb_profile,                  0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned char number_profile = 0;
        int err = send_cmd_0x5F(sb_profile->value(),
                                &number_profile);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("number_profile %1").arg(number_profile));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x60(void)
{
    int err = send_cmd_0x60();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x61(void)
{
    int32_t  Value_ADC_Zero = 0;
    double factor_k_DAC = 0;
    double factor_k_ADC = 0;

    int err = send_cmd_0x61(&Value_ADC_Zero,
                            &factor_k_DAC,
                            &factor_k_ADC);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("Value_Zero_ADC %1").arg(Value_ADC_Zero));
    emit info(QString("factor_k_DAC %1").arg(factor_k_DAC));
    emit info(QString("factor_k_ADC %1").arg(factor_k_ADC));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x62(void)
{
    int32_t  Value_ADC_Zero = 0;
    double factor_k_DAC = 0;
    double factor_k_ADC = 0;

    int err = send_cmd_0x62(&Value_ADC_Zero,
                            &factor_k_DAC,
                            &factor_k_ADC);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("Value_Zero_ADC %1").arg(Value_ADC_Zero));
    emit info(QString("factor_k_DAC %1").arg(factor_k_DAC));
    emit info(QString("factor_k_ADC %1").arg(factor_k_ADC));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x63(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x63_TEXT);

    QSpinBox *sb_number_point = new QSpinBox();
    sb_number_point->setMinimum(MIN_POINT_B590);
    sb_number_point->setMaximum(MAX_POINT_B590);

    QSpinBox *sb_data_point = new QSpinBox();
    sb_data_point->setMinimum(MIN_DATA_B590);
    sb_data_point->setMaximum(MAX_DATA_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Number point")), 0, 0);
    grid->addWidget(sb_number_point,                0, 1);
    grid->addWidget(new QLabel(tr("Data point")),   1, 0);
    grid->addWidget(sb_data_point,                  1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned char  r_number_point = 0;
        unsigned short r_data_point = 0;
        int err = send_cmd_0x63(0,
                                sb_number_point->value(),
                                sb_data_point->value(),
                                &r_number_point,
                                &r_data_point);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_number_point %1").arg(r_number_point));
        emit info(QString("r_data_point %1").arg(r_data_point));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x64(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x64_TEXT);

    QSpinBox *sb_data = new QSpinBox();
    sb_data->setMinimum(1);
    sb_data->setMaximum(5);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("1 - запуск калибровки АЦП по напряжению"),  0, 0);
    grid->addWidget(new QLabel("2 - запуск калибровки АЦП по току"),        1, 0);
    grid->addWidget(new QLabel("3 - запуск калибровки ЦАП по напряжению"),  2, 0);
    grid->addWidget(new QLabel("4 - запуск калибровки ЦАП по току"),        3, 0);
    grid->addWidget(new QLabel("5 - переход к следующей точке (при калибровке АЦП)"), 4, 0);
    grid->addWidget(new QLabel(tr("number")), 5, 0);
    grid->addWidget(sb_data,                5, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        int err = send_cmd_0x64(sb_data->value());
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x65(void)
{
    // 0 Пусто (нет задачи)
    // 1 Запуск калибровки АЦП по напряжению
    // 2 Запуск калибровки АЦП по току
    // 3 Запуск калибровки ЦАП по напряжению
    // 4 Запуск калибровки ЦАП по току
    // 5 Режим SLP
    // 6 Отображение ошибки - выход вниз (запомнить до тех пор пока не окажемся
    // внизу (там отображение)
    // 7 Выход в основное меню (или меню настроек, если неверны калибровки)
    // Поставлена новая задача - прекращаем все текущие
    //Application::get_number_Active_Task();
    //void Application::set_bit_Active_Task(uint8_t new_task)
    //{
    //  Active_Task.Active_Task = new_task;
    //}

    uint8_t number_current_point = 0;
    uint8_t state = 0;
    int err = send_cmd_0x65(&number_current_point, &state);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("number_current_point %1").arg((int)number_current_point));
    switch (state)
    {
    case NO_TASK:                emit info(tr("Нет задачи"));    break;
    case TASK_CALIBRATION_ADC_U: emit info(tr("Запуск калибровки АЦП по напряжению"));    break;
    case TASK_CALIBRATION_ADC_I: emit info(tr("Запуск калибровки АЦП по току"));    break;
    case TASK_CALIBRATION_DAC_U: emit info(tr("Запуск калибровки ЦАП по напряжению"));    break;
    case TASK_CALIBRATION_DAC_I: emit info(tr("Запуск калибровки ЦАП по току"));    break;
    case TASK_SLP:               emit info(tr("Режим SLP"));    break;
    case TASK_SHOW_ERROR:        emit info(tr("Отображение ошибки"));    break;
    case TASK_RETURN_MENU:       emit info(tr("Выход в основное меню"));    break;

    default:
        emit error(QString("unknown state %1").arg(state));
        break;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x66(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x66_TEXT);

    QSpinBox *sb_index_point = new QSpinBox();
    sb_index_point->setMinimum(0);
    sb_index_point->setMaximum(MAX_CALIBRATION_POINTS_B590_I);

    QSpinBox *sb_state = new QSpinBox();
    sb_state->setMinimum(0);
    sb_state->setMaximum(1);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("U=0 I=1")),      0, 0);
    grid->addWidget(sb_state,                       0, 1);
    grid->addWidget(new QLabel(tr("Index point")),  1, 0);
    grid->addWidget(sb_index_point,                 1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned short value = 0;

        int err = send_cmd_0x66(sb_state->value(),  //U=0 I=1
                                sb_index_point->value(),
                                &value);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("point = %1").arg(sb_index_point->value()));
        emit info(QString("value = %1").arg(value));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x67(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x67_TEXT);

    QSpinBox *sb_speed_cooler = new QSpinBox();
    sb_speed_cooler->setMinimum(MIN_SPEED_B590);
    sb_speed_cooler->setMaximum(MAX_SPEED_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Speed cooler")), 0, 0);
    grid->addWidget(sb_speed_cooler,                0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned short r_speed_cooler = 0;
        int err = send_cmd_0x67(sb_speed_cooler->value(),
                                &r_speed_cooler);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_speed_cooler %1").arg(r_speed_cooler));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x68(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x68_TEXT);

    QSpinBox *sb_data_PWM = new QSpinBox();
    sb_data_PWM->setMinimum(MIN_PWM_B590);
    sb_data_PWM->setMaximum(MAX_PWM_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Speed cooler")), 0, 0);
    grid->addWidget(sb_data_PWM,                    0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned short r_data_PWM = 0;
        int err = send_cmd_0x68(sb_data_PWM->value(),
                                &r_data_PWM);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_data_PWM %1").arg(r_data_PWM));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x69(void)
{
    unsigned short OCR3A = 0;
    unsigned short OCR3B = 0;
    int err = send_cmd_0x69(&OCR3A,
                            &OCR3B);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("OCR3A %1").arg(OCR3A));
    emit info(QString("OCR3B %1").arg(OCR3B));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x6A(void)
{
    int err = send_cmd_0x6A();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x6B(void)
{
    int err = send_cmd_0x6B();
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x6C(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x6C_TEXT);

    QSpinBox *sb_number_string = new QSpinBox();
    sb_number_string->setMinimum(MIN_NUMBER_STRING_B590);
    sb_number_string->setMaximum(MAX_NUMBER_STRING_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Number string")), 0, 0);
    grid->addWidget(sb_number_string,                0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        int err = send_cmd_0x6C(sb_number_string->value());
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x6D(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x6D_TEXT);

    QSpinBox *sb_ID = new QSpinBox();
    sb_ID->setMinimum(MIN_ID_B590);
    sb_ID->setMaximum(MAX_ID_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("ID")),       1, 0);
    grid->addWidget(sb_ID,                      1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        uint8_t secret_cmd = 0;
        uint8_t result = 0;
        uint8_t data[128] = { 0 };
        QByteArray ba;
        int err = send_cmd_0x6D(SECRET_CMD_GET_ID,
                                data,
                                &secret_cmd,
                                &result,
                                &ba);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x6E(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x6E_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B590);
    sb_profile->setMaximum(MAX_PROFILE_B590);

    QSpinBox *sb_repeat = new QSpinBox();
    sb_repeat->setMinimum(MIN_REPEAT_B590);
    sb_repeat->setMaximum(MAX_REPEAT_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Profile")),  0, 0);
    grid->addWidget(sb_profile,                 0, 1);
    grid->addWidget(new QLabel(tr("Repeat")),   1, 0);
    grid->addWidget(sb_repeat,                  1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned char r_profile = 0;
        unsigned char r_repeat = 0;
        int err = send_cmd_0x6E(sb_profile->value(),
                                sb_repeat->value(),
                                &r_profile,
                                &r_repeat);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_profile %1").arg(r_profile));
        emit info(QString("r_repeat %1").arg(r_repeat));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x6F(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x6F_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B590);
    sb_profile->setMaximum(MAX_PROFILE_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Profile")),  0, 0);
    grid->addWidget(sb_profile,                 0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned char r_profile = 0;
        int err = send_cmd_0x6F(sb_profile->value(),
                                &r_profile);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_profile %1").arg(r_profile));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x71(void)
{
    unsigned char state_ADC_U = 0;
    unsigned char state_ADC_I = 0;
    unsigned char state_ADC_Ui = 0;
    int err = send_cmd_0x71(&state_ADC_U,
                            &state_ADC_I,
                            &state_ADC_Ui);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("state_ADC_U %1").arg(state_ADC_U));
    emit info(QString("state_ADC_I %1").arg(state_ADC_I));
    emit info(QString("state_ADC_Ui %1").arg(state_ADC_Ui));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x72(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B590_CMD_0x72_TEXT);

    QSpinBox *sb_conf_ADC = new QSpinBox();
    sb_conf_ADC->setMinimum(MIN_CONF_ADC_B590);
    sb_conf_ADC->setMaximum(MAX_CONF_ADC_B590);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("conf_ADC")), 0, 0);
    grid->addWidget(sb_conf_ADC,                0, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    dialog->setFixedSize(320, 200);

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        unsigned short r_conf_ADC = 0;
        int err = send_cmd_0x72(sb_conf_ADC->value(),
                                &r_conf_ADC);
        if(err != E_B590_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_conf_ADC %1").arg(r_conf_ADC));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x73(void)
{
    unsigned short real_DAC_U = 0;
    unsigned short real_DAC_I = 0;
    unsigned char  revers = 0;
    unsigned short temperature = 0;
    int err = send_cmd_0x73(&real_DAC_U,
                            &real_DAC_I,
                            &revers,
                            &temperature);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("real_DAC_U %1").arg(real_DAC_U));
    emit info(QString("real_DAC_I %1").arg(real_DAC_I));
    emit info(QString("revers %1").arg(revers));
    emit info(QString("temperature %1").arg(temperature));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B590::run_dialog_0x75(void)
{
    unsigned char count_ReStart_ADC_1 = 0;
    unsigned char count_ReStart_ADC_0 = 0;
    unsigned char count_ReStart_ADC_2 = 0;
    unsigned short count_Error_Answer_ADC = 0;
    unsigned int   count_Error_ADC = 0;
    unsigned short MCUCSR = 0;
    int err = send_cmd_0x75(&count_ReStart_ADC_1,
                            &count_ReStart_ADC_0,
                            &count_ReStart_ADC_2,
                            &count_Error_Answer_ADC,
                            &count_Error_ADC,
                            &MCUCSR);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("count_ReStart_ADC_1 %1").arg(count_ReStart_ADC_1));
    emit info(QString("count_ReStart_ADC_0 %1").arg(count_ReStart_ADC_0));
    emit info(QString("count_ReStart_ADC_2 %1").arg(count_ReStart_ADC_2));
    emit info(QString("count_Error_Answer_ADC %1").arg(count_Error_Answer_ADC));
    emit info(QString("count_Error_ADC %1").arg(count_Error_ADC));
    emit info(QString("MCUCSR %1").arg(MCUCSR));
    emit info("OK");
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_write(uint16_t fram_address,
                                  uint8_t  len_data,
                                  QByteArray fram_data)
{
    DATA_FRAM_MORE_BYTES temp;
    temp.data.address = fram_address;
    temp.data.data_len = len_data;
    for(int n=0; n<len_data; n++)
    {
        temp.data.fram_data[n] = fram_data.at(n);
    }

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_SAVE_FRAM_MORE_BYTES,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_write_8(uint16_t fram_address,
                                    uint8_t fram_data)
{
    DATA_FRAM_8BIT temp;
    temp.data.address = fram_address;
    temp.data.value   = fram_data;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_SAVE_FRAM_8BIT,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_write_16(uint16_t fram_address,
                                     uint16_t fram_data)
{
    DATA_FRAM_16BIT temp;
    temp.data.address = fram_address;
    temp.data.value   = fram_data;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_SAVE_FRAM_16BIT,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_write_32(uint16_t fram_address,
                                     uint32_t fram_data)
{
    DATA_FRAM_32BIT temp;
    temp.data.address = fram_address;
    temp.data.value   = fram_data;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_SAVE_FRAM_8BIT,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_read(uint16_t fram_address,
                                 uint8_t len_data,
                                 QByteArray *fram_data)
{
    DATA_FRAM_MORE_BYTES temp;
    temp.data.address  = fram_address;
    temp.data.data_len = len_data;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray *ba = new QByteArray();
    ba->clear();
    int err = send_cmd_0x6D(SECRET_CMD_READ_FRAM_MORE_BYTES,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }

    for(unsigned int n=0; n<sizeof(DATA_FRAM_MORE_BYTES); n++)
    {
        temp.buf[n] = ba->at(n);
    }
    for(unsigned int n=0; n<len_data; n++)
    {
        (*fram_data).append(temp.data.fram_data[n]);
    }

    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_read_8(uint16_t fram_address,
                                   uint8_t *fram_data)
{
    DATA_FRAM_8BIT temp;
    temp.data.address = fram_address;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_READ_FRAM_8BIT,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    for(int n=0; n<ba.length(); n++)
    {
        temp.buf[n] = ba.at(n);
    }
    *fram_data = temp.data.value;
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_read_16(uint16_t fram_address,
                                    uint16_t *fram_data)
{
    DATA_FRAM_16BIT temp;
    temp.data.address = fram_address;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_READ_FRAM_16BIT,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    if(ba.length() != 128)
    {
        emit error(QString("len %1").arg(ba.length()));
    }
    for(int n=0; n<ba.length(); n++)
    {
        temp.buf[n] = ba.at(n);
    }
    *fram_data = temp.data.value;
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::fram_read_32(uint16_t fram_address,
                                    uint32_t *fram_data)
{
    DATA_FRAM_32BIT temp;
    temp.data.address = fram_address;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_READ_FRAM_32BIT,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    for(int n=0; n<ba.length(); n++)
    {
        temp.buf[n] = ba.at(n);
    }
    *fram_data = temp.data.value;
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_ID(uint8_t *ID)
{
    DATA_FRAM_GET_ID temp;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_GET_ID,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    for(int n=0; n<ba.length(); n++)
    {
        temp.buf[n] = ba.at(n);
    }
    *ID = temp.data.ID;
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::set_date_and_serno(uint16_t year,
                                          uint8_t  month,
                                          uint16_t serno)
{
    DATA_FRAM_DATE_AND_SERNO temp;
    temp.data.year = year;
    temp.data.month = month;
    temp.data.serno = serno;

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_SAVE_DATE_AND_SERNO,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    for(int n=0; n<ba.length(); n++)
    {
        temp.buf[n] = ba.at(n);
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::copy_user_calibration_point_to_factory(void)
{
    DATA_FRAM_8BIT temp; //просто так

    uint8_t secret_cmd = 0;
    uint8_t result = 0;
    QByteArray ba;
    int err = send_cmd_0x6D(SECRET_CMD_COPY_USER_TO_FACTORY,
                            temp.buf,
                            &secret_cmd,
                            &result,
                            &ba);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
#ifdef NEEDED_PAUSE
void Powersupply_B590::pause(unsigned int msec)
{
#ifndef FAKE
    Sleeper::msleep(msec);
#endif
}
#endif
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#if 0
#define B590_CMD_0x54_TEXT tr("Б5-90 функция 0x54: Запрос данных по профилям.")
#define B590_CMD_0x56_TEXT tr("Б5-90 функция 0x56: Чтение профиля по точкам.")
#define B590_CMD_0x5E_TEXT tr("Б5-90 функция 0x5E: Запись данных точки в профиль для исполнения.")
#define B590_CMD_0x5F_TEXT tr("Б5-90 функция 0x5F: Запуск профиля на исполнение.")
#define B590_CMD_0x6E_TEXT tr("Б5-90 функция 0x6E: Запись количества повторов в профиль.")
#define B590_CMD_0x6F_TEXT tr("Б5-90 функция 0x6F: Очистка профиля")
#endif
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_data_profile(int number_profile,
                                        unsigned char *count_point,
                                        unsigned char *count_repeat_profile)
{
    int err = send_cmd_0x54(number_profile, count_point, count_repeat_profile);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_point_profile(uint8_t number_profile,
                                         uint8_t number_point,
                                         uint8_t *r_profile,
                                         uint8_t *r_point,
                                         uint16_t *r_voltage,
                                         uint16_t *r_current,
                                         uint16_t *r_time)
{
    int err = send_cmd_0x56(number_profile,
                            number_point,
                            r_profile,
                            r_point,
                            r_voltage,
                            r_current,
                            r_time);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::add_point_profile(uint8_t number_profile,
                                         uint16_t voltage,
                                         uint16_t current,
                                         uint16_t time)
{
    uint16_t r_error = 0;
    uint8_t  r_profile = 0;
    uint16_t r_voltage_mV = 0;
    uint16_t r_current_mA = 0;
    uint16_t r_time = 0;
    int err = send_cmd_0x5E(number_profile,
                            voltage,
                            current,
                            time,
                            &r_error,
                            &r_profile,
                            &r_voltage_mV,
                            &r_current_mA,
                            &r_time);
    if(r_error)
    {
        emit error(QString("r_error %1").arg(r_error));
    }
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::run_profile(uint8_t number_profile,
                                   uint8_t *r_profile)
{
    int err = send_cmd_0x5F(number_profile, r_profile);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::add_count_repeat_profile(int number_profile, int repeat_count)
{
    unsigned char r_profile = 0;
    unsigned char r_repeat = 0;
    int err = send_cmd_0x6E(number_profile,
                            repeat_count,
                            &r_profile,
                            &r_repeat);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::clear_profile(int number_profile)
{
    unsigned char r_profile = 0;
    int err = send_cmd_0x6F(number_profile, &r_profile);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
int Powersupply_B590::print_last_error(void)
{
    switch(last_error)
    {
    case E_B590_NO_ERROR:                   emit info(tr("NO ERROR")); break;
    case E_B590_ERROR_INDEX_NOT_CORRECT:    emit error(tr("Индекс не корректен")); break;
    case E_B590_ERROR_PORT_NOT_OPEN:        emit error(tr("Порт источника питания Б5-90 не открыт!")); break;
    case E_B590_ERROR_BREAK:                emit error(tr("E_B590_ERROR_BREAK")); break;
    case E_B590_ERROR_MAX_REPEAT:           emit error(tr("E_B590_ERROR_MAX_REPEAT")); break;
    case E_B590_ERROR_NO_ANSWER:            emit error(QString(tr("%1 не отвечает")).arg(POWER_SUPPLY_NAME_B590)); break;
    case E_B590_ERROR_BIG_ANSWER:           emit error(tr("Неверная длина пакета (слишком большой)")); break;
    case E_B590_ERROR_SMALL_ANSWER:         emit error(tr("Неверная длина пакета (слишком маленький)")); break;
    case E_B590_ERROR_BAD_SERNO:            emit error(tr("Неверный серийный номер")); break;
    case E_B590_ERROR_BAD_ADDRESS:          emit error(tr("Неверный адрес")); break;
    case E_B590_ERROR_BAD_CRC:              emit error(tr("Ошибка контрольной суммы")); break;
    case E_B590_ERROR_BAD_COMMAND:          emit error(tr("Плохая команда")); break;
    case E_B590_ERROR_BAD_LEN:              emit error(tr("Неверная длина")); break;
    case E_B590_ERROR_UNKNOWN_COMMAND:      emit error(tr("Неизвестная команда")); break;
    case E_B590_INVALID_FLOAT:              emit error(tr("E_B590_INVALID_FLOAT")); break;
    case E_B590_INVALID_LIMIT:              emit error(tr("E_B590_INVALID_LIMIT")); break;
    case E_B590_INVALID_DELTA:              emit error(tr("E_B590_INVALID_DELTA")); break;
    case E_B590_BREAK_MEASURING:            emit error(tr("E_B590_BREAK_MEASURING")); break;
    case E_B590_UNKNOWN_PROFILE:            emit error(tr("Недопустимый номер профиля")); break;
    case E_B590_UNKNOWN_POINT:              emit error(tr("Недопустимый номер точки в профиле")); break;
    case E_B590_PROFILE_EMPTY:              emit error(tr("Профиль пуст")); break;
    case E_B590_ERROR_SIGNATURE:            emit error(tr("Не совпадает сигнатура")); break;
    case E_B590_NUMBER_PROFILE_ERROR:       emit error(tr("Недопустимый номер профиля.")); break;
    case E_B590_NUMBER_POINT_ERROR:         emit error(tr("Недопустимый номер точки.")); break;
    case E_B590_NUMBER_STRING_ERROR:        emit error(tr("Недопустимый номер строки.")); break;
    case E_B590_BUSY:                       emit error(tr("Прибор занят выполнением команды")); break;
    case E_B590_NUMBER_REPEAT_ERROR:        emit error(tr("Недопустимое количество повторов")); break;
    case E_B590_LO_VOLTAGE:                 emit error(tr("Значение напряжение ниже минимально допустимого.")); break;
    case E_B590_HI_VOLTAGE:                 emit error(tr("Превышено максимальное значение напряжения.")); break;
    case E_B590_LO_CURRENT:                 emit error(tr("Значение тока ниже минимально допустимого.")); break;
    case E_B590_HI_CURRENT:                 emit error(tr("Превышено максимальное значение тока.")); break;
    case E_B590_NO_REVERSE:                 emit error(tr("Нет платы реверса - попытка использовать реверс тока.")); break;
    case E_B590_MAX_POWER:                  emit error(tr("Превышена максимальная мощность источника.")); break;
    case E_B590_LO_TIME:                    emit error(tr("Значение времени ниже минимально допустимого.")); break;
    case E_B590_HI_TIME:                    emit error(tr("Превышено максимальное значение времени.")); break;
    case E_B590_ERROR_POINT:                emit error(tr("Неверный порядок точек при записи.")); break;
    case E_B590_PROFILE_IS_RUN:             emit error(tr("Попытка записи в исполняемый профиль.")); break;
    case E_B590_CALIBRATION_RUNNING:        emit error(tr("Производится процесс калибровки.")); break;
    case E_B590_NO_RC:                      emit error(tr("Нет захвата управления по данному интерфейсу.")); break;
    case E_B590_COMMAND_ON_CONSTRUCTION:    emit error(tr("Пока не сделано")); break;

    default:                                emit error(QString("Unknown error %1").arg(last_error)); break;
    }
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B590::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
void Powersupply_B590::print_adv_error(uint32_t bits, uint16_t code)
{
    U_INT error_int;
    error_int.value = bits;
    if(error_int.bites.bit0)  emit error(ERROR_E00_B590_TEXT);
    if(error_int.bites.bit1)  emit error(ERROR_E01_B590_TEXT);
    if(error_int.bites.bit2)  emit error(ERROR_E02_B590_TEXT);
    if(error_int.bites.bit3)  emit error(ERROR_E03_B590_TEXT);
    if(error_int.bites.bit4)  emit error(ERROR_E04_B590_TEXT);
    if(error_int.bites.bit5)  emit error(ERROR_E05_B590_TEXT);
    if(error_int.bites.bit6)  emit error(ERROR_E06_B590_TEXT);
    if(error_int.bites.bit7)  emit error(ERROR_E07_B590_TEXT);
    if(error_int.bites.bit8)  emit error(ERROR_E08_B590_TEXT);
    if(error_int.bites.bit9)  emit error(ERROR_E09_B590_TEXT);
    if(error_int.bites.bit10) emit error(ERROR_E10_B590_TEXT);
    if(error_int.bites.bit11) emit error(ERROR_E11_B590_TEXT);
    if(error_int.bites.bit12) emit error(ERROR_E12_B590_TEXT);
    if(error_int.bites.bit13) emit error(ERROR_E13_B590_TEXT);
    if(error_int.bites.bit14) emit error(ERROR_E14_B590_TEXT);
    if(error_int.bites.bit15) emit error(ERROR_E15_B590_TEXT);
    if(error_int.bites.bit16) emit error(ERROR_E16_B590_TEXT);
    if(error_int.bites.bit17) emit error(ERROR_E17_B590_TEXT);
    if(error_int.bites.bit18) emit error(ERROR_E18_B590_TEXT);
    if(error_int.bites.bit19) emit error(ERROR_E19_B590_TEXT);
    if(error_int.bites.bit20) emit error(ERROR_E20_B590_TEXT);
    if(error_int.bites.bit21) emit error(ERROR_E21_B590_TEXT);
    if(error_int.bites.bit24) emit error(ERROR_E24_B590_TEXT);
    if(error_int.bites.bit25) emit error(ERROR_E25_B590_TEXT);
    if(error_int.bites.bit26) emit error(ERROR_E26_B590_TEXT);
    if(error_int.bites.bit27) emit error(ERROR_E27_B590_TEXT);
    if(error_int.bites.bit28) emit error(ERROR_E28_B590_TEXT);
    if(error_int.bites.bit29) emit error(ERROR_E29_B590_TEXT);
    if(error_int.bites.bit30) emit error(ERROR_E30_B590_TEXT);
    if(error_int.bites.bit31) emit error(ERROR_E31_B590_TEXT);

    if(code)
    {
        emit error(QString("Error code: %1").arg(code));
    }
}
//--------------------------------------------------------------------------------
// тестовые функции
//--------------------------------------------------------------------------------
unsigned int Powersupply_B590::get_Correct_value_U(unsigned int new_setting_U)
{
    // Чтобы можно было выставить гарантированный нуль с запасом
    if (new_setting_U == 0)
    {
        return 0;
    }
    int index = (int)(new_setting_U / MAGIC_B590_U);  // Получаем индекс в таблице

    //
    uint16_t value = 0;
    uint16_t value2 = 0;
    uint16_t value3 = 0;
    bool ok = false;
    ok = get_calibration_point_U(COUNT_B590_CALIB_POINTS_DAC_U-20, &value);
    if(!ok) print_last_error();
    ok = get_calibration_point_U(index, &value2);
    if(!ok) print_last_error();
    ok = get_calibration_point_U(index + 1, &value3);
    if(!ok) print_last_error();
    //

    signed long V1 = 0;
    if(index > (COUNT_B590_CALIB_POINTS_DAC_U - 1))
    {
        float y = (float)MAX_B590_VALUE_U - (float)value;
        float x = (float)MAGIC_B590_U * 20;
        float m = (float)new_setting_U - (float)MAX_B590_VALUE_U + x;
        float n = (y * m) / x;
        //V1 = correct_DAC_U[COUNT_B590_CALIB_POINTS_DAC_U-1] + (signed long)n;
        V1 = value + (signed long)n;
        return abs(V1);
    }

    //---
    if(index == (COUNT_B590_CALIB_POINTS_DAC_U - 1))
    {
        //V1 = ((signed long)(new_setting_U % MAGIC_U) * (MAX_VALUE_U - correct_DAC_U[index])) / MAGIC_U;
        V1 = ((signed long)(new_setting_U % MAGIC_B590_U) * (MAX_B590_VALUE_U - value2)) / MAGIC_B590_U;
        //V1 += correct_DAC_U[index];
        V1 += value2;
        return abs(V1);
    }

    //---
    //V1 = ((signed long)(new_setting_U % MAGIC_U) * (correct_DAC_U[index + 1] - correct_DAC_U[index])) / MAGIC_U;
    V1 = ((signed long)(new_setting_U % MAGIC_B590_U) * (value3 - value2)) / MAGIC_B590_U;
    //V1 += correct_DAC_U[index];
    V1 += value2;
    return abs(V1);
}
//=============================================================================
unsigned int Powersupply_B590::get_Correct_value_I(unsigned long new_setting_I)
{
    // Чтобы можно было выставить гарантированный нуль с запасом
    if (new_setting_I < MINIMUM_B590_I)
    {
        return 0;
    }

    new_setting_I -= MINIMUM_B590_I;  // Так как вся таблица смещена 10 мА
    int index = (int)(new_setting_I / MAGIC_B590_I);  // Получаем индекс в таблице

    //
    uint16_t value = 0;
    uint16_t value2 = 0;
    uint16_t value3 = 0;
    bool ok = false;
    ok = get_calibration_point_I(COUNT_B590_CALIB_POINTS_DAC_I-20, &value);
    if(!ok) print_last_error();
    ok = get_calibration_point_I(index, &value2);
    if(!ok) print_last_error();
    ok = get_calibration_point_I(index + 1, &value3);
    if(!ok) print_last_error();
    //

    signed long V1 = 0;
    if(index > (COUNT_B590_CALIB_POINTS_DAC_I - 1))
    {
        float y = (float)MAX_B590_VALUE_I - (float)value;
        float x = (float)MAGIC_B590_I * 20;
        float m = (float)new_setting_I - (float)MAX_B590_VALUE_I + x;
        float n = (y * m) / x;
        //V1 = correct_DAC_I[COUNT_CALIB_POINTS_DAC_I-1] + (signed long)n;
        V1 = value + (signed long)n;
        return abs(V1);
    }

    //---
    if(index == (COUNT_B590_CALIB_POINTS_DAC_I - 1))
    {
        //V1 = ((signed long)(new_setting_I % MAGIC_I) * (MAX_VALUE_I - correct_DAC_I[index])) / MAGIC_I;
        V1 = ((signed long)(new_setting_I % MAGIC_B590_I) * (MAX_B590_VALUE_I - value2)) / MAGIC_B590_I;
        //V1 += correct_DAC_I[index];
        V1 += value2;
        return abs(V1);
    }

    //---
    //V1 = ((signed long)(new_setting_I % MAGIC_B590_I) * (correct_DAC_I[index + 1] - correct_DAC_I[index])) / MAGIC_B590_I;
    V1 = ((signed long)(new_setting_I % MAGIC_B590_I) * (value3 - value2)) / MAGIC_B590_I;
    //V1 += correct_DAC_I[index];
    V1 += value2;
    return abs(V1);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_error(uint32_t *bits, uint16_t *code)
{
    int err = send_cmd_0x4A(bits, code);
    return (err == E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Powersupply_B590::serial_send(const char *data, int len)
{
    QByteArray ba;
    ba.append(data, len);
    emit debug(QString("serial_send: [%1]").arg(ba.toHex().data()));

    serial.write(data, len);
}
//--------------------------------------------------------------------------------
bool Powersupply_B590::get_info(QTextEdit *log)
{
    bool ok = false;
    QString temp;
    s_zero_b590_U zero_U;
    s_zero_b590_I zero_I;

    int err = send_cmd_0x5D(&zero_U, &zero_I);
    if(err != E_B590_NO_ERROR)
    {
        print_last_error();
        return ok;
    }

    //---
    temp.clear();
    for(int n=0; n<CALIBRATION_POINTS_U; n++)
    {
        temp.append(QString("Value_ADC_U[%1] %2\n").arg(n).arg(zero_U.Value_ADC_U[n]));
    }
    for(int n=0; n<CALIBRATION_POINTS_U; n++)
    {
        temp.append(QString("Value_DAC_U[%1] %2\n").arg(n).arg(zero_U.Value_DAC_U[n]));
    }
    for(int n=0; n<CALIBRATION_POINTS_U; n++)
    {
        temp.append(QString("POINT_U[%1] %2\n").arg(n).arg(zero_U.POINT_U[n]));
    }

    temp.append(QString("parrot_in_point %1\n").arg(zero_U.parrot_in_point));
    //---
    for(int n=0; n<CALIBRATION_POINTS_I; n++)
    {
        temp.append(QString("Value_ADC_I[%1] %2\n").arg(n).arg(zero_I.Value_ADC_I[n]));
    }
    for(int n=0; n<CALIBRATION_POINTS_I; n++)
    {
        temp.append(QString("Value_DAC_I[%1] %2\n").arg(n).arg(zero_I.Value_DAC_I[n]));
    }
    for(int n=0; n<CALIBRATION_POINTS_I; n++)
    {
        temp.append(QString("POINT_I[%1] %2\n").arg(n).arg(zero_I.POINT_I[n]));
    }
    //---
    uint16_t value = 0;
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        ok = get_calibration_point_U(n, &value);
        if(ok)
        {
            temp.append(QString("dac_U[%1]=%2;\n").arg(n).arg(value));
        }
        else
        {
            print_last_error();
            return ok;
        }
    }

    temp.append("---\n");
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        ok = get_calibration_point_I(n, &value);
        if(ok)
        {
            temp.append(QString("dac_I[%1]=%2;\n").arg(n).arg(value));
        }
        else
        {
            print_last_error();
            return false;
        }
    }
    //---

    (*log).clear();
    (*log).append(temp);

    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//#include "find_powersupply.hpp"
QWidget *Powersupply_B590::add_frame_find_powersupply(void)
{
    QWidget *frame = new Find_powersupply(this);

    return frame;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Powersupply_B590::event(QEvent *e)
{
    //qDebug() << e->type();
    if(e->type() == QEvent::WindowDeactivate)
    {
#ifdef QT_DEBUG
        qDebug() << "Powersupply_B590 event:WindowDeactivate";
#endif
        flag_closed = true;
    }
    return true;
}
//--------------------------------------------------------------------------------
