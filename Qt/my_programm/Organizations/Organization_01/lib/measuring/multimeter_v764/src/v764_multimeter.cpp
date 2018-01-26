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
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "v764_multimeter.hpp"
#include "v764_packets.hpp"
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Multimeter_V764::Multimeter_V764(QObject *parent) :
    QObject(parent),
    delay_ms(1000),
    address(1)
{
    init();
}
//--------------------------------------------------------------------------------
Multimeter_V764::~Multimeter_V764()
{
#ifdef QT_DEBUG
    qDebug() << "~Multimeter_V764()";
#endif
    if(serial.isOpen())
    {
        serial.close();
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V764::close_connect(void)
{
    if(serial.isOpen()) serial.close();
}
//--------------------------------------------------------------------------------
void Multimeter_V764::init(void)
{
    last_error = E_V764_NO_ERROR;

    state = WAIT_CR;
    data_is_ready = false;

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
union MULTIMETER_DATA {
    struct {
        uint8_t begin_byte;
        uint8_t data[8];
        uint8_t dimension[2];
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
void Multimeter_V764::analize_data(void)
{
    //emit info(dirty_data.data());
    //if(clean_data.length() != sizeof(MULTIMETER_DATA)) return;

    MULTIMETER_DATA *data = (MULTIMETER_DATA *)clean_data.data();

    QString str;
    str.clear();
    for(int n=0; n<8; n++)
    {
        str.append(data->body.data[n]);
    }
    bool ok = false;
    double temp = str.toDouble(&ok);

    if(ok)
    {
        value = temp;
        //qDebug() << str << temp << temp * 1e6;
        //emit info(QString("[%1]").arg(value));
        data_is_ready = true;
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V764::port_read(void)
{
    //emit debug("port_read");
    QByteArray dirty_data;
    dirty_data.append(serial.readAll());
    for(int n=0; n<dirty_data.length(); n++)
    {
        switch(state)
        {
        case WAIT_CR:
            if(dirty_data.at(n) == CR)
                state = WAIT_LF;
            else
                clean_data.append(dirty_data.at(n));
            break;

        case WAIT_LF:
            if(dirty_data.at(n) == LF)
            {
                analize_data();
                dirty_data.clear();
                clean_data.clear();
                state = WAIT_CR;
                return;
            }
            break;

        default:
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V764::port_error(QSerialPort::SerialPortError serial_error)
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
void Multimeter_V764::set_state_silence(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::find_device(void)
{
    if(serial.isOpen()) serial.close();

    data_is_ready = false;
    flag_closed = false;
    set_address(address);
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        emit info(QString(tr("Ищем %1 на порту %2"))
                  .arg(MULTIMETER_NAME)
                  .arg(port.portName()));
        if(flag_closed) return false;

        serial.setPort(port);
        bool ok = serial.setBaudRate(9600);
        if(ok)
        {
            if(serial.open(QIODevice::ReadWrite))
            {
                QTime timer;
                timer.start();
#ifndef FAKE
                while(timer.elapsed() < 5000)
                {
                    QCoreApplication::processEvents();
                }
#else
                data_is_ready = true;
#endif
                if(data_is_ready)
                {
                    emit info(tr("Мультиметр найден!"));
                    return true;
                }
                else
                {
                    serial.close();
                }
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
    return false;
}
//--------------------------------------------------------------------------------
void Multimeter_V764::set_delay_ms(int delay_ms)
{
    this->delay_ms = delay_ms;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::send_cmd_0x41(void)
{
    last_error = E_V764_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::send_cmd_0x42(unsigned char baud,
                                   unsigned char time)
{
    Q_UNUSED(baud);
    Q_UNUSED(time);

    last_error = E_V764_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::send_cmd_0x45(void)
{
    last_error = E_V764_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::send_cmd_0x46(void)
{
    if(is_silence == false)
    {
        emit info(V764_CMD_0x46_TEXT);
        emit info(QString("address %1").arg(address));
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    last_error = E_V764_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::send_cmd_0x47(int *value_uV)
{
    if(is_silence == false)
    {
        emit info(V764_CMD_0x47_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    //???
    int cnt_error = 0;
    data_is_ready = false;
    while(!data_is_ready)
    {
        cnt_error++;
        if(cnt_error>100)
        {
            last_error = E_V764_ILLEGAL_DATA_VALUE;
            return last_error;
        }
        QCoreApplication::processEvents();
        Sleeper::msleep(100);
    }
    //---

    *value_uV = value * 1e6;
    //emit trace(QString(tr("Значение: %1")).arg(value));
    data_is_ready = false;

    last_error = E_V764_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::send_cmd_0x48(unsigned char new_address)
{
    Q_UNUSED(new_address);

    last_error = E_V764_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::send_cmd(const char btn)
{
    serial.write(&btn);
    return true;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::send_cmd_0x53(unsigned char number_button_1_16)
{
    if(is_silence == false)
    {
        emit info(V764_CMD_0x53_TEXT);
    }
    if(serial.isOpen() == false)
    {
        last_error = E_V764_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    //---
    bool ok = false;

    switch(number_button_1_16)
    {
    case BUTTON_NUMBER_UDC:
    case BUTTON_NUMBER_IDC:
    case BUTTON_NUMBER_R:
    case BUTTON_NUMBER_UAC:
    case BUTTON_NUMBER_IAC:
    case BUTTON_NUMBER_HZ:
    case BUTTON_NUMBER_MHZ:
    case BUTTON_NUMBER_AVR:
        ok = send_cmd(number_button_1_16);
        break;

    default:
        ok = false;
        break;
    }

    //---

    if(!ok)
        last_error = E_V764_UNKNOWN_ERROR;
    else
        last_error = E_V764_NO_ERROR;

    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V764::msgError(const QString &message)
{
    QMessageBox::critical(0, tr("Ошибка"), message);
}
//--------------------------------------------------------------------------------
void Multimeter_V764::check_multimeter_v762(void)
{
    int res = send_cmd_0x46();
    if(res == E_V764_NO_ERROR)
        emit info(tr("Мультиметр В7-62 найден"));
}
//--------------------------------------------------------------------------------
void Multimeter_V764::set_address(int new_address)
{
    address = new_address;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V764::get_address(void)
{
    return address;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::print_last_error(void)
{
    switch(last_error)
    {
    case E_V764_NO_ERROR:
        break;
    case E_V764_ERROR_INDEX_NOT_CORRECT:  emit error(tr("E_V764_ERROR_INDEX_NOT_CORRECT")); break;
    case E_V764_ERROR_PORT_NOT_OPEN:      emit error(tr("Порт мультиметра В7-64 не открыт!")); break;
    case E_V764_ERROR_BREAK:              emit error(tr("E_V764_ERROR_BREAK")); break;
    case E_V764_ERROR_MAX_REPEAT:         emit error(tr("E_V764_ERROR_MAX_REPEAT")); break;
    case E_V764_ERROR_NO_ANSWER:          emit error(tr("Мультиметр не отвечает")); break;
    case E_V764_ERROR_BIG_ANSWER:         emit error(tr("Неверная длина пакета (слишком большая)")); break;
    case E_V764_ERROR_SMALL_ANSWER:       emit error(tr("Неверная длина пакета (слишком маленькая)")); break;
    case E_V764_ERROR_BAD_SERNO:          emit error(tr("Неверный серийный номер")); break;
    case E_V764_ERROR_BAD_ADDRESS:        emit error(tr("Неверный адрес")); break;
    case E_V764_ERROR_BAD_CRC:            emit error(tr("Неверная контрольная сумма")); break;
    case E_V764_ERROR_BAD_COMMAND:        emit error(tr("Неверная команда")); break;
    case E_V764_ERROR_BAD_LEN:            emit error(tr("Неверная длина")); break;
    case E_V764_ERROR_UNKNOWN_COMMAND:    emit error(tr("Неизвестная команда")); break;
    case E_V764_ERROR_SIZE_NOT_CORRECT:   emit error(tr("Длина данных не соответствует завленной"));
    case E_V764_UNKNOWN_ERROR:            emit error(tr("Неизвестная ошибка")); break;
    case E_V764_INVALID_FLOAT:            emit error(tr("E_V764_INVALID_FLOAT")); break;
    case E_V764_INVALID_LIMIT:            emit error(tr("E_V764_INVALID_LIMIT")); break;
    case E_V764_INVALID_DELTA:            emit error(tr("E_V764_INVALID_DELTA")); break;
    case E_V764_BREAK_MEASURING:          emit error(tr("E_V764_BREAK_MEASURING")); break;
    case E_V764_ILLEGAL_FUNCTION:         emit error(tr("Команда не реализована (недопустимый номер функции")); break;
    case E_V764_ILLEGAL_DATA_VALUE:       emit error(tr("Не корректные данные или не верный CRC")); break;
    case E_V764_OVERLOAD:                 emit error(tr("Перегрузка")); break;
    case E_V764_LO_FREQ:                  emit error(tr("Низкая частота")); break;

    default: emit error(QString(tr("Unknown cmd %1")).arg(last_error)); break;
    }
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::event(QEvent *e)
{
    //qDebug() << e->type();
    if(e->type() == QEvent::WindowDeactivate)
    {
        flag_closed = true;
        emit debug("Multimeter_V764 event:WindowDeactivate");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_measuring_UDC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_UDC);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_measuring_UAC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_UAC);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_measuring_IDC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_IDC);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_measuring_IAC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_IAC);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_measuring_R(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_R);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_measuring_HZ(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_HZ);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_measuring_MHZ(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_MHZ);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::set_state_AVR(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_AVR);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764::get_value(int *value_uV)
{
    int err = send_cmd_0x47(value_uV);
    return (err == E_V764_NO_ERROR);
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void Multimeter_V764::receive(const QVariant &data)
{
    serial.receive(data);
}
#endif
//--------------------------------------------------------------------------------
