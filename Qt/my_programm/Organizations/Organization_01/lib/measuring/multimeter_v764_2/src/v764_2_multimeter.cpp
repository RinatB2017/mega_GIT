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
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QMenu>
#include <QTime>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "v764_2_multimeter.hpp"
#include "v764_2_packets.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
union STATE_BYTE
{
    unsigned char value;
    struct
    {
        uint8_t state:3;
        uint8_t limit:3;
        uint8_t not_new_value:1;
        uint8_t sign:1;
    } bites;
};

union ERROR_BYTE
{
    unsigned char value;
    struct
    {
        uint8_t overload:1;
        uint8_t lo_freq:1;
        uint8_t reserved:6;
    } bites;
};
//--------------------------------------------------------------------------------
Multimeter_V764_2::Multimeter_V764_2(QObject *parent) :
    QObject(parent),
    is_ready(false),
    delay_ms(1000),
    address(1)
{
    init();
}
//--------------------------------------------------------------------------------
Multimeter_V764_2::~Multimeter_V764_2()
{
#ifdef QT_DEBUG
    qDebug() << "~Multimeter_V764_2()";
#endif
    if(serial.isOpen())
    {
        serial.close();
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::close_connect(void)
{
    if(serial.isOpen()) serial.close();
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::init(void)
{
    last_error = E_V764_2_NO_ERROR;

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::port_read(void)
{
    //emit debug("port_read");
    data_multimeter.append(serial.readAll());
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::port_error(QSerialPort::SerialPortError serial_error)
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
void Multimeter_V764_2::set_state_silence(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::find_device(bool init_mb)
{
    QList<int> speeds;

    speeds.clear();
    speeds.append(9600);
    speeds.append(19200);
    speeds.append(28800);
    speeds.append(38400);
    speeds.append(57600);
    speeds.append(76800);
    speeds.append(115200);

    if(serial.isOpen()) serial.close();

    flag_closed = false;
    set_address(address);
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
#ifdef ONLY_ARDUINO
        if(port.portName() == "ttyUSB0")    continue;
        if(port.portName() == "ttyUSB1")    continue;
        if(port.portName() == "ttyUSB2")    continue;
        if(port.portName() == "ttyUSB3")    continue;
        if(port.portName() == "ttyS0")      continue;
        if(port.portName() == "ttyS1")      continue;
        if(port.portName() == "ttyS2")      continue;
        if(port.portName() == "ttyS3")      continue;
#endif

        emit info(QString(tr("Ищем %1 на порту %2"))
                  .arg(MULTIMETER_NAME)
                  .arg(port.portName()));
        foreach (int speed, speeds)
        {
            if(flag_closed) return false;

            serial.setPort(port);
            bool ok = serial.setBaudRate(speed);
            if(ok)
            {
                if(serial.open(QIODevice::ReadWrite))
                {
                    if(init_mb)
                    {
                        init_modbus();
                    }
                    int err = send_cmd_0x46();
                    if(err == E_V764_2_NO_ERROR)
                    {
                        emit info(QString(tr("Мультиметр В7-64/2 найден на скорости %1")).arg(speed));
                        return true;
                    }
                    serial.close();
                }
                else
                {
                    //emit error(QString("error open port %1").arg(port.portName()));
                }
            }
            else
            {
                //emit error(QString("setBaudRate(%1) is failed!").arg(speed));
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::init_modbus(void)
{
    if(is_silence == false)
    {
        emit info(tr("Инициализация режима MODBUS"));
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    QByteArray ba;
    ba.clear();
    ba.append('4');
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    ba.clear();
    ba.append('F');
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    if(is_silence == false)
    {
        emit info("OK");
    }

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::init_terminal(void)
{
    emit info(tr("Инициализация режима TERMINAL"));
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    QByteArray ba;
    ba.clear();
    ba.append('4');
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    ba.clear();
    ba.append('G');
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::set_delay_ms(int delay_ms)
{
    this->delay_ms = delay_ms;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::send_cmd_0x41(void)
{
    if(is_silence == false)
    {
        emit info(V764_2_CMD_0x41_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    CMD_V7642_0x41_QUESTION question;

    question.body.header.address = get_address();
    question.body.header.cmd = V764_2_CMD_0x41;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf,
                                                 sizeof(CMD_V7642_0x41_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(CMD_V7642_0x41_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }
    data_multimeter.clear();
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::send_cmd_0x42(unsigned char baud,
                                     unsigned char time)
{
    if(is_silence == false)
    {
        emit info(V764_2_CMD_0x42_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    CMD_V7642_0x42_QUESTION question;

    question.body.header.address = 0xFE;
    question.body.header.cmd = V764_2_CMD_0x42;
    question.body.header.count_data = 2;
    question.body.data.baud = baud;
    question.body.data.time = time;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf,
                                                 sizeof(CMD_V7642_0x42_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(CMD_V7642_0x42_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }
    data_multimeter.clear();
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::send_cmd_0x45(void)
{
    if(is_silence == false)
    {
        emit info(V764_2_CMD_0x45_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    CMD_V7642_0x45_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = V764_2_CMD_0x45;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf,
                                                 sizeof(CMD_V7642_0x45_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(CMD_V7642_0x45_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }

    data_multimeter.clear();
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    int err = check_packet_answer(V764_2_CMD_0x45, sizeof(CMD_V7642_0x45_ANSWER));
    if(err != E_V764_2_NO_ERROR)
    {
        return print_last_error();
    }

    //CMD_V7642_0x45_ANSWER *answer = (CMD_V7642_0x45_ANSWER *)data_multimeter.data();

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::send_cmd_0x46(void)
{
    if(is_silence == false)
    {
        emit info(V764_2_CMD_0x46_TEXT);
        emit info(QString("address %1").arg(address));
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    CMD_V7642_0x46_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = V764_2_CMD_0x46;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf,
                                                 sizeof(CMD_V7642_0x46_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(CMD_V7642_0x46_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }
    //emit trace(ba.toHex().toUpper());

    data_multimeter.clear();
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    int err = check_packet_answer(V764_2_CMD_0x46, sizeof(CMD_V7642_0x46_ANSWER));
    if(err != E_V764_2_NO_ERROR)
    {
        return get_last_error();
    }

    if(is_silence == false)
    {
        CMD_V7642_0x46_ANSWER *answer = (CMD_V7642_0x46_ANSWER *)data_multimeter.data();
        emit debug(QString("address %1").arg(answer->body.header.address));
        emit debug(QString("cmd 0x%1").arg(answer->body.header.cmd, 0, 16));
        emit debug(QString("count_data %1").arg(answer->body.header.count_data));
        emit debug(QString("type %1").arg(answer->body.data.type));
        emit debug(QString("year %1").arg(answer->body.data.year));
        emit debug(QString("month %1").arg(answer->body.data.month));
        emit debug(QString("serno_lo %1").arg(answer->body.data.serno_lo));
        emit debug(QString("serno_hi %1").arg(answer->body.data.serno_hi));
        emit debug(QString("baud %1").arg(answer->body.data.baud));
        emit debug(QString("time_waiting %1").arg(answer->body.data.time_waiting));
        emit debug(QString("address_station %1").arg(answer->body.data.address_station));
        emit debug(QString("byte_test %1").arg(answer->body.data.byte_test));
    }

    emit debug(ba.toHex());
    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::send_cmd_0x47(int *value_uV)
{
    if(is_silence == false)
    {
        emit info(V764_2_CMD_0x47_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    CMD_V7642_0x47_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = V764_2_CMD_0x47;
    question.body.header.count_data = 0;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf,
                                                 sizeof(CMD_V7642_0x47_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(CMD_V7642_0x47_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }

    emit debug(ba.toHex());
    data_multimeter.clear();
    is_ready = false;
    serial.write(ba);

#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(V764_2_CMD_0x47), 200, delay_ms);
#endif

    int err = check_packet_answer(V764_2_CMD_0x47, sizeof(CMD_V7642_0x47_ANSWER));
    if(err != E_V764_2_NO_ERROR)
    {
        return get_last_error();
    }

    CMD_V7642_0x47_ANSWER *answer = (CMD_V7642_0x47_ANSWER *)data_multimeter.data();
    if(answer->body.data.error & 0x01)
    {
        last_error = E_V764_2_OVERLOAD;
        return last_error;
    }
    if(answer->body.data.error & 0x02)
    {
        last_error = E_V764_2_LO_FREQ;
        return last_error;
    }

    int result = answer->body.data.value;
    if(answer->body.data.state & 0x80) result = -result;

#ifdef USE_TEST_BOARD
    int temp = result * 65535000.0f / 2500000.0f;
    result = temp;
#endif

    *value_uV = result;
    emit debug(QString(tr("Значение: %1")).arg(result));

    STATE_BYTE state_byte;
    state_byte.value = answer->body.data.state;
    emit current_limit(state_byte.bites.limit);
    emit current_sign(state_byte.bites.sign);

    switch(state_byte.bites.state)
    {
    case MODE_UDC:
        emit measurement_mode(MODE_UDC);
        emit debug(tr("Режим измерений: V"));
        switch(state_byte.bites.limit)
        {
        case 0: emit debug(tr("Предел измерений: 0.5V"));    break;
        case 1: emit debug(tr("Предел измерений: 2.5V"));    break;
        case 2: emit debug(tr("Предел измерений: 12.5V"));   break;
        case 3: emit debug(tr("Предел измерений: 50V"));     break;
        case 4: emit debug(tr("Предел измерений: 250V"));    break;
        case 5: emit debug(tr("Предел измерений: 1000V"));   break;
        }
        break;

    case MODE_IDC:
        emit measurement_mode(MODE_IDC);
        emit debug(tr("Режим измерений: I"));
        break;

    case MODE_R:
        emit measurement_mode(MODE_R);
        emit debug(tr("Режим измерений: O"));
        switch(state_byte.bites.limit)
        {
        case 0: emit debug(tr("Предел измерений: 2.5кОм"));  break;
        case 1: emit debug(tr("Предел измерений: 16кОм"));   break;
        case 2: emit debug(tr("Предел измерений: 150кОм"));  break;
        case 3: emit debug(tr("Предел измерений: 1000МОм")); break;
        }
        break;

    case MODE_UAC:
        emit measurement_mode(MODE_UAC);
        emit debug(tr("Режим измерений: ~V"));
        switch(state_byte.bites.limit)
        {
        case 0: emit debug(tr("Предел измерений: 0.2V"));    break;
        case 1: emit debug(tr("Предел измерений: 2V"));      break;
        case 2: emit debug(tr("Предел измерений: 20V"));     break;
        case 3: emit debug(tr("Предел измерений: 200V"));    break;
        case 4: emit debug(tr("Предел измерений: 700V"));    break;
        }
        break;

    case MODE_IAC:
        emit measurement_mode(MODE_IAC);
        emit debug(tr("Режим измерений: ~I"));
        switch(state_byte.bites.limit)
        {
        case 0: emit debug(tr("Предел измерений: 0.2А"));    break;
        case 1: emit debug(tr("Предел измерений: 2А"));      break;
        }
        break;

    case MODE_HZ:
        emit measurement_mode(MODE_HZ);
        emit debug(tr("Режим измерений: Hz"));
        break;

    case MODE_MHZ:
        emit measurement_mode(MODE_MHZ);
        emit debug(tr("Режим измерений: MHz"));
        break;

    default:
        emit error(QString("unknown state %1").arg(state_byte.bites.state));
        break;
    }

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::send_cmd_0x48(unsigned char new_address)
{
    if(is_silence == false)
    {
        emit info(V764_2_CMD_0x48_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    CMD_V7642_0x48_QUESTION question;

    question.body.header.address = address;
    question.body.header.cmd = V764_2_CMD_0x48;
    question.body.header.count_data = 1;
    question.body.data.data_0 = new_address;
    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf,
                                                 sizeof(CMD_V7642_0x48_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(CMD_V7642_0x48_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }
    data_multimeter.clear();
    is_ready = false;
    serial.write(ba);

#ifndef FAST_COMMAND
    wait(delay_ms);
#else
    wait(sizeof(V764_2_CMD_0x48), 200, delay_ms);
#endif

    int err = check_packet_answer(V764_2_CMD_0x48, sizeof(CMD_V7642_0x48_ANSWER));
    if(err != E_V764_2_NO_ERROR)
    {
        return print_last_error();
    }

    //CMD_V7642_0x48_ANSWER *answer = (CMD_V7642_0x48_ANSWER *)data_multimeter.data();

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::send_cmd_0x53(unsigned char number_button_1_16)
{
    if(is_silence == false)
    {
        emit info(V764_2_CMD_0x53_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_2_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    CMD_V7642_0x53_QUESTION question;

    if((number_button_1_16 == 0) || (number_button_1_16 > 16))
    {
        last_error = E_V764_2_UNKNOWN_ERROR;
        return last_error;
    }

    question.body.header.address = address;
    question.body.header.cmd = V764_2_CMD_0x53;
    question.body.header.count_data = sizeof(question.body.data);
    switch(number_button_1_16)
    {
    case BUTTON_NUMBER_UDC: question.body.data.data_0 = BUTTON_CODE_UDC;    break;
    case BUTTON_NUMBER_IDC: question.body.data.data_0 = BUTTON_CODE_IDC;    break;
    case BUTTON_NUMBER_R:   question.body.data.data_0 = BUTTON_CODE_R;      break;
    case BUTTON_NUMBER_UAC: question.body.data.data_0 = BUTTON_CODE_UAC;    break;
    case BUTTON_NUMBER_IAC: question.body.data.data_0 = BUTTON_CODE_IAC;    break;
    case BUTTON_NUMBER_HZ:  question.body.data.data_0 = BUTTON_CODE_HZ;     break;
    case BUTTON_NUMBER_MHZ: question.body.data.data_0 = BUTTON_CODE_MHZ;    break;
    case BUTTON_NUMBER_AVR: question.body.data.data_0 = BUTTON_CODE_AVR;    break;
    }

    question.body.crc16 = CRC::powersupply_crc16((unsigned char *)&question.buf,
                                                 sizeof(CMD_V7642_0x53_QUESTION) - 2);

    QByteArray ba;
    ba.clear();
    for(unsigned int n=0; n<sizeof(CMD_V7642_0x53_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }
    data_multimeter.clear();
    is_ready = false;
    serial.write(ba);

    wait(delay_ms);

    int err = check_packet_answer(V764_2_CMD_0x53, sizeof(CMD_V7642_0x53_ANSWER));
    if(err != E_V764_2_NO_ERROR)
    {
        return print_last_error();
    }

    union U_BYTE acmd;
    acmd.value = data_multimeter.at(1);
    if(acmd.bites.bit7 == 0)
    {
        if((unsigned int)data_multimeter.length() > sizeof(CMD_V7642_0x53_ANSWER))
        {
            last_error = E_V764_2_ERROR_BIG_ANSWER;
            return last_error;
        }
        if((unsigned int)data_multimeter.length() < sizeof(CMD_V7642_0x53_ANSWER))
        {
            last_error = E_V764_2_ERROR_SMALL_ANSWER;
            return last_error;
        }
    }
    else
    {
        if((unsigned int)data_multimeter.length() > sizeof(CMD_V7642_0x53_ANSWER_ERROR))
        {
            last_error = E_V764_2_ERROR_BIG_ANSWER;
            return last_error;
        }
        if((unsigned int)data_multimeter.length() < sizeof(CMD_V7642_0x53_ANSWER_ERROR))
        {
            last_error = E_V764_2_ERROR_SMALL_ANSWER;
            return last_error;
        }
        CMD_V7642_0x53_ANSWER_ERROR *err = (CMD_V7642_0x53_ANSWER_ERROR *)data_multimeter.data();
        emit error(QString("error %1").arg(err->body.error));
    }

    //CMD_V7642_0x53_ANSWER *answer = (CMD_V7642_0x53_ANSWER *)data_multimeter.data();

    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::wait(int time_msec)
{
#ifdef FAKE
    Q_UNUSED(time_msec)
#else
    QTime time;

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
void Multimeter_V764_2::wait(int max_len,
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

        if(data_multimeter.size() >= max_len)
            break;
    }
#endif
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::msgError(const QString &message)
{
    QMessageBox::critical(0, tr("Ошибка"), message);
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::check_multimeter_v762_2(void)
{
    int res = send_cmd_0x46();
    if(res == E_V764_2_NO_ERROR)
        emit info(tr("Мультиметр В7-62/2 найден"));
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::set_address(int new_address)
{
    address = new_address;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V764_2::get_address(void)
{
    return address;
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::run_dialog_0x41(void)
{
    int err = send_cmd_0x41();
    if(err != E_V764_2_NO_ERROR)
    {
        print_last_error();
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::run_dialog_0x42(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(V764_2_CMD_0x42_TEXT);

    QComboBox *cb_speed = new QComboBox();
    cb_speed->addItem("9600");
    cb_speed->addItem("19200");
    cb_speed->addItem("38400");

    QSpinBox *sb_timeout = new QSpinBox();
    sb_timeout->setMinimum(MIN_TIME);
    sb_timeout->setMaximum(MAX_TIME);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Скорость")), 0, 0);
    grid->addWidget(cb_speed,                   0, 1);
    grid->addWidget(new QLabel(tr("Время")),    1, 0);
    grid->addWidget(sb_timeout,                 1, 1);

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
        int err = send_cmd_0x42(cb_speed->currentIndex()+1,
                                sb_timeout->value());
        if(err != E_V764_2_NO_ERROR)
        {
            print_last_error();
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::run_dialog_0x45(void)
{
    int err = send_cmd_0x45();
    if(err != E_V764_2_NO_ERROR)
    {
        print_last_error();
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::run_dialog_0x46(void)
{
    int err = send_cmd_0x46();
    if(err != E_V764_2_NO_ERROR)
    {
        print_last_error();
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::run_dialog_0x47(void)
{
    int value_mkV = 0;
    int err = send_cmd_0x47(&value_mkV);

    if(err == E_V764_2_NO_ERROR)
    {
        emit info(QString("value_mkV %1").arg(value_mkV));
    }
    else
    {
        print_last_error();
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::run_dialog_0x48(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(V764_2_CMD_0x48_TEXT);

    QSpinBox *sb_address = new QSpinBox();
    sb_address->setMinimum(MIN_ADDRESS);
    sb_address->setMaximum(MAX_ADDRESS);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Новый адрес")), 0, 0);
    grid->addWidget(sb_address,                    0, 1);

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
        int err = send_cmd_0x48(sb_address->value());
        if(err != E_V764_2_NO_ERROR)
        {
            print_last_error();
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void Multimeter_V764_2::run_dialog_0x53(void)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(V764_2_CMD_0x53_TEXT);

    QSpinBox *sb_code_button = new QSpinBox();
    sb_code_button->setMinimum(MIN_CODE_BUTTON);
    sb_code_button->setMaximum(MAX_CODE_BUTTON);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(tr("Код кнопки")), 0, 0);
    grid->addWidget(sb_code_button,               0, 1);

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
        int err = send_cmd_0x53(sb_code_button->value());
        if(err != E_V764_2_NO_ERROR)
        {
            print_last_error();
        }
    }
    dialog->deleteLater();
    emit info("OK");
}
//--------------------------------------------------------------------------------
QMenu *Multimeter_V764_2::add_menu(void)
{
    QMenu *menu = new QMenu(tr("Мультиметр В764/2"));
    QAction *action_m_0x41 = new QAction(menu);
    QAction *action_m_0x42 = new QAction(menu);
    QAction *action_m_0x45 = new QAction(menu);
    QAction *action_m_0x46 = new QAction(menu);
    QAction *action_m_0x47 = new QAction(menu);
    QAction *action_m_0x48 = new QAction(menu);
    QAction *action_m_0x53 = new QAction(menu);

    action_m_0x41->setText(V764_2_CMD_0x41_TEXT);
    action_m_0x42->setText(V764_2_CMD_0x42_TEXT);
    action_m_0x45->setText(V764_2_CMD_0x45_TEXT);
    action_m_0x46->setText(V764_2_CMD_0x46_TEXT);
    action_m_0x47->setText(V764_2_CMD_0x47_TEXT);
    action_m_0x48->setText(V764_2_CMD_0x48_TEXT);
    action_m_0x53->setText(V764_2_CMD_0x53_TEXT);

    menu->addAction(action_m_0x41);
    menu->addAction(action_m_0x42);
    menu->addAction(action_m_0x45);
    menu->addAction(action_m_0x46);
    menu->addAction(action_m_0x47);
    menu->addAction(action_m_0x48);
    menu->addAction(action_m_0x53);

    connect(action_m_0x41, SIGNAL(triggered()), this, SLOT(run_dialog_0x41()));
    connect(action_m_0x42, SIGNAL(triggered()), this, SLOT(run_dialog_0x42()));
    connect(action_m_0x45, SIGNAL(triggered()), this, SLOT(run_dialog_0x45()));
    connect(action_m_0x46, SIGNAL(triggered()), this, SLOT(run_dialog_0x46()));
    connect(action_m_0x47, SIGNAL(triggered()), this, SLOT(run_dialog_0x47()));
    connect(action_m_0x48, SIGNAL(triggered()), this, SLOT(run_dialog_0x48()));
    connect(action_m_0x53, SIGNAL(triggered()), this, SLOT(run_dialog_0x53()));

    return menu;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::print_last_error(void)
{
    switch(last_error)
    {
    case E_V764_2_NO_ERROR:
        break;
    case E_V764_2_ERROR_INDEX_NOT_CORRECT:  emit error("E_V764_2_ERROR_INDEX_NOT_CORRECT");     break;
    case E_V764_2_ERROR_PORT_NOT_OPEN:      emit error("Порт мультиметра В7-64/2 не открыт!");  break;
    case E_V764_2_ERROR_BREAK:              emit error("E_V764_2_ERROR_BREAK");                 break;
    case E_V764_2_ERROR_MAX_REPEAT:         emit error("E_V764_2_ERROR_MAX_REPEAT");            break;
    case E_V764_2_ERROR_NO_ANSWER:          emit error("Мультиметр не отвечает");               break;
    case E_V764_2_ERROR_BIG_ANSWER:         emit error("Неверная длина пакета (слишком большая)");      break;
    case E_V764_2_ERROR_SMALL_ANSWER:       emit error("Неверная длина пакета (слишком маленькая)");    break;
    case E_V764_2_ERROR_BAD_SERNO:          emit error("Неверный серийный номер");              break;
    case E_V764_2_ERROR_BAD_ADDRESS:        emit error("Неверный адрес");                       break;
    case E_V764_2_ERROR_BAD_CRC:            emit error("Неверная контрольная сумма");           break;
    case E_V764_2_ERROR_BAD_COMMAND:        emit error("Неверная команда");                     break;
    case E_V764_2_ERROR_BAD_LEN:            emit error("Неверная длина");                       break;
    case E_V764_2_ERROR_UNKNOWN_COMMAND:    emit error("Неизвестная команда");                  break;
    case E_V764_2_ERROR_SIZE_NOT_CORRECT:   emit error("Длина данных не соответствует завленной");  break;
    case E_V764_2_UNKNOWN_ERROR:            emit error("Неизвестная ошибка");                   break;
    case E_V764_2_INVALID_FLOAT:            emit error("E_V764_2_INVALID_FLOAT");               break;
    case E_V764_2_INVALID_LIMIT:            emit error("E_V764_2_INVALID_LIMIT");               break;
    case E_V764_2_INVALID_DELTA:            emit error("E_V764_2_INVALID_DELTA");               break;
    case E_V764_2_BREAK_MEASURING:          emit error("E_V764_2_BREAK_MEASURING");             break;
    case E_V764_2_ILLEGAL_FUNCTION:         emit error("Команда не реализована (недопустимый номер функции");   break;
    case E_V764_2_ILLEGAL_DATA_VALUE:       emit error("Не корректные данные или не верный CRC");   break;
    case E_V764_2_OVERLOAD:                 emit error("Перегрузка");                           break;
    case E_V764_2_LO_FREQ:                  emit error("Низкая частота");                       break;

    default: emit error(QString(tr("Unknown cmd %1")).arg(last_error)); break;
    }
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_2::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::event(QEvent *e)
{
    //qDebug() << e->type();
    if(e->type() == QEvent::WindowDeactivate)
    {
        flag_closed = true;
        emit debug("Multimeter_V764_2 event:WindowDeactivate");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_measuring_UDC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_UDC);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_measuring_UAC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_UAC);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_measuring_IDC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_IDC);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_measuring_IAC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_IAC);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_measuring_R(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_R);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_measuring_HZ(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_HZ);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_measuring_MHZ(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_MHZ);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::set_state_AVR(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_AVR);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::get_value(int *value_uV)
{
    int err = send_cmd_0x47(value_uV);
    return (err == E_V764_2_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_2::send_cmd(const char btn)
{
    serial.write(&btn);
    return true;
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void Multimeter_V764_2::receive(const QVariant &data)
{
    serial.receive(data);
}
#endif
//--------------------------------------------------------------------------------
int Multimeter_V764_2::check_packet_answer(unsigned char cmd, int packet_len)
{
    if(data_multimeter.isEmpty())
    {
        last_error = E_V764_2_ERROR_NO_ANSWER;
        return last_error;
    }

    CMD_V7642_ERROR_ANSWER *answer = (CMD_V7642_ERROR_ANSWER *)data_multimeter.constData();
    unsigned char answer_cmd = answer->body.header.cmd & 0x7F;
    bool flag_error = answer->body.header.cmd & 0x80;

    if(answer_cmd != cmd)
    {
        last_error = E_V764_2_ERROR_BAD_COMMAND;
        return last_error;
    }
    if(flag_error)
    {
        switch(answer->body.error)
        {
        case 1:
            last_error = E_V764_2_ILLEGAL_FUNCTION;
            return last_error;

        case 4:
            last_error = E_V764_2_ILLEGAL_DATA_VALUE;
            return last_error;
        }
    }

    if(data_multimeter.length() < packet_len)
    {
        emit debug(data_multimeter.toHex());
        emit debug(QString("data_multimeter.length %1 < packet_len %2")
                   .arg(data_multimeter.length())
                   .arg(packet_len));
        last_error = E_V764_2_ERROR_SMALL_ANSWER;
        return last_error;
    }
    if(data_multimeter.length() > packet_len)
    {
        emit debug(data_multimeter.toHex());
        emit debug(QString("data_multimeter.length %1 > packet_len %2")
                   .arg(data_multimeter.length())
                   .arg(packet_len));
        last_error = E_V764_2_ERROR_BIG_ANSWER;
        return last_error;
    }

    V7642_HEADER *header = (V7642_HEADER *)data_multimeter.constData();
    int len = data_multimeter.size() - sizeof(V7642_HEADER) - 2;
    if(header->cmd != cmd)
    {
        emit error(data_multimeter.toHex());
        last_error = E_V764_2_ERROR_BAD_COMMAND;
        return last_error;
    }
    if(header->count_data != len)
    {
        emit debug(data_multimeter.toHex());
        emit debug(QString(tr("check_packet_answer: size (пришло) %1 < (ожидалось) %2"))
                   .arg(header->count_data)
                   .arg(len));
        last_error = E_V764_2_ERROR_SIZE_NOT_CORRECT;
        return last_error;
    }
    last_error = E_V764_2_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------

