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
#include <QGridLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QWidget>
#include <QTime>
#include <QMenu>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "b588_powersupply.hpp"
#include "b588_packets.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "crc.h"

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
Powersupply_B588::Powersupply_B588(QObject *parent) :
    QObject(parent),
    is_ready(false),
    busy(false),
    address(1),
    delay_ms(1000),
    is_silence(false),
    ignore_bad_cmd(false)
{
    init();
}
//--------------------------------------------------------------------------------
Powersupply_B588::~Powersupply_B588()
{
#ifdef QT_DEBUG
    qDebug() << "~Powersupply_B588()";
#endif
#if 0
    if(serial.isOpen())
    {
        serial.close();
    }
#endif
}
//--------------------------------------------------------------------------------
void Powersupply_B588::init(void)
{
    last_error = E_B588_NO_ERROR;
    busy = false;

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

#ifdef FAKE
    connect(&serial, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(&serial, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(&serial, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(&serial, SIGNAL(trace(QString)),  this, SIGNAL(trace(QString)));
#endif
}
//--------------------------------------------------------------------------------
void Powersupply_B588::set_ignore_bad_cmd(bool state)
{
    ignore_bad_cmd = state;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::find_device(void)
{
    QList<int> speeds;

    speeds.clear();
    speeds.append(4800);
    speeds.append(9600);
    speeds.append(19200);
    speeds.append(28800);
    speeds.append(38400);
    speeds.append(57600);
    speeds.append(76800);
    speeds.append(115200);

    if(serial.isOpen()) serial.close();

    set_address(address);
    flag_closed = false;
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("ищем источник питания на порту %1")).arg(port.portName()));
        foreach (int speed, speeds)
        {
            if(flag_closed) return false;

            serial.setPort(port);
            //emit info(QString("speed %1").arg(speed));
            bool ok = serial.setBaudRate(speed);
            if(ok)
            {
                if(serial.open(QIODevice::ReadWrite))
                {
                    uint8_t  type  = 0;
                    uint16_t year  = 0;
                    uint8_t  month = 0;
                    uint16_t serno = 0;
                    int err = send_cmd_0x46(&type, &year, &month, &serno);
                    if(err == E_B588_NO_ERROR)
                    {
                        emit info(tr("Проверяем идентификатор источника"));
                        if(type == 3)
                        {
                            emit info(QString(tr("Источник питания найден на скорости %1")).arg(speed));
                            return true;
                        }
                        else
                        {
                            emit error(QString("Найдено неизвестное устройство: type == %1").arg(type));
                        }
                    }
                    if((err != E_B588_ERROR_NO_ANSWER) && (err != E_B588_NO_ERROR))
                    {
                        print_last_error();
                    }
                    serial.close();
                }
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::port_is_open(void)
{
    return serial.isOpen();
}
//--------------------------------------------------------------------------------
void Powersupply_B588::port_read(void)
{
    data_powersupply.append(serial.readAll());
    is_ready = true;
}
//--------------------------------------------------------------------------------
void Powersupply_B588::port_error(QSerialPort::SerialPortError serial_error)
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
void Powersupply_B588::set_delay_ms(int delay_ms)
{
    emit info(QString("new delay %1 ms").arg(delay_ms));
    this->delay_ms = delay_ms;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::get_delay_ms(void)
{
    return delay_ms;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::check_dirty_packet(const QByteArray &data)
{
    if(data.length() < 2) return PACKET_B588_FAIL;

    unsigned char cmd = data.at(1) | 0x7F;
    //unsigned char err = data.at(1) | 0x80;

    int result = PACKET_B588_OK;
    switch(cmd)
    {
    case B588_CMD_0x41: result = PACKET_B588_OK; break;
    case B588_CMD_0x42: result = PACKET_B588_OK; break;
    case B588_CMD_0x46: result = check_packet_answer(B588_CMD_0x46, sizeof(B588_CMD_46_ANSWER)); break;
    case B588_CMD_0x47: result = check_packet_answer(B588_CMD_0x47, sizeof(B588_CMD_47_ANSWER)); break;
    case B588_CMD_0x48: result = check_packet_answer(B588_CMD_0x48, sizeof(B588_CMD_48_ANSWER)); break;
    case B588_CMD_0x49: result = check_packet_answer(B588_CMD_0x49, sizeof(B588_CMD_49_ANSWER)); break;
    case B588_CMD_0x4A: result = check_packet_answer(B588_CMD_0x4A, sizeof(B588_CMD_4A_ANSWER)); break;
    case B588_CMD_0x54: result = check_packet_answer(B588_CMD_0x54, sizeof(B588_CMD_54_ANSWER)); break;
    case B588_CMD_0x55: result = check_packet_answer(B588_CMD_0x55, sizeof(B588_CMD_55_ANSWER)); break;
    case B588_CMD_0x56: result = check_packet_answer(B588_CMD_0x56, sizeof(B588_CMD_56_ANSWER)); break;
    case B588_CMD_0x57: result = check_packet_answer(B588_CMD_0x57, sizeof(B588_CMD_57_ANSWER)); break;
    case B588_CMD_0x58: result = check_packet_answer(B588_CMD_0x58, sizeof(B588_CMD_58_ANSWER)); break;
    case B588_CMD_0x59: result = check_packet_answer(B588_CMD_0x59, sizeof(B588_CMD_59_ANSWER)); break;
    case B588_CMD_0x5A: result = check_packet_answer(B588_CMD_0x5A, sizeof(B588_CMD_5A_ANSWER)); break;
    case B588_CMD_0x5B: result = check_packet_answer(B588_CMD_0x5B, sizeof(B588_CMD_5B_ANSWER)); break;
    case B588_CMD_0x5C: result = check_packet_answer(B588_CMD_0x5C, sizeof(B588_CMD_5C_ANSWER)); break;
    case B588_CMD_0x5D: result = check_packet_answer(B588_CMD_0x5D, sizeof(B588_CMD_5D_ANSWER)); break;
    case B588_CMD_0x5E: result = check_packet_answer(B588_CMD_0x5E, sizeof(B588_CMD_5E_ANSWER)); break;
    case B588_CMD_0x5F: result = check_packet_answer(B588_CMD_0x5F, sizeof(B588_CMD_5F_ANSWER)); break;
    case B588_CMD_0x60: result = check_packet_answer(B588_CMD_0x60, sizeof(B588_CMD_60_ANSWER)); break;
    case B588_CMD_0x63: result = check_packet_answer(B588_CMD_0x63, sizeof(B588_CMD_63_ANSWER)); break;
    case B588_CMD_0x64: result = check_packet_answer(B588_CMD_0x64, sizeof(B588_CMD_64_ANSWER)); break;
    case B588_CMD_0x65: result = check_packet_answer(B588_CMD_0x65, sizeof(B588_CMD_65_ANSWER)); break;
    case B588_CMD_0x66: result = check_packet_answer(B588_CMD_0x66, sizeof(B588_CMD_66_ANSWER)); break;
    case B588_CMD_0x67: result = check_packet_answer(B588_CMD_0x67, sizeof(B588_CMD_67_ANSWER)); break;
    case B588_CMD_0x68: result = check_packet_answer(B588_CMD_0x68, sizeof(B588_CMD_68_ANSWER)); break;
    case B588_CMD_0x69: result = check_packet_answer(B588_CMD_0x69, sizeof(B588_CMD_69_ANSWER)); break;
    case B588_CMD_0x6A: result = check_packet_answer(B588_CMD_0x6A, sizeof(B588_CMD_6A_ANSWER)); break;
    case B588_CMD_0x6B: result = check_packet_answer(B588_CMD_0x6B, sizeof(B588_CMD_6B_ANSWER)); break;
    case B588_CMD_0x6D: result = check_packet_answer(B588_CMD_0x6D, sizeof(B588_CMD_6D_ANSWER)); break;
    case B588_CMD_0x6E: result = check_packet_answer(B588_CMD_0x6E, sizeof(B588_CMD_6E_ANSWER)); break;
    case B588_CMD_0x6F: result = check_packet_answer(B588_CMD_0x6F, sizeof(B588_CMD_6F_ANSWER)); break;
    case B588_CMD_0x71: result = check_packet_answer(B588_CMD_0x71, sizeof(B588_CMD_71_ANSWER)); break;
    case B588_CMD_0x72: result = check_packet_answer(B588_CMD_0x72, sizeof(B588_CMD_72_ANSWER)); break;
    case B588_CMD_0x73: result = check_packet_answer(B588_CMD_0x73, sizeof(B588_CMD_73_ANSWER)); break;
    case B588_CMD_0x75: result = check_packet_answer(B588_CMD_0x75, sizeof(B588_CMD_75_ANSWER)); break;

    default:
        result = PACKET_B588_FAIL;
        break;

    }

    return result;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::rc_on(void)
{
    int err = send_cmd_0x6A();
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::rc_off(void)
{
    int err = send_cmd_0x6B();
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::corridor_on(void)
{
    int err = send_cmd_0x44();
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::corridor_off(void)
{
    int err = send_cmd_0x43();
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Powersupply_B588::close_connect(void)
{
    if(serial.isOpen()) serial.close();
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_temperature(unsigned char *temp)
{
    unsigned short in_voltage = 0;
    unsigned short in_current = 0;
    unsigned short out_voltage = 0;
    unsigned short out_current = 0;
    unsigned char  powersupply_temp = 0;
    unsigned char  active_profile = 0;
    unsigned char  current_point = 0;

    set_ignore_bad_cmd(true);
    int err = send_cmd_0x47(&in_voltage,
                            &in_current,
                            &out_voltage,
                            &out_current,
                            &powersupply_temp,
                            &current_point,
                            &active_profile);
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    *temp = powersupply_temp;
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_accurate_temperature(double *temp)
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
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        return false;
    }
    *temp = (float)((float)temperature / (float)16.0f - 200.0f) / (float)4.0f;
    return true;
}
//--------------------------------------------------------------------------------
void Powersupply_B588::set_state_silence(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::set_UI(unsigned int voltage_mV,
                              unsigned int current_mA)
{
    unsigned short r_voltage = 0;
    unsigned short r_current = 0;
    int err = send_cmd_0x49(voltage_mV,
                            current_mA,
                            &r_voltage,
                            &r_current);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::set_UI_parrot(unsigned short voltage,
                                     unsigned int current)
{
    int err = send_cmd_0x57(voltage,
                            current);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::set_vent_speed(unsigned int speed)
{
    unsigned short r_speed_cooler = 0;
    int err = send_cmd_0x67(speed, &r_speed_cooler);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::set_PWM(uint16_t PWM_value)
{
    unsigned short r_data_PWM = 0;
    int err = send_cmd_0x68(PWM_value, &r_data_PWM);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_ADC(uint32_t *U_parrot,
                               uint32_t *I_parrot,
                               uint32_t *U_input_parrot)
{
    int err = send_cmd_0x5B(U_parrot,
                            I_parrot,
                            U_input_parrot);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_UI(unsigned short *in_voltage,
                              unsigned short *in_current,
                              unsigned short *out_voltage,
                              unsigned short *out_current,
                              unsigned char  *temp,
                              unsigned char  *current_point,
                              unsigned char  *active_profile)
{
    emit info(B588_CMD_0x47_TEXT);
    int err = send_cmd_0x47(in_voltage,
                            in_current,
                            out_voltage,
                            out_current,
                            temp,
                            current_point,
                            active_profile);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_DAC_ADC(unsigned short *setting_U,
                                   unsigned int   *setting_I,
                                   unsigned short *current_U,
                                   unsigned int   *current_I,
                                   unsigned char  *temperature,
                                   unsigned char  *active_profile,
                                   unsigned char  *point)
{
    int err = send_cmd_0x5A(setting_U,
                            setting_I,
                            current_U,
                            current_I,
                            temperature,
                            active_profile,
                            point);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_adv_ADC(uint32_t *U_parrot,
                                   uint32_t *I_parrot,
                                   uint32_t *U_input_parrot)
{
    int err = send_cmd_0x5B(U_parrot,
                            I_parrot,
                            U_input_parrot);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::run_calibration(char code)
{
    int err = send_cmd_0x64(code);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_current_calibration_point(char *point)
{
    int err = send_cmd_0x65(point);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_calibration_point_U(uint8_t number_point,
                                               uint16_t *value)
{
    int err = send_cmd_0x66(0,
                            number_point,
                            value);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_calibration_point_I(uint8_t number_point,
                                               uint16_t *value)
{
    int err = send_cmd_0x66(1,
                            number_point,
                            value);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::set_calibration_point_U(uint8_t number_point,
                                               uint16_t value)
{
    unsigned char  r_number_point = 0;
    unsigned short r_value = 0;
    int err = send_cmd_0x63(0,
                            number_point,
                            value,
                            &r_number_point,
                            &r_value);
    if(err != E_B588_NO_ERROR) return false;
    if(r_number_point != number_point) return false;
    if(r_value != value) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::set_calibration_point_I(uint8_t number_point,
                                               uint16_t value)
{
    unsigned char  r_number_point = 0;
    unsigned short r_value = 0;
    int err = send_cmd_0x63(1,
                            number_point,
                            value,
                            &r_number_point,
                            &r_value);
    if(err != E_B588_NO_ERROR) return false;
    if(r_number_point != number_point) return false;
    if(r_value != value) return false;

    return true;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x01(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x01_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_01_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x01;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_01_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf, sizeof(B588_CMD_01_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_01_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x01, sizeof(B588_CMD_01_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x02(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x02_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_02_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x02;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_02_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_02_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_02_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x02, sizeof(B588_CMD_02_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x03(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x03_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_03_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x03;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_03_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_03_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_03_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x03, sizeof(B588_CMD_03_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x04(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x04_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_04_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x04;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_04_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_04_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_04_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x04, sizeof(B588_CMD_04_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x05(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x05_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_05_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x05;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_05_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_05_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_05_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x05, sizeof(B588_CMD_05_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x06(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x06_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_06_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x06;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_06_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_06_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_06_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x06, sizeof(B588_CMD_06_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x07(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x07_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_07_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x07;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_07_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_07_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_07_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x07, sizeof(B588_CMD_07_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x08(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x08_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_08_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x08;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_08_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_08_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_08_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x08, sizeof(B588_CMD_08_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x09(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x09_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_09_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x09;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_09_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_09_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_09_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x09, sizeof(B588_CMD_09_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x0A(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x0A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_0A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x0A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_0A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_0A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_0A_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x0A, sizeof(B588_CMD_0A_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x0B(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x0B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_0B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x0B;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_0B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_0B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_0B_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x0B, sizeof(B588_CMD_0B_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x0C(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x0C_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_0C_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x0C;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_0C_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_0C_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_0C_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x0C, sizeof(B588_CMD_0C_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x0D(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x0D_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_0D_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x0D;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_0D_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_0D_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_0D_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x0D, sizeof(B588_CMD_0D_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x0E(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x0E_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_0E_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x0E;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_0E_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_0E_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_0E_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x0E, sizeof(B588_CMD_0E_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x0F(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x0F_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_0F_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x0F;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_0F_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_0F_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_0F_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x0F, sizeof(B588_CMD_0F_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x10(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x10_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_10_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x10;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_10_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_10_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_10_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x10, sizeof(B588_CMD_10_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x11(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x11_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_11_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x11;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_11_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_11_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_11_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x11, sizeof(B588_CMD_11_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x12(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x12_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_12_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x12;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_12_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_12_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_12_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x12, sizeof(B588_CMD_12_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x13(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x13_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_13_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x13;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_13_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_13_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_13_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x13, sizeof(B588_CMD_13_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x14(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x14_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_14_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x14;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_14_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_14_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_14_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x14, sizeof(B588_CMD_14_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x15(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x15_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_15_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x15;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_15_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_15_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_15_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x15, sizeof(B588_CMD_15_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x16(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x16_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_16_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x16;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_16_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_16_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_16_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x16, sizeof(B588_CMD_16_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x17(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x17_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_17_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x17;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_17_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_17_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_17_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x17, sizeof(B588_CMD_17_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x20(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x20_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_20_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x20;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_20_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_20_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_20_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x20, sizeof(B588_CMD_20_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x21(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x21_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_21_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x21;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_21_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_21_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_21_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x21, sizeof(B588_CMD_21_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x22(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x22_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_22_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x22;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_22_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_22_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_22_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x22, sizeof(B588_CMD_22_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x23(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x23_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_23_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x23;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_23_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_23_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_23_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x23, sizeof(B588_CMD_23_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x24(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x24_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_24_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x24;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_24_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_24_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_24_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x24, sizeof(B588_CMD_24_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x25(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x25_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_25_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x25;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_25_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_25_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_25_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x25, sizeof(B588_CMD_25_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x26(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x26_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_26_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x26;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_26_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_26_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_26_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x26, sizeof(B588_CMD_26_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x27(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x27_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_27_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x27;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_27_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_27_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_27_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x27, sizeof(B588_CMD_27_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x28(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x28_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_41_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x28;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_28_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_28_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_28_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x28, sizeof(B588_CMD_28_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x29(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x29_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_29_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x29;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_29_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_29_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_29_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x29, sizeof(B588_CMD_29_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x2A(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x2A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_2A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x2A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_2A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_2A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_2A_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x2A, sizeof(B588_CMD_2A_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x2B(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x2B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_2B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x2B;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_2B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_2B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_2B_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x2B, sizeof(B588_CMD_2B_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x2C(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x2C_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_2C_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x2C;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_2C_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_2C_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_2C_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x2C, sizeof(B588_CMD_2C_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x2D(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x2D_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_2D_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x2D;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_2D_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_2D_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_2D_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x2D, sizeof(B588_CMD_2D_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x2E(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x2E_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_2E_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x2E;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_2E_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_2E_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_2E_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x2E, sizeof(B588_CMD_2E_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x2F(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x2F_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_2F_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x2F;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_2F_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_2F_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_2F_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x2F, sizeof(B588_CMD_2F_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x30(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x30_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_30_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x30;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_30_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_30_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_30_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x30, sizeof(B588_CMD_30_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x31(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x31_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_31_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x31;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_31_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_31_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_31_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x31, sizeof(B588_CMD_31_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x32(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x32_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_32_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x32;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_32_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_32_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_32_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x32, sizeof(B588_CMD_32_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x33(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x33_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_33_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x33;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_33_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_33_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_33_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x33, sizeof(B588_CMD_33_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x34(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x34_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_34_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x34;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_34_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_34_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_34_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x34, sizeof(B588_CMD_34_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x35(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x35_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_35_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x35;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_35_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_35_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_35_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x35, sizeof(B588_CMD_35_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x36(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x36_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_36_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x36;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_36_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_36_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_36_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x36, sizeof(B588_CMD_36_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x37(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x37_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_37_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x37;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_37_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_37_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_37_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x37, sizeof(B588_CMD_37_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x38(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x38_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_38_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x38;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_38_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_38_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_38_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x38, sizeof(B588_CMD_38_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x39(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x39_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_39_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x39;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_39_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_39_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_39_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x39, sizeof(B588_CMD_39_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x3A(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x3A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_3A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x3A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_3A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_3A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_3A_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x3A, sizeof(B588_CMD_3A_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x3B(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x3B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_3B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x3B;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_3B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_3B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_3B_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x3B, sizeof(B588_CMD_3B_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x3C(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x3C_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_3C_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x3C;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_3C_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_3C_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_3C_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x3C, sizeof(B588_CMD_3C_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x3D(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x3D_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_3D_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x3D;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_3D_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_3D_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_3D_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x3D, sizeof(B588_CMD_3D_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x3E(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x3E_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_3E_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x3E;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_3E_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_3E_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_3E_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x3E, sizeof(B588_CMD_3E_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x3F(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x3F_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_3F_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x3F;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_3F_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_3F_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_3F_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x3F, sizeof(B588_CMD_3F_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x41(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x41_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_41_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x41;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_41_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_41_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_41_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x41, sizeof(B588_CMD_41_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x42(unsigned char code_speed)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x42_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_42_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x42;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.code_speed = code_speed;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_42_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_42_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_42_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x42, sizeof(B588_CMD_42_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x43(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x43_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_43_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x43;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_43_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_43_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_43_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x43, sizeof(B588_CMD_43_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x44(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x44_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_44_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x44;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_44_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_44_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_44_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x44, sizeof(B588_CMD_44_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::check_packet_answer(unsigned char cmd,
                                          int packet_len)
{
    if(data_powersupply.isEmpty())
    {
        last_error = E_B588_ERROR_NO_ANSWER;
        return last_error;
    }

    emit debug(data_powersupply.toHex());

    if(data_powersupply.length() < 2)
    {
        emit debug(data_powersupply.toHex());
        last_error = E_B588_ERROR_SMALL_ANSWER;
        return last_error;
    }

    unsigned char temp = data_powersupply.at(1) & 0x80;
    unsigned char data_cmd = data_powersupply.at(1) & 0x7F;
    if(data_cmd != cmd)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        return last_error;
    }
    if(!ignore_bad_cmd)
    {
        if(temp)
        {
            last_error = E_B588_ERROR_BAD_COMMAND;
            return last_error;
        }
    }

    //---
    unsigned short calc_crc16 = CRC::powersupply_crc16((unsigned char *)data_powersupply.constData(),
                                                       packet_len - 2);

    union U_SHORT_BYTES temp_crc;
    temp_crc.bytes.byte_0 = data_powersupply.at(packet_len - 2);
    temp_crc.bytes.byte_1 = data_powersupply.at(packet_len - 1);

    unsigned short data_crc16 = temp_crc.value;
    if(data_crc16 != calc_crc16)
    {
        emit error(QString("calc_crc16 0x%1").arg(calc_crc16, 0, 16));
        emit error(QString("data_crc16 0x%1").arg(data_crc16, 0, 16));
        last_error = E_B588_ERROR_BAD_CRC;
        return last_error;
    }
    //---

    if(data_powersupply.length() > packet_len)
    {
        emit debug(data_powersupply.toHex());
#ifdef QT_DEBUG
        qDebug() << "data_powersupply.length()" << data_powersupply.length() << "packet_len" << packet_len;
        qDebug() << "data" << data_powersupply.toHex();
#endif
        last_error = E_B588_ERROR_BIG_ANSWER;
        return last_error;
    }
    if(data_powersupply.length() < packet_len)
    {
        emit debug(data_powersupply.toHex());
#ifdef QT_DEBUG
        qDebug() << "data_powersupply.length()" << data_powersupply.length() << "packet_len" << packet_len;
        qDebug() << "data" << data_powersupply.toHex();
#endif
        last_error = E_B588_ERROR_SMALL_ANSWER;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x46(uint8_t  *type,
                                    uint16_t *year,
                                    uint8_t  *month,
                                    uint16_t *serno)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x46_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_46_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x46;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_46_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_46_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_46_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x46, sizeof(B588_CMD_46_ANSWER));
    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_46_ANSWER *answer = (B588_CMD_46_ANSWER *)data_powersupply.data();
    *type = answer->body.data.type;
    *year = answer->body.data.year;
    *month = answer->body.data.month;
    *serno = answer->body.data.serno;
    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x47(unsigned short *in_voltage,
                                    unsigned short *in_current,
                                    unsigned short *out_voltage,
                                    unsigned short *out_current,
                                    unsigned char  *temp,
                                    unsigned char  *current_point,
                                    unsigned char  *active_profile)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x47_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_47_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x47;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_47_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_47_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_47_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x47, sizeof(B588_CMD_47_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_47_ANSWER *answer = (B588_CMD_47_ANSWER *)data_powersupply.data();

    *in_voltage = answer->body.data.in_voltage;
    *in_current = answer->body.data.in_current;
    *out_voltage = answer->body.data.out_voltage;
    *out_current = answer->body.data.out_current;
    *temp = answer->body.data.temperature;
    *active_profile = answer->body.data.active_profile;
    *current_point = answer->body.data.current_point;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x48(unsigned char new_address_MODBUS,
                                    unsigned char *r_new_address_MODBUS)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x48_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_48_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x48;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.new_address_modbus = new_address_MODBUS;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_48_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_48_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_48_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x48, sizeof(B588_CMD_48_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_48_ANSWER *answer = (B588_CMD_48_ANSWER *)data_powersupply.data();
    *r_new_address_MODBUS = answer->body.data.new_address_modbus;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x49(unsigned int voltage_mV,
                                    unsigned int current_mA,
                                    unsigned short *r_voltage,
                                    unsigned short *r_current)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x49_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_49_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x49;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.current = current_mA;
    question.body.data.voltage = voltage_mV;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_49_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_49_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_49_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x49, sizeof(B588_CMD_49_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_49_ANSWER *answer = (B588_CMD_49_ANSWER *)data_powersupply.data();
    if(answer->body.data.error != 0)
    {
        U_BYTE error_byte;
        error_byte.value = answer->body.data.error;
        last_error = E_B588_NO_ERROR;
        if(error_byte.bites.bit0) last_error = E_B588_LO_VOLTAGE;
        if(error_byte.bites.bit1) last_error = E_B588_HI_VOLTAGE;
        if(error_byte.bites.bit2) last_error = E_B588_LO_CURRENT;
        if(error_byte.bites.bit3) last_error = E_B588_HI_CURRENT;
        if(error_byte.bites.bit4) last_error = E_B588_NO_REVERSE;
        if(error_byte.bites.bit5) last_error = E_B588_MAX_POWER;
        if(error_byte.bites.bit6) last_error = E_B588_CALIBRATION_RUNNING;
        if(error_byte.bites.bit7) last_error = E_B588_NO_RC;
        busy = false;
        return last_error;
    }
    *r_voltage = answer->body.data.voltage;
    *r_current = answer->body.data.current;

#ifdef NEEDED_PAUSE
    pause();
#endif
    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x4A(uint32_t *bits_error)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x4A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_4A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x4A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_4A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_4A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_4A_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x4A, sizeof(B588_CMD_4A_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_4A_ANSWER *answer = (B588_CMD_4A_ANSWER *)data_powersupply.data();
    U_INT error_int;
    error_int.value = answer->body.data.error_data;
    *bits_error = answer->body.data.error_data;
    if(error_int.bites.bit0)  emit error(ERROR_E00_B588_TEXT);
    if(error_int.bites.bit1)  emit error(ERROR_E01_B588_TEXT);
    if(error_int.bites.bit2)  emit error(ERROR_E02_B588_TEXT);
    if(error_int.bites.bit3)  emit error(ERROR_E03_B588_TEXT);
    if(error_int.bites.bit4)  emit error(ERROR_E04_B588_TEXT);
    if(error_int.bites.bit5)  emit error(ERROR_E05_B588_TEXT);
    if(error_int.bites.bit6)  emit error(ERROR_E06_B588_TEXT);
    if(error_int.bites.bit7)  emit error(ERROR_E07_B588_TEXT);

    if(error_int.bites.bit8)  emit error(ERROR_E08_B588_TEXT);
    if(error_int.bites.bit9)  emit error(ERROR_E09_B588_TEXT);
    if(error_int.bites.bit10) emit error(ERROR_E10_B588_TEXT);
    if(error_int.bites.bit11) emit error(ERROR_E11_B588_TEXT);

    if(error_int.bites.bit12) emit error(ERROR_E12_B588_TEXT);
    if(error_int.bites.bit13) emit error(ERROR_E13_B588_TEXT);
    if(error_int.bites.bit14) emit error(ERROR_E14_B588_TEXT);
    if(error_int.bites.bit15) emit error(ERROR_E15_B588_TEXT);
    if(error_int.bites.bit16) emit error(ERROR_E16_B588_TEXT);
    if(error_int.bites.bit17) emit error(ERROR_E17_B588_TEXT);
    if(error_int.bites.bit18) emit error(ERROR_E18_B588_TEXT);
    if(error_int.bites.bit19) emit error(ERROR_E19_B588_TEXT);
    if(error_int.bites.bit20) emit error(ERROR_E20_B588_TEXT);
    if(error_int.bites.bit21) emit error(ERROR_E21_B588_TEXT);
    if(error_int.bites.bit24) emit error(ERROR_E24_B588_TEXT);
    if(error_int.bites.bit25) emit error(ERROR_E25_B588_TEXT);
    if(error_int.bites.bit26) emit error(ERROR_E26_B588_TEXT);
    if(error_int.bites.bit27) emit error(ERROR_E27_B588_TEXT);
    if(error_int.bites.bit28) emit error(ERROR_E28_B588_TEXT);
    if(error_int.bites.bit29) emit error(ERROR_E29_B588_TEXT);
    if(error_int.bites.bit30) emit error(ERROR_E30_B588_TEXT);
    if(error_int.bites.bit31) emit error(ERROR_E31_B588_TEXT);

#if 1
    switch(error_int.bites.control)
    {
    case 0: emit error(tr("дистанционное управление: клавиатура")); break;
    case 1: emit error(tr("дистанционное управление: RS232"));      break;
    case 2: emit error(tr("дистанционное управление: USB"));        break;
    case 3: emit error(tr("дистанционное управление: GPIB"));       break;
    }
#endif

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::set_data_32(QStringList text,
                                   uint32_t value,
                                   uint32_t *r_value)
{
    int num = 32;
    QDialog *dialog = new QDialog();
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(0);
    QCheckBox *bit[num];

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    for(int n=0; n<num; n++)
    {
        bit[n] = new QCheckBox();
        bit[n]->setChecked((value >> n) & 1);
        if(text.length() > n) bit[n]->setText(text.at(n));
        vbox->addWidget(bit[n]);
    }
    vbox->addWidget(btn_box);

    dialog->setLayout(vbox);
    int btn = dialog->exec();
    if(btn != QDialog::Accepted) return false;

    //---
    uint32_t temp = 0;
    for(int n=0; n<num; n++)
    {
        temp |= (bit[n]->isChecked() << n);
    }
    emit info(QString("value %1").arg(temp));
    *r_value = temp;
    return true;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x54(unsigned char profile,
                                    unsigned char *r_count_point,
                                    unsigned char *r_count_repeat_profile)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x54_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_54_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x54;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_54_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_54_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_54_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x54, sizeof(B588_CMD_54_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_54_ANSWER *answer = (B588_CMD_54_ANSWER *)data_powersupply.data();
    if(answer->body.data.error == 1)
    {
        last_error = E_B588_NUMBER_PROFILE_ERROR;
        busy = false;
        return last_error;
    }
    *r_count_point = answer->body.data.count_point;
    *r_count_repeat_profile = answer->body.data.count_repeat_profile;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x55(unsigned int *mototime_min)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x55_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_55_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x55;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_55_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_55_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_55_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x55, sizeof(B588_CMD_55_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_55_ANSWER *answer = (B588_CMD_55_ANSWER *)data_powersupply.data();
    *mototime_min = answer->body.data.mototime_min;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x56(uint8_t  profile,
                                    uint8_t  point,
                                    uint8_t  *r_profile,
                                    uint8_t  *r_point,
                                    uint16_t *r_voltage,
                                    uint16_t *r_current,
                                    uint16_t *r_time)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x56_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_56_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x56;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.data.point = point;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_56_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_56_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_56_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x56, sizeof(B588_CMD_56_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_56_ANSWER_ERROR *answer_error = (B588_CMD_56_ANSWER_ERROR *)data_powersupply.data();
    if(data_powersupply.length() == sizeof(B588_CMD_56_ANSWER_ERROR))
    {
        if(answer_error->body.data.error == 1)
        {
            last_error = E_B588_UNKNOWN_PROFILE;
            busy = false;
            return last_error;
        }
        if(answer_error->body.data.error == 2)
        {
            last_error = E_B588_UNKNOWN_POINT;
            busy = false;
            return last_error;
        }
        if(answer_error->body.data.error == 3)
        {
            last_error = E_B588_PROFILE_EMPTY;
            busy = false;
            return last_error;
        }
    }

    B588_CMD_56_ANSWER *answer = (B588_CMD_56_ANSWER *)data_powersupply.data();
    *r_profile = answer->body.data.profile;
    *r_point = answer->body.data.point;
    *r_voltage = answer->body.data.voltage;
    *r_current = answer->body.data.current;
    *r_time = answer->body.data.time;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x57(unsigned short U,
                                    short I)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x57_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }
    B588_CMD_57_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x57;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.U = U;
    question.body.data.I = I;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_57_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_57_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_57_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B588_CMD_0x57, sizeof(B588_CMD_57_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

#ifdef NEEDED_PAUSE
    pause();
#endif
    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x58(unsigned short U,
                                    short I,
                                    unsigned short *r_U,
                                    short          *r_I)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x58_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_58_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x58;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.U = U;
    question.body.data.I = I;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_58_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_58_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_58_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x58, sizeof(B588_CMD_58_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_58_ANSWER *answer = (B588_CMD_58_ANSWER *)data_powersupply.data();
    *r_U = answer->body.data.U;
    *r_I = answer->body.data.I;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x59(unsigned short U,
                                    short I,
                                    unsigned short *r_U,
                                    short          *r_I)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x59_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_59_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x59;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.U = U;
    question.body.data.I = I;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_59_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_59_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_59_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x59, sizeof(B588_CMD_59_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_59_ANSWER *answer = (B588_CMD_59_ANSWER *)data_powersupply.data();
    *r_U = answer->body.data.U;
    *r_I = answer->body.data.I;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x5A(unsigned short *setting_U,
                                    unsigned int   *setting_I,
                                    unsigned short *current_U,
                                    unsigned int   *current_I,
                                    unsigned char  *temperature,
                                    unsigned char  *active_profile,
                                    unsigned char  *point)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x5A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_5A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x5A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_5A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_5A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_5A_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B588_CMD_0x5A, sizeof(B588_CMD_5A_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    B588_CMD_5A_ANSWER *answer = (B588_CMD_5A_ANSWER *)data_powersupply.data();
    *setting_U = answer->body.data.setting_DAC_U;
    *setting_I = answer->body.data.setting_DAC_I;
    *current_U = answer->body.data.current_ADC_U;
    *current_I = answer->body.data.current_ADC_I;
    *temperature = answer->body.data.temperature;
    *active_profile = answer->body.data.active_profile;
    *point = answer->body.data.active_point;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x5B(uint32_t *U_parrot,
                                    uint32_t *I_parrot,
                                    uint32_t *U_input_parrot)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x5B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_5B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x5B;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_5B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_5B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_5B_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B588_CMD_0x5B, sizeof(B588_CMD_5B_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    B588_CMD_5B_ANSWER *answer = (B588_CMD_5B_ANSWER *)data_powersupply.data();
    *U_parrot = answer->body.data.ADC_U;
    *I_parrot = answer->body.data.ADC_I;
    *U_input_parrot = answer->body.data.ADC_U_input;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x5C(int32_t Value_Zero_ADC_U,
                                    int32_t Value_delta_ADC_U,
                                    uint16_t Value_DAC_MIN,
                                    uint16_t Value_DAC_MAX,
                                    int32_t *r_Value_Zero_ADC_U,
                                    int32_t *r_Value_delta_ADC_U,
                                    uint16_t *r_Value_DAC_MIN,
                                    uint16_t *r_Value_DAC_MAX)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x5C_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_5C_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x5C;
    question.body.header.count_data = sizeof(question.body.data);

    question.body.data.Value_Zero_ADC_U  = Value_Zero_ADC_U;
    question.body.data.Value_delta_ADC_U = Value_delta_ADC_U;
    question.body.data.Value_DAC_MIN     = Value_DAC_MIN;
    question.body.data.Value_DAC_MAX     = Value_DAC_MAX;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_5C_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf, sizeof(B588_CMD_5C_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_5C_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x5C, sizeof(B588_CMD_5C_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_5C_ANSWER *answer = (B588_CMD_5C_ANSWER *)data_powersupply.data();

    *r_Value_Zero_ADC_U  = answer->body.data.Value_Zero_ADC_U;
    *r_Value_delta_ADC_U = answer->body.data.Value_delta_ADC_U;
    *r_Value_DAC_MIN     = answer->body.data.Value_DAC_MIN;
    *r_Value_DAC_MAX     = answer->body.data.Value_DAC_MAX;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x5D(int32_t Value_Zero_ADC_I,
                                    int32_t Value_delta_ADC_I,
                                    uint16_t Value_DAC_MIN,
                                    uint16_t Value_DAC_MAX,
                                    int32_t *r_Value_Zero_ADC_I,
                                    int32_t *r_Value_delta_ADC_I,
                                    uint16_t *r_Value_DAC_MIN,
                                    uint16_t *r_Value_DAC_MAX)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x5D_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_5D_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x5D;
    question.body.header.count_data = sizeof(question.body.data);

    question.body.data.Value_Zero_ADC_I  = Value_Zero_ADC_I;
    question.body.data.Value_delta_ADC_I = Value_delta_ADC_I;
    question.body.data.Value_DAC_MIN     = Value_DAC_MIN;
    question.body.data.Value_DAC_MAX     = Value_DAC_MAX;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_5D_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_5D_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_5D_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x5D, sizeof(B588_CMD_5D_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_5D_ANSWER *answer = (B588_CMD_5D_ANSWER *)data_powersupply.data();

    *r_Value_Zero_ADC_I  = answer->body.data.Value_Zero_ADC_I;
    *r_Value_delta_ADC_I = answer->body.data.Value_delta_ADC_I;
    *r_Value_DAC_MIN     = answer->body.data.Value_DAC_MIN;
    *r_Value_DAC_MAX     = answer->body.data.Value_DAC_MAX;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x5E(unsigned char profile,
                                    unsigned char point,
                                    unsigned short voltage_mV,
                                    unsigned int   current_mA,
                                    unsigned short time,
                                    unsigned short *r_error,
                                    unsigned char  *r_profile,
                                    unsigned char  *r_point,
                                    unsigned short *r_voltage,
                                    signed int     *r_current,
                                    unsigned short *r_time)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x5E_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_5E_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x5E;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.data.point = point;
    question.body.data.voltage = voltage_mV;
    question.body.data.current = current_mA;
    question.body.data.time = time;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_5E_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_5E_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_5E_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x5E, sizeof(B588_CMD_5E_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_5E_ANSWER *answer = (B588_CMD_5E_ANSWER *)data_powersupply.data();
    if(answer->body.data.error)
    {
        U_SHORT error_short;
        error_short.value = answer->body.data.error;
        last_error = E_B588_NO_ERROR;
        if(error_short.bites.bit0)  last_error = E_B588_LO_VOLTAGE;
        if(error_short.bites.bit1)  last_error = E_B588_HI_VOLTAGE;
        if(error_short.bites.bit2)  last_error = E_B588_LO_CURRENT;
        if(error_short.bites.bit3)  last_error = E_B588_HI_CURRENT;
        if(error_short.bites.bit4)  last_error = E_B588_NO_REVERSE;
        if(error_short.bites.bit5)  last_error = E_B588_MAX_POWER;
        if(error_short.bites.bit6)  last_error = E_B588_LO_TIME;
        if(error_short.bites.bit7)  last_error = E_B588_HI_TIME;
        if(error_short.bites.bit8)  last_error = E_B588_NUMBER_PROFILE_ERROR;
        if(error_short.bites.bit9)  last_error = E_B588_NUMBER_POINT_ERROR;
        if(error_short.bites.bit11) last_error = E_B588_ERROR_POINT;
        if(error_short.bites.bit12) last_error = E_B588_PROFILE_IS_RUN;
        if(error_short.bites.bit14) last_error = E_B588_CALIBRATION_RUNNING;
        if(error_short.bites.bit15) last_error = E_B588_NO_RC;
        busy = false;
        return last_error;
    }
    *r_error = answer->body.data.error;
    *r_profile = answer->body.data.profile;
    *r_point = answer->body.data.point;
    *r_voltage = answer->body.data.voltage;
    *r_current = answer->body.data.current;
    *r_time = answer->body.data.time;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x5F(unsigned char profile,
                                    unsigned char *number_profile)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x5F_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_5F_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x5F;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.number_profile = profile;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_5F_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_5F_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_5F_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x5F, sizeof(B588_CMD_5F_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_5F_ANSWER *answer = (B588_CMD_5F_ANSWER *)data_powersupply.data();
    if(answer->body.data.error)
    {
        last_error = E_B588_NUMBER_PROFILE_ERROR;
        busy = false;
        return last_error;
    }
    *number_profile = answer->body.data.number_profile;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x60(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x60_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_60_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x60;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_60_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_60_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_60_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x60, sizeof(B588_CMD_60_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x61(int32_t *Value_ADC_Zero_U,
                                    double  *factor_K)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x61_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_61_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x61;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_61_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_61_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_61_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x61, sizeof(B588_CMD_61_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_61_ANSWER *answer = (B588_CMD_61_ANSWER *)data_powersupply.data();

    *Value_ADC_Zero_U   = answer->body.data.Value_ADC_Zero_U;
    *factor_K = answer->body.data.factor_K;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x62(int32_t  *Value_ADC_Zero_I,
                                    double *factor_K)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x62_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_62_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x62;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_62_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_62_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_62_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x62, sizeof(B588_CMD_62_ANSWER));
    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_62_ANSWER *answer = (B588_CMD_62_ANSWER *)data_powersupply.data();

    *Value_ADC_Zero_I  = answer->body.data.Value_ADC_Zero_I;
    *factor_K = answer->body.data.factor_K;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x63(uint8_t  code,
                                    uint8_t  number_point,
                                    uint16_t data_point,
                                    uint8_t *r_number_point,
                                    uint16_t *r_data_point)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x63_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_63_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x63;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.type_calibration = code;
    question.body.data.number_point = number_point;
    question.body.data.data_point = data_point;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_63_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_63_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_63_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x63, sizeof(B588_CMD_63_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_63_ANSWER *answer = (B588_CMD_63_ANSWER *)data_powersupply.data();
    *r_number_point = answer->body.data.number_point;
    *r_data_point = answer->body.data.data_point;

    last_error = E_B588_NO_ERROR;
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
int Powersupply_B588::send_cmd_0x64(char code)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x64_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_64_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x64;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.code = code;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_64_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_64_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_64_ANSWER), 100, delay_ms);
#endif

    int res = check_packet_answer(B588_CMD_0x64, sizeof(B588_CMD_64_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }

    //B588_CMD_64_ANSWER *answer = (B588_CMD_64_ANSWER *)data_powersupply.data();
    //emit info(QString("code %1").arg(answer->code));

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x65(char *number_current_point)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x65_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_65_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x65;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_65_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_65_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_65_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x65, sizeof(B588_CMD_65_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_65_ANSWER *answer = (B588_CMD_65_ANSWER *)data_powersupply.data();
    *number_current_point = answer->body.data.number_current_point;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x66(uint16_t code,
                                    uint16_t index,
                                    uint16_t *value)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x66_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_66_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x66;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.code = code;
    question.body.data.index_point = index;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_66_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_66_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_66_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x66, sizeof(B588_CMD_66_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_66_ANSWER *answer = (B588_CMD_66_ANSWER *)data_powersupply.data();
    *value = answer->body.data.point_value;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x67(unsigned short speed_cooler,
                                    unsigned short *r_speed_cooler)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x67_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_67_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x67;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.speed_cooler = speed_cooler;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_67_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_67_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_67_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x67, sizeof(B588_CMD_67_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_67_ANSWER *answer = (B588_CMD_67_ANSWER *)data_powersupply.data();
    *r_speed_cooler = answer->body.data.speed_cooler;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x68(unsigned short data_PWM,
                                    unsigned short *r_data_PWM)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x68_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_68_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x68;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.data_PWM = data_PWM;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_68_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_68_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_68_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x68, sizeof(B588_CMD_68_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_68_ANSWER *answer = (B588_CMD_68_ANSWER *)data_powersupply.data();
    *r_data_PWM = answer->body.data.data_PWM;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x69(unsigned short *OCR3A,
                                    unsigned short *OCR3B)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x69_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_69_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x69;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_69_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_69_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_69_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x69, sizeof(B588_CMD_69_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_69_ANSWER *answer = (B588_CMD_69_ANSWER *)data_powersupply.data();

    *OCR3A = answer->body.data.OCR3A;
    *OCR3B = answer->body.data.OCR3B;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x6A(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x6A_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_6A_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x6A;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_6A_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_6A_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_6A_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x6A, sizeof(B588_CMD_6A_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_6A_ANSWER *answer = (B588_CMD_6A_ANSWER *)data_powersupply.data();
    U_BYTE data_byte;
    data_byte.value = answer->body.data.current_driver;
    if(data_byte.bites.bit0) emit info(tr("Захват произведен."));
    if(data_byte.bites.bit1) emit error(tr("Захват невозможен, т.к. ручное управление."));
    if(data_byte.bites.bit2) emit error(tr("Захват невозможен, т.к. RS232 сейчас используется."));
    if(data_byte.bites.bit4) emit error(tr("Захват невозможен, т.к. USB сейчас используется."));

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x6B(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x6B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_6B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x6B;
    question.body.header.count_data = 0;

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_6B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_6B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_6B_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x6B, sizeof(B588_CMD_6B_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_6B_ANSWER *answer = (B588_CMD_6B_ANSWER *)data_powersupply.data();
    U_BYTE data_byte;
    data_byte.value = answer->body.data.current_driver;
    if(data_byte.bites.bit0) emit info(tr("Снятие произведено."));
    if(data_byte.bites.bit1) emit error(tr("Снятие невозможено, т.к. ручное управление."));
    if(data_byte.bites.bit2) emit error(tr("Снятие невозможено, т.к. RS232 сейчас используется."));
    if(data_byte.bites.bit4) emit error(tr("Снятие невозможено, т.к. USB сейчас используется."));

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x6D(uint8_t secret_cmd,
                                    uint8_t input_data[128],
uint8_t *r_secret_cmd,
uint8_t *result,
QByteArray *output_data)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x6D_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_6D_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x6D;
    question.body.header.count_data = sizeof(question.body.data);

    question.body.data.secret_cmd = secret_cmd;
    question.body.data.signature.byte_0 = 0xAA;
    question.body.data.signature.byte_1 = 0xBB;
    question.body.data.signature.byte_2 = 0xCC;
    question.body.data.signature.byte_3 = 0xDD;
    question.body.data.signature.byte_4 = 0xEE;
    question.body.data.signature.byte_5 = 0x55;
    for(int n=0; n<128; n++)
    {
        question.body.data.secret_data[n] = input_data[n];
    }
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_6D_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_6D_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_6D_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x6D, sizeof(B588_CMD_6D_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_6D_ANSWER *answer = (B588_CMD_6D_ANSWER *)data_powersupply.data();
    *r_secret_cmd = answer->body.data.secret_cmd;
    *result = answer->body.data.result;
    for(int n=0; n<128; n++)
    {
        (*output_data).append(answer->body.data.secret_data[n]);
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x6C(unsigned int number_string)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x6C_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    if(number_string > 2)
    {
        last_error = E_B588_NUMBER_STRING_ERROR;
        busy = false;
        return last_error;
    }

    B588_CMD_6C_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x6C;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_6C_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_6C_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_6C_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x6C, sizeof(B588_CMD_6C_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    QString temp_data;
    temp_data.clear();

#if 0
    B588_CMD_6C_ANSWER_DATA11 *answer11 = (B588_CMD_6C_ANSWER_DATA11 *)data_powersupply.data();
    B588_CMD_6C_ANSWER_DATA8  *answer8 = (B588_CMD_6C_ANSWER_DATA8 *)data_powersupply.data();
    switch(number_string)
    {
    case 0:
        if(data_powersupply.length() != sizeof(B588_CMD_6C_ANSWER_DATA11))
        {
            last_error = E_B588_ERROR_BIG_ANSWER;
            busy = false;
            return last_error;
        }

        for(unsigned int n=0; n<sizeof(answer11->body.data); n++)
            temp_data.append(answer11->body.data.data[n]);
        emit info(QString("%1").arg(temp_data));
        break;

    case 1:
        if(data_powersupply.length() != sizeof(B588_CMD_6C_ANSWER_DATA8))
        {
            last_error = E_B588_ERROR_BIG_ANSWER;
            busy = false;
            return last_error;
        }

        for(unsigned int n=0; n<sizeof(answer8->body.data); n++)
            temp_data.append(answer8->body.data.data[n]);
        emit info(QString("%1").arg(temp_data));
        break;

    case 2:
        if(data_powersupply.length() != sizeof(B588_CMD_6C_ANSWER_DATA8))
        {
            last_error =  E_B588_ERROR_BIG_ANSWER;
            busy = false;
            return last_error;
        }

        B588_CMD_6C_ANSWER *answer = (B588_CMD_6C_ANSWER_DATA8 *)data_powersupply.data();
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

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x6E(unsigned char profile,
                                    unsigned char repeat,
                                    unsigned char *r_profile,
                                    unsigned char *r_repeat)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x6E_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_6E_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x6E;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.data.repeat = repeat;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_6E_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_6E_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_6E_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x6E, sizeof(B588_CMD_6E_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_6E_ANSWER *answer = (B588_CMD_6E_ANSWER *)data_powersupply.data();
    if(answer->body.data.error)
    {
        U_BYTE error_byte;
        error_byte.value = answer->body.data.error;
        last_error = E_B588_NO_ERROR;
        if(error_byte.bites.bit0) last_error = E_B588_NUMBER_PROFILE_ERROR;
        if(error_byte.bites.bit1) last_error = E_B588_NUMBER_REPEAT_ERROR;
        if(error_byte.bites.bit4) last_error = E_B588_PROFILE_IS_RUN;
        if(error_byte.bites.bit7) last_error = E_B588_NO_RC;
        busy = false;
        return last_error;
    }
    *r_profile = answer->body.data.profile;
    *r_repeat  = answer->body.data.repeat;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x6F(unsigned char profile,
                                    unsigned char *r_profile)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x6F_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_6F_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x6F;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.profile = profile;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_6F_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_6F_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_6F_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x6F, sizeof(B588_CMD_6F_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_6F_ANSWER *answer = (B588_CMD_6F_ANSWER *)data_powersupply.data();
    if(answer->body.data.error)
    {
        U_BYTE error_byte;
        error_byte.value = answer->body.data.error;
        last_error = E_B588_NO_ERROR;
        if(error_byte.bites.bit0) last_error = E_B588_NUMBER_PROFILE_ERROR;
        if(error_byte.bites.bit4) last_error = E_B588_PROFILE_IS_RUN;
        if(error_byte.bites.bit7) last_error = E_B588_NO_RC;
        busy = false;
        return last_error;
    }
    *r_profile = answer->body.data.profile;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x71(unsigned char *r_state_ADC_U,
                                    unsigned char *r_state_ADC_I,
                                    unsigned char *r_state_ADC_Ui)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x71_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_71_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x71;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_71_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_71_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_71_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x71, sizeof(B588_CMD_71_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_71_ANSWER *answer = (B588_CMD_71_ANSWER *)data_powersupply.data();
    *r_state_ADC_U = answer->body.data.state_ADC_U;
    *r_state_ADC_I = answer->body.data.state_ADC_I;
    *r_state_ADC_Ui = answer->body.data.state_ADC_Ui;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x72(unsigned short conf_ADC,
                                    unsigned short *r_conf_ADC)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x72_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_72_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x72;
    question.body.header.count_data = sizeof(question.body.data);
    question.body.data.conf_ADC = conf_ADC;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_72_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_72_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_72_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x72, sizeof(B588_CMD_72_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_72_ANSWER *answer = (B588_CMD_72_ANSWER *)data_powersupply.data();
    *r_conf_ADC = answer->body.data.conf_ADC;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x73(unsigned short *real_DAC_U,
                                    unsigned short *real_DAC_I,
                                    unsigned char  *revers,
                                    unsigned short *temperature)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x73_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_73_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x73;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_73_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_73_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_73_ANSWER), 73, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x73, sizeof(B588_CMD_73_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    if((unsigned int)data_powersupply.length() < sizeof(B588_CMD_73_ANSWER))
    {
        emit debug(data_powersupply.toHex());
        last_error = E_B588_ERROR_SMALL_ANSWER;
        return last_error;
    }

    if((unsigned int)data_powersupply.length() > sizeof(B588_CMD_73_ANSWER))
    {
        emit debug(data_powersupply.toHex());
        last_error = E_B588_ERROR_BIG_ANSWER;
        return last_error;
    }

    B588_CMD_73_ANSWER *answer = (B588_CMD_73_ANSWER *)data_powersupply.data();
    *real_DAC_U = answer->body.data.real_DAC_U;
    *real_DAC_I = answer->body.data.real_DAC_I;
    *revers = answer->body.data.revers;
    *temperature = answer->body.data.temperature;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x75(unsigned char  *count_ReStart_ADC_1,
                                    unsigned char  *count_ReStart_ADC_0,
                                    unsigned char  *count_ReStart_ADC_2,
                                    unsigned short *count_Error_Answer_ADC,
                                    unsigned int   *count_Error_ADC,
                                    unsigned short *MCUCSR)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }

    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x75_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    B588_CMD_75_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x75;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_75_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_75_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_75_ANSWER), 100, delay_ms);
#endif

    //---
    int res = check_packet_answer(B588_CMD_0x75, sizeof(B588_CMD_75_ANSWER));
    if(res == E_B588_ERROR_UNKNOWN_COMMAND)
    {
        last_error = E_B588_ERROR_UNKNOWN_COMMAND;
        busy = false;
        return last_error;
    }
    if(res == E_B588_ERROR_BAD_COMMAND)
    {
        last_error = E_B588_ERROR_BAD_COMMAND;
        busy = false;
        return last_error;
    }

    if(res != E_B588_NO_ERROR)
    {
        last_error = res;
        busy = false;
        return last_error;
    }
    //---

    B588_CMD_75_ANSWER *answer = (B588_CMD_75_ANSWER *)data_powersupply.data();
    *count_ReStart_ADC_1 = answer->body.data.count_ReStart_ADC_1;
    *count_ReStart_ADC_0 = answer->body.data.count_ReStart_ADC_0;
    *count_ReStart_ADC_2 = answer->body.data.count_ReStart_ADC_2;
    *count_Error_Answer_ADC = answer->body.data.count_Error_Answer_ADC;
    *count_Error_ADC = answer->body.data.count_Error_ADC;
    *MCUCSR = answer->body.data.MCUCSR;

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::send_cmd_0x7B(void)
{
    if(busy)
    {
        last_error = E_B588_BUSY;
        return last_error;
    }
    busy = true;

    if(is_silence == false)
    {
        emit info(B588_CMD_0x7B_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_B588_ERROR_PORT_NOT_OPEN;
        busy = false;
        return last_error;
    }

    union B588_CMD_7B_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = B588_CMD_0x7B;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf, sizeof(B588_CMD_7B_QUESTION) - 2);

    data_powersupply.clear();
    is_ready = false;
    serial.write((const char *)question.buf,sizeof(B588_CMD_7B_QUESTION));
#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(B588_CMD_7B_ANSWER), 100, delay_ms);
#endif

    int err = check_packet_answer(B588_CMD_0x7B, sizeof(B588_CMD_7B_ANSWER));
    if(err != E_B588_NO_ERROR)
    {
        last_error = err;
        busy = false;
        return last_error;
    }

    last_error = E_B588_NO_ERROR;
    busy = false;
    return last_error;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_information(uint8_t  *type,
                                       uint16_t *year,
                                       uint8_t  *month,
                                       uint16_t *serno)
{
    int err = send_cmd_0x46(type,
                            year,
                            month,
                            serno);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Powersupply_B588::wait(int time_msec)
{
    QTime time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > time_msec)
            break;
    }
}
//--------------------------------------------------------------------------------
void Powersupply_B588::wait(int max_len,
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
void Powersupply_B588::msgError(const QString &message)
{
    QMessageBox::critical(0, tr("Ошибка"), message);
}
//--------------------------------------------------------------------------------
void Powersupply_B588::set_address(int new_address)
{
    address = new_address;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::test_3_channel(int PWM,
                                      int set_voltage_mV,
                                      int get_voltage_mV,
                                      int *result_mV,
                                      int max_deviation_mV)
{
    bool ok = set_PWM(PWM);
    if(!ok)
    {
        print_last_error();
        return false;
    }

    ok = set_UI(set_voltage_mV, DEFAULT_I);
    if(!ok)
    {
        print_last_error();
        return false;
    }

#ifdef NEEDED_PAUSE
    Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

    uint32_t U = 0;
    uint32_t I = 0;
    uint32_t Ui = 0;
    ok = get_adv_ADC(&U, &I, &Ui);
    if(!ok)
    {
        print_last_error();
        return false;
    }

    //int result = (int)((Ui-26952)*0.00000462286873898f + 0.5f); //V
    int result = (int)((Ui-26952)*0.00462286873898f + 0.5f); //mV
    unsigned int delta = abs(result - get_voltage_mV);

    *result_mV = result;
    return !(abs(delta) > abs(max_deviation_mV));
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::test(void)
{
#if 0
    QStringList sl;
    sl << "Не используется";
    sl << "E1 - ошибка контрольной суммы FLASH платы индикации";
    sl << "E2 - нет ответа от контроллера платы преобразователя!";
    sl << "E3 - низкая температура (температура меньше 5 градусов) !";
    sl << "E4 - перегрев (температура больше 80 градусов) !";
    sl << "E5 - нет ответа АЦП (неответ 10 раз после 10 перезапусков каждые 160 мс) !";
    sl << "E6 - нет опорного напряжения на АЦП (5 раз проверки status) !";
    sl << "E7 - ошибка АЦП  (5 раз проверки status) !";
    sl << "E8 - ошибка контрольной суммы калибровочных данных в АЦП напряжения";
    sl << "E9 - ошибка контрольной суммы калибровочных данных в АЦП тока";
    sl << "E10 - ошибка контрольной суммы калибровочных данных в ЦАП напряжения";
    sl << "E11 - ошибка контрольной суммы калибровочных данных в ЦАП тока";
    sl << "E12 - неверна калибровка АЦП напряжения - выход за допустимые пределы";
    sl << "E13 - неверна калибровка АЦП тока - выход за допустимые пределы";
    sl << "E14 - неверна калибровка ЦАП напряжения - (следующее значение меньше предыдущего)";
    sl << "E15 - неверна калибровка ЦАП тока - (следующее значение меньше предыдущего)";
    sl << "E16 - нет заводской калибровки напряжения или она неверна";
    sl << "E17 - нет заводской калибровки тока или она неверна";
    sl << "E18 - нет ответа от FRAM (после 5 попыток)!";
    sl << "E19 - пропадание питания - после чтение функцией 0x46 - обнуляется";
    sl << "E20 - Установлен в 1, если присутствует реверс";
    sl << "E21 - Установлен в 1, если присутсвует плата интерфейса";
    sl << "Текущее управление (22) (01=RS-232 10=USB 11=GPIB)";
    sl << "Текущее управление (23)";
    sl << "E24 - попытка записи в исполняемый профиль";
    sl << "E25 - неверный номер профиля";
    sl << "E26 - профиль пуст";
    sl << "E27 - неверный номер точки";
    sl << "E28 - превышено максимальное значение напряжения";
    sl << "E29 - превышено максимальное значение напряжения тока";
    sl << "E30 - превышено максимальное значение напряжения времени";
    sl << "E31 - попытка установить неверную скорость обмена";

    uint32_t value = 0;
    bool ok = false;
    int err = 0;

    err = send_cmd_0x4A(&value);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return false;
    }
#endif

#if 0
    int err = send_cmd_0x4A();
    return (err == E_B588_NO_ERROR);
#endif

#if 0
    int err = 0;
    int32_t Value_Zero_ADC_U = 0;
    int32_t Value_delta_ADC_U = 0;
    uint16_t Value_DAC_MIN = 0;
    uint16_t Value_DAC_MAX = 0;

    int32_t r_Value_Zero_ADC_U = 0;
    int32_t r_Value_delta_ADC_U = 0;
    uint16_t r_Value_DAC_MIN = 0;
    uint16_t r_Value_DAC_MAX = 0;

    if(!rc_on())
    {
        return false;
    }

    err = send_cmd_0x5C(Value_Zero_ADC_U,
                        Value_delta_ADC_U,
                        Value_DAC_MIN,
                        Value_DAC_MAX,
                        &r_Value_Zero_ADC_U,
                        &r_Value_delta_ADC_U,
                        &r_Value_DAC_MIN,
                        &r_Value_DAC_MAX);
    if(err != E_B588_NO_ERROR)
    {
        return false;
    }

    err = send_cmd_0x61(&Value_Zero_ADC_U,
                        &Value_delta_ADC_U,
                        &Value_DAC_MIN,
                        &Value_DAC_MAX);
    if(err != E_B588_NO_ERROR)
    {
        return false;
    }

    if(!rc_off())
    {
        return false;
    }

    emit info(QString("Value_Zero_ADC_U %1").arg(Value_Zero_ADC_U));
    emit info(QString("Value_delta_ADC_U %1").arg(Value_delta_ADC_U));
    emit info(QString("Value_DAC_MIN %1").arg(Value_DAC_MIN));
    emit info(QString("Value_DAC_MAX %1").arg(Value_DAC_MAX));
    emit info("-----------");
    emit info(QString("r_Value_Zero_ADC_U %1").arg(r_Value_Zero_ADC_U));
    emit info(QString("r_Value_delta_ADC_U %1").arg(r_Value_delta_ADC_U));
    emit info(QString("r_Value_DAC_MIN %1").arg(r_Value_DAC_MIN));
    emit info(QString("r_Value_DAC_MAX %1").arg(r_Value_DAC_MAX));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Set_DAC_U_I(signed int U,signed int I)
{
    //0x57
    int err = send_cmd_0x57(U, I);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Set_Auto_Send_Power_Unit_Get_real_U(void)
{
    unsigned short setting_U = 0;
    unsigned int   setting_I = 0;
    unsigned short current_U = 0;
    unsigned int   current_I = 0;
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
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Set_Auto_Send_Power_Unit_Get_ADC_U(void)
{
    uint32_t U = 0;
    uint32_t I = 0;
    uint32_t U_input = 0;
    int err = send_cmd_0x5B(&U,
                            &I,
                            &U_input);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Save_Calib_Point_DAC(unsigned char number_point,
                                            unsigned short data_point)
{
    unsigned char  r_number_point = 0;
    unsigned short r_data_point = 0;
    int err = send_cmd_0x63(0,
                            number_point,
                            data_point,
                            &r_number_point,
                            &r_data_point);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Send_Command_To_Calib_ADC_U(void)
{
    char code = 1; //Запуск калибровки АЦП по напряжению
    int err = send_cmd_0x64(code);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Send_Command_To_Calib_ADC_I(void)
{
    char code = 2; //Запуск калибровки АЦП по току
    int err = send_cmd_0x64(code);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Send_Command_To_Calib_DAC_U(void)
{
    char code = 3; //Запуск калибровки ЦАП по напряжению
    int err = send_cmd_0x64(code);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Send_Command_To_Calib_DAC_I(void)
{
    char code = 4; //Запуск калибровки ЦАП по току
    int err = send_cmd_0x64(code);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Send_Command_To_Next_Point(void)
{
    char code = 5; //Переход к следующей точке - при калибровке АЦП
    int err = send_cmd_0x64(code);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Test_Calib_Point_DAC(unsigned char number_point)
{
    unsigned short value = 0;
    int err = send_cmd_0x66(0, number_point, &value);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Set_Auto_Send_Power_Unit_Get_Setting_DAC_U(void)
{
    unsigned short real_DAC_U = 0;
    unsigned short real_DAC_I = 0;
    unsigned char  revers = 0;
    unsigned short temperature = 0;
    int err = send_cmd_0x73(&real_DAC_U,
                            &real_DAC_I,
                            &revers,
                            &temperature);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::Set_Ventilator_MAX_Speed(unsigned short *r_speed_cooler)
{
    unsigned short speed_cooler = 1600;
    int err = send_cmd_0x67(speed_cooler,
                            r_speed_cooler);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog(int index)
{
    switch(index)
    {
    case B588_CMD_0x01: run_dialog_0x01(); break;
    case B588_CMD_0x02: run_dialog_0x02(); break;
    case B588_CMD_0x03: run_dialog_0x03(); break;
    case B588_CMD_0x04: run_dialog_0x04(); break;
    case B588_CMD_0x05: run_dialog_0x05(); break;
    case B588_CMD_0x06: run_dialog_0x06(); break;
    case B588_CMD_0x07: run_dialog_0x07(); break;
    case B588_CMD_0x08: run_dialog_0x08(); break;
    case B588_CMD_0x09: run_dialog_0x09(); break;
    case B588_CMD_0x0A: run_dialog_0x0A(); break;
    case B588_CMD_0x0B: run_dialog_0x0B(); break;
    case B588_CMD_0x0C: run_dialog_0x0C(); break;
    case B588_CMD_0x0D: run_dialog_0x0D(); break;
    case B588_CMD_0x0E: run_dialog_0x0E(); break;
    case B588_CMD_0x0F: run_dialog_0x0F(); break;

    case B588_CMD_0x10: run_dialog_0x10(); break;
    case B588_CMD_0x11: run_dialog_0x11(); break;
    case B588_CMD_0x12: run_dialog_0x12(); break;
    case B588_CMD_0x13: run_dialog_0x13(); break;
    case B588_CMD_0x14: run_dialog_0x14(); break;
    case B588_CMD_0x15: run_dialog_0x15(); break;
    case B588_CMD_0x16: run_dialog_0x16(); break;
    case B588_CMD_0x17: run_dialog_0x17(); break;

    case B588_CMD_0x20: run_dialog_0x20(); break;
    case B588_CMD_0x21: run_dialog_0x21(); break;
    case B588_CMD_0x22: run_dialog_0x22(); break;
    case B588_CMD_0x23: run_dialog_0x23(); break;
    case B588_CMD_0x24: run_dialog_0x24(); break;
    case B588_CMD_0x25: run_dialog_0x25(); break;
    case B588_CMD_0x26: run_dialog_0x26(); break;
    case B588_CMD_0x27: run_dialog_0x27(); break;
    case B588_CMD_0x28: run_dialog_0x28(); break;
    case B588_CMD_0x29: run_dialog_0x29(); break;
    case B588_CMD_0x2A: run_dialog_0x2A(); break;
    case B588_CMD_0x2B: run_dialog_0x2B(); break;
    case B588_CMD_0x2C: run_dialog_0x2C(); break;
    case B588_CMD_0x2D: run_dialog_0x2D(); break;
    case B588_CMD_0x2E: run_dialog_0x2E(); break;
    case B588_CMD_0x2F: run_dialog_0x2F(); break;

    case B588_CMD_0x30: run_dialog_0x30(); break;
    case B588_CMD_0x31: run_dialog_0x31(); break;
    case B588_CMD_0x32: run_dialog_0x32(); break;
    case B588_CMD_0x33: run_dialog_0x33(); break;
    case B588_CMD_0x34: run_dialog_0x34(); break;
    case B588_CMD_0x35: run_dialog_0x35(); break;
    case B588_CMD_0x36: run_dialog_0x36(); break;
    case B588_CMD_0x37: run_dialog_0x37(); break;
    case B588_CMD_0x38: run_dialog_0x38(); break;
    case B588_CMD_0x39: run_dialog_0x39(); break;
    case B588_CMD_0x3A: run_dialog_0x3A(); break;
    case B588_CMD_0x3B: run_dialog_0x3B(); break;
    case B588_CMD_0x3C: run_dialog_0x3C(); break;
    case B588_CMD_0x3D: run_dialog_0x3D(); break;
    case B588_CMD_0x3E: run_dialog_0x3E(); break;
    case B588_CMD_0x3F: run_dialog_0x3F(); break;

    case B588_CMD_0x41: run_dialog_0x41(); break;
    case B588_CMD_0x42: run_dialog_0x42(); break;
    case B588_CMD_0x43: run_dialog_0x43(); break;
    case B588_CMD_0x44: run_dialog_0x44(); break;
    case B588_CMD_0x46: run_dialog_0x46(); break;
    case B588_CMD_0x47: run_dialog_0x47(); break;
    case B588_CMD_0x48: run_dialog_0x48(); break;
    case B588_CMD_0x49: run_dialog_0x49(); break;
    case B588_CMD_0x4A: run_dialog_0x4A(); break;

    case B588_CMD_0x54: run_dialog_0x54(); break;
    case B588_CMD_0x55: run_dialog_0x55(); break;
    case B588_CMD_0x56: run_dialog_0x56(); break;
    case B588_CMD_0x57: run_dialog_0x57(); break;
    case B588_CMD_0x58: run_dialog_0x58(); break;
    case B588_CMD_0x59: run_dialog_0x59(); break;
    case B588_CMD_0x5A: run_dialog_0x5A(); break;
    case B588_CMD_0x5B: run_dialog_0x5B(); break;
    case B588_CMD_0x5C: run_dialog_0x5C(); break;
    case B588_CMD_0x5D: run_dialog_0x5D(); break;
    case B588_CMD_0x5E: run_dialog_0x5E(); break;
    case B588_CMD_0x5F: run_dialog_0x5F(); break;

    case B588_CMD_0x60: run_dialog_0x60(); break;
    case B588_CMD_0x61: run_dialog_0x61(); break;
    case B588_CMD_0x62: run_dialog_0x62(); break;
    case B588_CMD_0x63: run_dialog_0x63(); break;
    case B588_CMD_0x64: run_dialog_0x64(); break;
    case B588_CMD_0x65: run_dialog_0x65(); break;
    case B588_CMD_0x66: run_dialog_0x66(); break;
    case B588_CMD_0x67: run_dialog_0x67(); break;
    case B588_CMD_0x68: run_dialog_0x68(); break;
    case B588_CMD_0x69: run_dialog_0x69(); break;
    case B588_CMD_0x6A: run_dialog_0x6A(); break;
    case B588_CMD_0x6B: run_dialog_0x6B(); break;
    case B588_CMD_0x6C: run_dialog_0x6C(); break;
    case B588_CMD_0x6D: run_dialog_0x6D(); break;
    case B588_CMD_0x6E: run_dialog_0x6E(); break;
    case B588_CMD_0x6F: run_dialog_0x6F(); break;

    case B588_CMD_0x70: run_dialog_0x70(); break;
    case B588_CMD_0x71: run_dialog_0x71(); break;
    case B588_CMD_0x72: run_dialog_0x72(); break;
    case B588_CMD_0x73: run_dialog_0x73(); break;
    case B588_CMD_0x75: run_dialog_0x75(); break;
    case B588_CMD_0x7B: run_dialog_0x7B(); break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x01(void)
{
    int err = send_cmd_0x01();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x02(void)
{
    int err = send_cmd_0x02();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x03(void)
{
    int err = send_cmd_0x03();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x04(void)
{
    int err = send_cmd_0x04();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x05(void)
{
    int err = send_cmd_0x05();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x06(void)
{
    int err = send_cmd_0x06();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x07(void)
{
    int err = send_cmd_0x07();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x08(void)
{
    int err = send_cmd_0x08();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x09(void)
{
    int err = send_cmd_0x09();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x0A(void)
{
    int err = send_cmd_0x0A();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x0B(void)
{
    int err = send_cmd_0x0B();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x0C(void)
{
    int err = send_cmd_0x0C();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x0D(void)
{
    int err = send_cmd_0x0D();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x0E(void)
{
    int err = send_cmd_0x0E();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x0F(void)
{
    int err = send_cmd_0x0F();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x10(void)
{
    int err = send_cmd_0x10();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x11(void)
{
    int err = send_cmd_0x11();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x12(void)
{
    int err = send_cmd_0x12();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x13(void)
{
    int err = send_cmd_0x13();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x14(void)
{
    int err = send_cmd_0x14();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x15(void)
{
    int err = send_cmd_0x15();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x16(void)
{
    int err = send_cmd_0x16();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x17(void)
{
    int err = send_cmd_0x17();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x20(void)
{
    int err = send_cmd_0x20();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x21(void)
{
    int err = send_cmd_0x21();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x22(void)
{
    int err = send_cmd_0x22();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x23(void)
{
    int err = send_cmd_0x23();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x24(void)
{
    int err = send_cmd_0x24();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x25(void)
{
    int err = send_cmd_0x25();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x26(void)
{
    int err = send_cmd_0x26();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x27(void)
{
    int err = send_cmd_0x27();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x28(void)
{
    int err = send_cmd_0x28();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x29(void)
{
    int err = send_cmd_0x29();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x2A(void)
{
    int err = send_cmd_0x2A();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x2B(void)
{
    int err = send_cmd_0x2B();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x2C(void)
{
    int err = send_cmd_0x2C();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x2D(void)
{
    int err = send_cmd_0x2D();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x2E(void)
{
    int err = send_cmd_0x2E();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x2F(void)
{
    int err = send_cmd_0x2F();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x30(void)
{
    int err = send_cmd_0x30();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x31(void)
{
    int err = send_cmd_0x31();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x32(void)
{
    int err = send_cmd_0x32();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x33(void)
{
    int err = send_cmd_0x33();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x34(void)
{
    int err = send_cmd_0x34();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x35(void)
{
    int err = send_cmd_0x35();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x36(void)
{
    int err = send_cmd_0x36();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x37(void)
{
    int err = send_cmd_0x37();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x38(void)
{
    int err = send_cmd_0x38();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x39(void)
{
    int err = send_cmd_0x39();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x3A(void)
{
    int err = send_cmd_0x3A();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x3B(void)
{
    int err = send_cmd_0x3B();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x3C(void)
{
    int err = send_cmd_0x3C();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x3D(void)
{
    int err = send_cmd_0x3D();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x3E(void)
{
    int err = send_cmd_0x3E();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x3F(void)
{
    int err = send_cmd_0x3F();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x41(void)
{
    int err = send_cmd_0x41();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x42(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x42_TEXT);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x43(void)
{
    int err = send_cmd_0x43();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x44(void)
{
    int err = send_cmd_0x44();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x46(void)
{
    uint8_t  type  = 0;
    uint16_t year  = 0;
    uint8_t  month = 0;
    uint16_t serno = 0;
    int err = send_cmd_0x46(&type, &year, &month, &serno);
    if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x47(void)
{
    unsigned short in_voltage = 0;
    unsigned short in_current = 0;
    unsigned short out_voltage = 0;
    unsigned short out_current = 0;
    unsigned char  temp = 0;
    unsigned char  active_profile = 0;
    unsigned char  current_point = 0;
    int err = send_cmd_0x47(&in_voltage,
                            &in_current,
                            &out_voltage,
                            &out_current,
                            &temp,
                            &current_point,
                            &active_profile);
    if(err != E_B588_NO_ERROR)
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
    emit info(QString(tr("current_point: %1")).arg(current_point));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x48(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x48_TEXT);

    QSpinBox *sb_address = new QSpinBox();
    sb_address->setMinimum(MIN_ADDRESS_B588);
    sb_address->setMaximum(MAX_ADDRESS_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x49(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x49_TEXT);

    QSpinBox *sb_voltage = new QSpinBox();
    sb_voltage->setMinimum(MIN_VOLTAGE_B588);
    sb_voltage->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *sb_current = new QSpinBox();
    sb_current->setMinimum(MIN_CURRENT_B588);
    sb_current->setMaximum(MAX_CURRENT_B588);

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
        unsigned short r_voltage = 0;
        unsigned short r_current = 0;
        int err = send_cmd_0x49(sb_voltage->value(),
                                sb_current->value(),
                                &r_voltage,
                                &r_current);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_voltage %1").arg(r_voltage));
        emit info(QString("r_current %1").arg(r_current));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x4A(void)
{
    uint32_t bits_error = 0;
    int err = send_cmd_0x4A(&bits_error);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("bits_error 0x%1").arg(bits_error, 0, 16));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x54(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x54_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B588);
    sb_profile->setMaximum(MAX_PROFILE_B588);

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
        unsigned char r_profile = 0;
        unsigned char r_count_point = 0;
        unsigned char r_count_repeat_profile = 0;
        int err = send_cmd_0x54(sb_profile->value(),
                                &r_count_point,
                                &r_count_repeat_profile);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_profile %1").arg(r_profile));
        emit info(QString("r_count_point %1").arg(r_count_point));
        emit info(QString("r_count_repeat_profile %1").arg(r_count_repeat_profile));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x55(void)
{
    unsigned int mototime_min = 0;
    int err = send_cmd_0x55(&mototime_min);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("mototime_min %1").arg(mototime_min));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x56(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x56_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B588);
    sb_profile->setMaximum(MAX_PROFILE_B588);

    QSpinBox *sb_point = new QSpinBox();
    sb_point->setMinimum(MIN_POINT_B588);
    sb_point->setMaximum(MAX_POINT_B588);

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

    uint8_t  profile = 0;
    uint8_t  point = 0;
    uint16_t voltage = 0;
    uint16_t current = 0;
    uint16_t time = 0;

    int btn = dialog->exec();
    if(btn == QDialog::Accepted)
    {
        int err = send_cmd_0x56(sb_profile->value(),
                                sb_point->value(),
                                &profile,
                                &point,
                                &voltage,
                                &current,
                                &time);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
    }
    dialog->deleteLater();
    emit info(QString("profile %1").arg(profile));
    emit info(QString("point %1").arg(point));
    emit info(QString("voltage %1").arg(voltage));
    emit info(QString("current %1").arg(current));
    emit info(QString("time %1").arg(time));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x57(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x57_TEXT);

    QSpinBox *sb_U = new QSpinBox();
    sb_U->setMinimum(MIN_VOLTAGE_B588);
    sb_U->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *sb_I = new QSpinBox();
    sb_I->setMinimum(MIN_CURRENT_B588);
    sb_I->setMaximum(MAX_CURRENT_B588);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Напряжение")),   0, 0);
    grid->addWidget(sb_U,                           0, 1);
    grid->addWidget(new QLabel(tr("Ток")),          1, 0);
    grid->addWidget(sb_I,                           1, 1);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x58(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x58_TEXT);

    QSpinBox *sb_U = new QSpinBox();
    sb_U->setMinimum(MIN_VOLTAGE_B588);
    sb_U->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *sb_I = new QSpinBox();
    sb_I->setMinimum(MIN_CURRENT_B588);
    sb_I->setMaximum(MAX_CURRENT_B588);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Напряжение")),   0, 0);
    grid->addWidget(sb_U,                           0, 1);
    grid->addWidget(new QLabel(tr("Ток")),          1, 0);
    grid->addWidget(sb_I,                           1, 1);

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
        unsigned short r_U = 0;
        short          r_I = 0;
        int err = send_cmd_0x58(sb_U->value(),
                                sb_I->value(),
                                &r_U,
                                &r_I);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_U %1").arg(r_U));
        emit info(QString("r_I %1").arg(r_I));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x59(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x59_TEXT);

    QSpinBox *sb_U = new QSpinBox();
    sb_U->setMinimum(MIN_VOLTAGE_B588);
    sb_U->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *sb_I = new QSpinBox();
    sb_I->setMinimum(MIN_CURRENT_B588);
    sb_I->setMaximum(MAX_CURRENT_B588);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Напряжение")),   0, 0);
    grid->addWidget(sb_U,                           0, 1);
    grid->addWidget(new QLabel(tr("Ток")),          1, 0);
    grid->addWidget(sb_I,                           1, 1);

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
        unsigned short r_U = 0;
        short          r_I = 0;
        int err = send_cmd_0x59(sb_U->value(),
                                sb_I->value(),
                                &r_U,
                                &r_I);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_U %1").arg(r_U));
        emit info(QString("r_I %1").arg(r_I));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x5A(void)
{
    unsigned short  setting_U = 0;
    unsigned int    setting_I = 0;
    unsigned short  current_U = 0;
    unsigned int    current_I = 0;
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
    if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x5B(void)
{
    uint32_t U = 0;
    uint32_t I = 0;
    uint32_t U_input = 0;
    int err = send_cmd_0x5B(&U,
                            &I,
                            &U_input);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("U %1").arg(U));
    emit info(QString("I %1").arg(I));
    emit info(QString("U_input %1").arg(U_input));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x5C(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x5C_TEXT);

    QSpinBox *Value_Zero_ADC_U = new QSpinBox();
    Value_Zero_ADC_U->setMinimum(MIN_VOLTAGE_B588);
    Value_Zero_ADC_U->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *Value_delta_ADC_U = new QSpinBox();
    Value_delta_ADC_U->setMinimum(MIN_VOLTAGE_B588);
    Value_delta_ADC_U->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *Value_DAC_MIN = new QSpinBox();
    Value_DAC_MIN->setMinimum(MIN_VOLTAGE_B588);
    Value_DAC_MIN->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *Value_DAC_MAX = new QSpinBox();
    Value_DAC_MAX->setMinimum(MIN_VOLTAGE_B588);
    Value_DAC_MAX->setMaximum(MAX_VOLTAGE_B588);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Value_Zero_ADC_U")),  0, 0);
    grid->addWidget(Value_Zero_ADC_U,                    0, 1);
    grid->addWidget(new QLabel(tr("Value_delta_ADC_U")), 1, 0);
    grid->addWidget(Value_delta_ADC_U,                   1, 1);
    grid->addWidget(new QLabel(tr("Value_DAC_MIN")),     2, 0);
    grid->addWidget(Value_DAC_MIN,                       2, 1);
    grid->addWidget(new QLabel(tr("Value_DAC_MAX")),     3, 0);
    grid->addWidget(Value_DAC_MAX,                       3, 1);

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
        int32_t  r_Value_Zero_ADC_U = 0;
        int32_t  r_Value_delta_ADC_U = 0;
        uint16_t r_Value_DAC_MIN = 0;
        uint16_t r_Value_DAC_MAX = 0;
        int err = send_cmd_0x5C(Value_Zero_ADC_U->value(),
                                Value_delta_ADC_U->value(),
                                Value_DAC_MIN->value(),
                                Value_DAC_MAX->value(),
                                &r_Value_Zero_ADC_U,
                                &r_Value_delta_ADC_U,
                                &r_Value_DAC_MIN,
                                &r_Value_DAC_MAX);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_Value_Zero_ADC_U %1").arg(r_Value_Zero_ADC_U));
        emit info(QString("r_Value_delta_ADC_U %1").arg(r_Value_delta_ADC_U));
        emit info(QString("r_Value_DAC_MIN %1").arg(r_Value_DAC_MIN));
        emit info(QString("r_Value_DAC_MAX %1").arg(r_Value_DAC_MAX));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x5D(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x5D_TEXT);

    QSpinBox *Value_Zero_ADC_I = new QSpinBox();
    Value_Zero_ADC_I->setMinimum(MIN_CURRENT_B588);
    Value_Zero_ADC_I->setMaximum(MAX_CURRENT_B588);

    QSpinBox *Value_delta_ADC_I = new QSpinBox();
    Value_delta_ADC_I->setMinimum(MIN_CURRENT_B588);
    Value_delta_ADC_I->setMaximum(MAX_CURRENT_B588);

    QSpinBox *Value_DAC_MIN = new QSpinBox();
    Value_DAC_MIN->setMinimum(MIN_CURRENT_B588);
    Value_DAC_MIN->setMaximum(MAX_CURRENT_B588);

    QSpinBox *Value_DAC_MAX = new QSpinBox();
    Value_DAC_MAX->setMinimum(MIN_CURRENT_B588);
    Value_DAC_MAX->setMaximum(MAX_CURRENT_B588);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Value_Zero_ADC_I")),  0, 0);
    grid->addWidget(Value_Zero_ADC_I,                    0, 1);
    grid->addWidget(new QLabel(tr("Value_delta_ADC_I")), 1, 0);
    grid->addWidget(Value_delta_ADC_I,                   1, 1);
    grid->addWidget(new QLabel(tr("Value_DAC_MIN")),     2, 0);
    grid->addWidget(Value_DAC_MIN,                       2, 1);
    grid->addWidget(new QLabel(tr("Value_DAC_MAX")),     3, 0);
    grid->addWidget(Value_DAC_MAX,                       3, 1);

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
        int32_t  r_Value_Zero_ADC_I = 0;
        int32_t  r_Value_delta_ADC_I = 0;
        uint16_t r_Value_DAC_MIN = 0;
        uint16_t r_Value_DAC_MAX = 0;
        int err = send_cmd_0x5D(Value_Zero_ADC_I->value(),
                                Value_delta_ADC_I->value(),
                                Value_DAC_MIN->value(),
                                Value_DAC_MAX->value(),
                                &r_Value_Zero_ADC_I,
                                &r_Value_delta_ADC_I,
                                &r_Value_DAC_MIN,
                                &r_Value_DAC_MAX);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("r_Value_Zero_ADC_I %1").arg(r_Value_Zero_ADC_I));
        emit info(QString("r_Value_delta_ADC_I %1").arg(r_Value_delta_ADC_I));
        emit info(QString("r_Value_DAC_MIN %1").arg(r_Value_DAC_MIN));
        emit info(QString("r_Value_DAC_MAX %1").arg(r_Value_DAC_MAX));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x5E(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x5E_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B588);
    sb_profile->setMaximum(MAX_PROFILE_B588);

    QSpinBox *sb_point = new QSpinBox();
    sb_point->setMinimum(MIN_POINT_B588);
    sb_point->setMaximum(MAX_POINT_B588);

    QSpinBox *sb_voltage_mV = new QSpinBox();
    sb_voltage_mV->setMinimum(MIN_VOLTAGE_B588);
    sb_voltage_mV->setMaximum(MAX_VOLTAGE_B588);

    QSpinBox *sb_current_mA = new QSpinBox();
    sb_current_mA->setMinimum(MIN_CURRENT_B588);
    sb_current_mA->setMaximum(MAX_CURRENT_B588);

    QSpinBox *sb_time = new QSpinBox();
    sb_time->setMinimum(MIN_TIME_B588);
    sb_time->setMaximum(MAX_TIME_B588);


    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Профиль")),      0, 0);
    grid->addWidget(sb_profile,                     0, 1);
    grid->addWidget(new QLabel(tr("Точка")),        1, 0);
    grid->addWidget(sb_point,                       1, 1);
    grid->addWidget(new QLabel(tr("Напряжение")),   2, 0);
    grid->addWidget(sb_voltage_mV,                  2, 1);
    grid->addWidget(new QLabel(tr("Ток")),          3, 0);
    grid->addWidget(sb_current_mA,                  3, 1);
    grid->addWidget(new QLabel(tr("Время")),        4, 0);
    grid->addWidget(sb_time,                        4, 1);

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
        unsigned short r_voltage = 0;
        signed int     r_current = 0;
        unsigned short r_time = 0;
        int err = send_cmd_0x5E(sb_profile->value(),
                                sb_point->value(),
                                sb_voltage_mV->value(),
                                sb_current_mA->value(),
                                sb_time->value(),
                                &r_error,
                                &r_profile,
                                &r_point,
                                &r_voltage,
                                &r_current,
                                &r_time);
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x5F(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x5F_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B588);
    sb_profile->setMaximum(MAX_PROFILE_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x60(void)
{
    int err = send_cmd_0x60();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x61(void)
{
    int32_t Value_ADC_Zero_U = 0;
    double  factor_K = 0;

    int err = send_cmd_0x61(&Value_ADC_Zero_U,
                            &factor_K);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("Value_ADC_Zero_U %1").arg(Value_ADC_Zero_U));
    emit info(QString("factor_K %1").arg(factor_K));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x62(void)
{
    int32_t  Value_ADC_Zero_I = 0;
    double  factor_K = 0;

    int err = send_cmd_0x62(&Value_ADC_Zero_I,
                            &factor_K);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("Value_ADC_Zero_I %1").arg(Value_ADC_Zero_I));
    emit info(QString("factor_K %1").arg(factor_K));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x63(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x63_TEXT);

    QSpinBox *sb_number_point = new QSpinBox();
    sb_number_point->setMinimum(MIN_POINT_B588);
    sb_number_point->setMaximum(MAX_POINT_B588);

    QSpinBox *sb_data_point = new QSpinBox();
    sb_data_point->setMinimum(MIN_DATA_B588);
    sb_data_point->setMaximum(MAX_DATA_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x64(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x64_TEXT);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x65(void)
{
    char number_current_point = 0;
    int err = send_cmd_0x65(&number_current_point);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("number_current_point %1").arg((int)number_current_point));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x66(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x66_TEXT);

    QSpinBox *sb_index_point = new QSpinBox();
    sb_index_point->setMinimum(0);
    sb_index_point->setMaximum(MAX_CALIBRATION_POINTS_B588_I);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Index")), 0, 0);
    grid->addWidget(sb_index_point,          0, 1);

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
        unsigned short value_U = 0;

        int err = send_cmd_0x66(0,
                                sb_index_point->value(),
                                &value_U);
        if(err != E_B588_NO_ERROR)
        {
            print_last_error();
            dialog->deleteLater();
            return;
        }
        emit info(QString("index   = %1").arg(sb_index_point->value()));
        emit info(QString("value_U = %1").arg(value_U));
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x67(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x67_TEXT);

    QSpinBox *sb_speed_cooler = new QSpinBox();
    sb_speed_cooler->setMinimum(MIN_SPEED_B588);
    sb_speed_cooler->setMaximum(MAX_SPEED_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x68(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x68_TEXT);

    QSpinBox *sb_data_PWM = new QSpinBox();
    sb_data_PWM->setMinimum(MIN_PWM_B588);
    sb_data_PWM->setMaximum(MAX_PWM_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x69(void)
{
    unsigned short OCR3A = 0;
    unsigned short OCR3B = 0;
    int err = send_cmd_0x69(&OCR3A,
                            &OCR3B);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("OCR3A %1").arg(OCR3A));
    emit info(QString("OCR3B %1").arg(OCR3B));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x6A(void)
{
    int err = send_cmd_0x6A();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x6B(void)
{
    int err = send_cmd_0x6B();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x6C(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x6C_TEXT);

    QSpinBox *sb_number_string = new QSpinBox();
    sb_number_string->setMinimum(MIN_NUMBER_STRING_B588);
    sb_number_string->setMaximum(MAX_NUMBER_STRING_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x6D(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x6D_TEXT);

    QSpinBox *sb_ID = new QSpinBox();
    sb_ID->setMinimum(MIN_ID_B588);
    sb_ID->setMaximum(MAX_ID_B588);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dialog, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x6E(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x6E_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B588);
    sb_profile->setMaximum(MAX_PROFILE_B588);

    QSpinBox *sb_repeat = new QSpinBox();
    sb_repeat->setMinimum(MIN_REPEAT_B588);
    sb_repeat->setMaximum(MAX_REPEAT_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x6F(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x6F_TEXT);

    QSpinBox *sb_profile = new QSpinBox();
    sb_profile->setMinimum(MIN_PROFILE_B588);
    sb_profile->setMaximum(MAX_PROFILE_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x70(void)
{
    emit error("Только в тестовой версии АЦП");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x71(void)
{
    unsigned char r_state_ADC_U = 0;
    unsigned char r_state_ADC_I = 0;
    unsigned char r_state_ADC_Ui = 0;
    int err = send_cmd_0x71(&r_state_ADC_U,
                            &r_state_ADC_I,
                            &r_state_ADC_Ui);
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info(QString("state_ADC_U %1").arg(r_state_ADC_U));
    emit info(QString("state_ADC_I %1").arg(r_state_ADC_I));
    emit info(QString("state_ADC_Ui %1").arg(r_state_ADC_Ui));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Powersupply_B588::run_dialog_0x72(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(B588_CMD_0x72_TEXT);

    QSpinBox *sb_conf_ADC = new QSpinBox();
    sb_conf_ADC->setMinimum(MIN_CONF_ADC_B588);
    sb_conf_ADC->setMaximum(MAX_CONF_ADC_B588);

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
        if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x73(void)
{
    unsigned short real_DAC_U = 0;
    unsigned short real_DAC_I = 0;
    unsigned char  revers = 0;
    unsigned short temperature = 0;
    int err = send_cmd_0x73(&real_DAC_U,
                            &real_DAC_I,
                            &revers,
                            &temperature);
    if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x75(void)
{
    unsigned char  count_ReStart_ADC_1 = 0;
    unsigned char  count_ReStart_ADC_0 = 0;
    unsigned char  count_ReStart_ADC_2 = 0;
    unsigned short count_Error_Answer_ADC = 0;
    unsigned int   count_Error_ADC = 0;
    unsigned short MCUCSR = 0;

    int err = send_cmd_0x75(&count_ReStart_ADC_1,
                            &count_ReStart_ADC_0,
                            &count_ReStart_ADC_2,
                            &count_Error_Answer_ADC,
                            &count_Error_ADC,
                            &MCUCSR);
    if(err != E_B588_NO_ERROR)
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
void Powersupply_B588::run_dialog_0x7B(void)
{
    int err = send_cmd_0x7B();
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::fram_write(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::fram_write_8(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::fram_write_16(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::fram_write_32(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::fram_read(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
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
bool Powersupply_B588::fram_read_8(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
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
bool Powersupply_B588::fram_read_16(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
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
bool Powersupply_B588::fram_read_32(uint16_t fram_address,
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
    if(err != E_B588_NO_ERROR)
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
bool Powersupply_B588::get_ID(uint8_t *ID)
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
    if(err != E_B588_NO_ERROR)
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
bool Powersupply_B588::set_date_and_serno(uint16_t year,
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
    if(err != E_B588_NO_ERROR)
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
bool Powersupply_B588::copy_user_calibration_point_to_factory(void)
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
    if(err != E_B588_NO_ERROR)
    {
        print_last_error();
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_data_profile(int number_profile,
                                        unsigned char *count_point,
                                        unsigned char *count_repeat_profile)
{
    int err = send_cmd_0x54(number_profile, count_point, count_repeat_profile);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::get_point_profile(uint8_t  number_profile,
                                         uint8_t  number_point,
                                         uint8_t  *r_profile,
                                         uint8_t  *r_point,
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
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::add_point_profile(int number_profile,
                                         int number_point,
                                         unsigned short voltage,
                                         unsigned int   current,
                                         unsigned short time)
{
    unsigned short r_error = 0;
    unsigned char  r_profile = 0;
    unsigned char  r_point = 0;
    unsigned short r_voltage_mV = 0;
    signed int     r_current_mA = 0;
    unsigned short r_time = 0;
    int err = send_cmd_0x5E(number_profile,
                            number_point,
                            voltage,
                            current,
                            time,
                            &r_error,
                            &r_profile,
                            &r_point,
                            &r_voltage_mV,
                            &r_current_mA,
                            &r_time);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::run_profile(int number_profile)
{
    unsigned char r_profile = 0;
    int err = send_cmd_0x5F(number_profile, &r_profile);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::add_count_repeat_profile(int number_profile, int repeat_count)
{
    unsigned char r_profile = 0;
    unsigned char r_repeat = 0;
    int err = send_cmd_0x6E(number_profile,
                            repeat_count,
                            &r_profile,
                            &r_repeat);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Powersupply_B588::clear_profile(int number_profile)
{
    unsigned char r_profile = 0;
    int err = send_cmd_0x6F(number_profile, &r_profile);
    return (err == E_B588_NO_ERROR);
}
//--------------------------------------------------------------------------------
#ifdef NEEDED_PAUSE
void Powersupply_B588::pause(unsigned int msec)
{
#ifndef FAKE
    Sleeper::msleep(msec);
#endif
}
#endif
//--------------------------------------------------------------------------------
int Powersupply_B588::print_last_error(void)
{
    switch(last_error)
    {
    case E_B588_NO_ERROR:                   emit info(tr("ОК")); break;
    case E_B588_ERROR_INDEX_NOT_CORRECT:    emit error(tr("Индекс не корректен")); break;
    case E_B588_ERROR_PORT_NOT_OPEN:        emit error(tr("Порт источника питания Б5-88 не открыт!")); break;
    case E_B588_ERROR_BREAK:                emit error(tr("E_B588_ERROR_BREAK")); break;
    case E_B588_ERROR_MAX_REPEAT:           emit error(tr("E_B588_ERROR_MAX_REPEAT")); break;
    case E_B588_ERROR_NO_ANSWER:            emit error(QString(tr("%1 не отвечает")).arg(POWER_SUPPLY_NAME_B588)); break;
    case E_B588_ERROR_BIG_ANSWER:           emit error(tr("Неверная длина пакета (слишком большой)"));      break;
    case E_B588_ERROR_SMALL_ANSWER:         emit error(tr("Неверная длина пакета (слишком маленький)"));    break;
    case E_B588_ERROR_BAD_SERNO:            emit error(tr("Неверный серийный номер")); break;
    case E_B588_ERROR_BAD_ADDRESS:          emit error(tr("Неверный адрес")); break;
    case E_B588_ERROR_BAD_CRC:              emit error(tr("Ошибка контрольной суммы")); break;
    case E_B588_ERROR_BAD_COMMAND:          emit error(tr("Плохая команда")); break;
    case E_B588_ERROR_BAD_LEN:              emit error(tr("Неверная длина")); break;
    case E_B588_ERROR_UNKNOWN_COMMAND:      emit error(tr("Неизвестная команда")); break;
    case E_B588_INVALID_FLOAT:              emit error(tr("E_B588_INVALID_FLOAT")); break;
    case E_B588_INVALID_LIMIT:              emit error(tr("E_B588_INVALID_LIMIT")); break;
    case E_B588_INVALID_DELTA:              emit error(tr("E_B588_INVALID_DELTA")); break;
    case E_B588_BREAK_MEASURING:            emit error(tr("E_B588_BREAK_MEASURING")); break;
    case E_B588_UNKNOWN_PROFILE:            emit error(tr("Недопустимый номер профиля")); break;
    case E_B588_UNKNOWN_POINT:              emit error(tr("Недопустимый номер точки в профиле")); break;
    case E_B588_PROFILE_EMPTY:              emit error(tr("Профиль пуст")); break;
    case E_B588_ERROR_SIGNATURE:            emit error(tr("Не совпадает сигнатура")); break;
    case E_B588_NUMBER_PROFILE_ERROR:       emit error(tr("Недопустимый номер профиля.")); break;
    case E_B588_NUMBER_POINT_ERROR:         emit error(tr("Недопустимый номер точки.")); break;
    case E_B588_NUMBER_STRING_ERROR:        emit error(tr("Недопустимый номер строки.")); break;
    case E_B588_BUSY:                       emit error(tr("Прибор занят выполнением команды")); break;
    case E_B588_NUMBER_REPEAT_ERROR:        emit error(tr("Недопустимое количество повторов")); break;
    case E_B588_LO_VOLTAGE:                 emit error(tr("Значение напряжение ниже минимально допустимого.")); break;
    case E_B588_HI_VOLTAGE:                 emit error(tr("Превышено максимальное значение напряжения.")); break;
    case E_B588_LO_CURRENT:                 emit error(tr("Значение тока ниже минимально допустимого.")); break;
    case E_B588_HI_CURRENT:                 emit error(tr("Превышено максимальное значение тока.")); break;
    case E_B588_NO_REVERSE:                 emit error(tr("Нет платы реверса - попытка использовать реверс тока.")); break;
    case E_B588_MAX_POWER:                  emit error(tr("Превышена максимальная мощность источника.")); break;
    case E_B588_LO_TIME:                    emit error(tr("Значение времени ниже минимально допустимого.")); break;
    case E_B588_HI_TIME:                    emit error(tr("Превышено максимальное значение времени.")); break;
    case E_B588_ERROR_POINT:                emit error(tr("Неверный порядок точек при записи.")); break;
    case E_B588_PROFILE_IS_RUN:             emit error(tr("Попытка записи в исполняемый профиль.")); break;
    case E_B588_CALIBRATION_RUNNING:        emit error(tr("Производится процесс калибровки.")); break;
    case E_B588_NO_RC:                      emit error(tr("Нет захвата управления по данному интерфейсу.")); break;
    case E_B588_COMMAND_ON_CONSTRUCTION:    emit error(tr("Пока не сделано")); break;

    default:                                emit error(QString("Unknown error %1").arg(last_error)); break;
    }

    if(last_error != E_B588_NO_ERROR)
    {
        emit error(data_powersupply.toHex());
    }

    return last_error;
}
//--------------------------------------------------------------------------------
int Powersupply_B588::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
// тестовые функции
//--------------------------------------------------------------------------------
unsigned int Powersupply_B588::get_Correct_value_U(unsigned int new_setting_U)
{
    // Чтобы можно было выставить гарантированный нуль с запасом
    if (new_setting_U == 0)
    {
        return 0;
    }
    int index = (int)(new_setting_U / MAGIC_B588_U);  // Получаем индекс в таблице

    //
    uint16_t value = 0;
    uint16_t value2 = 0;
    uint16_t value3 = 0;
    bool ok = 0;
    ok = get_calibration_point_U(COUNT_B588_CALIB_POINTS_DAC_U-1, &value);
    if(!ok)
    {
        print_last_error();
    }
    ok = get_calibration_point_U(index, &value2);
    if(!ok)
    {
        print_last_error();
    }
    ok = get_calibration_point_U(index + 1, &value3);
    if(!ok)
    {
        print_last_error();
    }
    //

    signed long V1 = 0;
    if(index > (COUNT_B588_CALIB_POINTS_DAC_U - 1))
    {
        //float y = (float)MAX_VALUE_U - (float)correct_DAC_U[COUNT_B588_CALIB_POINTS_DAC_U-1];
        float y = (float)MAX_B588_VALUE_U - (float)value;
        float x = (float)MAGIC_B588_U;
        float m = (float)new_setting_U - (float)MAX_B588_VALUE_U + x;
        float n = (y * m) / x;
        //V1 = correct_DAC_U[COUNT_B588_CALIB_POINTS_DAC_U-1] + (signed long)n;
        V1 = value + (signed long)n;
        return abs(V1);
    }

    //---
    if(index == (COUNT_B588_CALIB_POINTS_DAC_U - 1))
    {
        //V1 = ((signed long)(new_setting_U % MAGIC_B588_U) * (MAX_B588_VALUE_U - correct_DAC_U[index])) / MAGIC_B588_U;
        V1 = ((signed long)(new_setting_U % MAGIC_B588_U) * (MAX_B588_VALUE_U - value2)) / MAGIC_B588_U;
        //V1 += correct_DAC_U[index];
        V1 += value2;
        return abs(V1);
    }

    //---
    //V1 = ((signed long)(new_setting_U % MAGIC_U) * (correct_DAC_U[index + 1] - correct_DAC_U[index])) / MAGIC_U;
    V1 = ((signed long)(new_setting_U % MAGIC_B588_U) * (value3 - value2)) / MAGIC_B588_U;
    //V1 += correct_DAC_U[index];
    V1 += value2;
    return abs(V1);
}
//=============================================================================
unsigned int Powersupply_B588::get_Correct_value_I(unsigned long new_setting_I)
{
    // Чтобы можно было выставить гарантированный нуль с запасом
    if (new_setting_I < MINIMUM_B588_I)
    {
        return 0;
    }

    new_setting_I -= MINIMUM_B588_I;  // Так как вся таблица смещена 10 мА
    int index = (int)(new_setting_I / MAGIC_B588_I);  // Получаем индекс в таблице

    //
    uint16_t value = 0;
    uint16_t value2 = 0;
    uint16_t value3 = 0;
    bool ok = 0;
    ok = get_calibration_point_I(COUNT_B588_CALIB_POINTS_DAC_I-1, &value);
    if(!ok)
    {
        print_last_error();
    }
    ok = get_calibration_point_I(index, &value2);
    if(!ok)
    {
        print_last_error();
    }
    ok = get_calibration_point_I(index + 1, &value3);
    if(!ok)
    {
        print_last_error();
    }
    //

    signed long V1 = 0;
    if(index > (COUNT_B588_CALIB_POINTS_DAC_I - 1))
    {
        //float y = (float)MAX_B588_VALUE_I - (float)correct_DAC_I[COUNT_B588_CALIB_POINTS_DAC_I-1];
        float y = (float)MAX_B588_VALUE_I - (float)value;
        float x = (float)MAGIC_B588_I;
        float m = (float)new_setting_I - (float)MAX_B588_VALUE_I + x;
        float n = (y * m) / x;
        //V1 = correct_DAC_I[COUNT_B588_CALIB_POINTS_DAC_I-1] + (signed long)n;
        V1 = value + (signed long)n;
        return abs(V1);
    }

    //---
    if(index == (COUNT_B588_CALIB_POINTS_DAC_I - 1))
    {
        //V1 = ((signed long)(new_setting_I % MAGIC_B588_I) * (MAX_B588_VALUE_I - correct_DAC_I[index])) / MAGIC_B588_I;
        V1 = ((signed long)(new_setting_I % MAGIC_B588_I) * (MAX_B588_VALUE_I - value2)) / MAGIC_B588_I;
        //V1 += correct_DAC_I[index];
        V1 += value;
        return abs(V1);
    }

    //---
    //V1 = ((signed long)(new_setting_I % MAGIC_B588_I) * (correct_DAC_I[index + 1] - correct_DAC_I[index])) / MAGIC_B588_I;
    V1 = ((signed long)(new_setting_I % MAGIC_B588_I) * (value3 - value2)) / MAGIC_B588_I;
    //V1 += correct_DAC_I[index];
    V1 += value;
    return abs(V1);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Powersupply_B588::event(QEvent *e)
{
    //qDebug() << e->type();
    if(e->type() == QEvent::WindowDeactivate)
    {
#ifdef QT_DEBUG
        qDebug() << "Powersupply_B588 event:WindowDeactivate";
#endif
        flag_closed = true;
    }
    return true;
}
//--------------------------------------------------------------------------------
