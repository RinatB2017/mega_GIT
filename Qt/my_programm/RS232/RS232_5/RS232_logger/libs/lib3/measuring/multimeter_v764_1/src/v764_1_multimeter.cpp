/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QtGlobal>
#ifdef Q_OS_LINUX
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "serialbox5_wo_form.hpp"
#include "v764_1_multimeter.hpp"
#include "v764_1_packets.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Multimeter_V764_1::Multimeter_V764_1(QWidget *parent) :
    MyWidget(parent),
    delay_ms(1000),
    address(1)
{
    init();
}
//--------------------------------------------------------------------------------
Multimeter_V764_1::~Multimeter_V764_1()
{
#ifdef QT_DEBUG
    qDebug() << "~Multimeter_V764_1()";
#endif
    if(serial)
    {
        serial->serial_close();
#ifdef TEST
        disconnect(serial, &SerialPort_V764_1_test::output,
                   this,   &Multimeter_V764_1::port_read);
#else
        disconnect(serial, &SerialBox5_wo_form::output,
                   this,   &Multimeter_V764_1::port_read);
#endif
        delete serial;
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::close_connect(void)
{
    if(serial->isOpen()) serial->serial_close();
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::init(void)
{
    last_error = E_V764_1_NO_ERROR;

    state = WAIT_CR;
    data_is_ready = false;

#ifndef TEST
    serial = new SerialBox5_wo_form();
    connect(serial, &SerialBox5_wo_form::output,
            this,   &Multimeter_V764_1::port_read);

    connect(serial, &SerialBox5_wo_form::info,  this,   &Multimeter_V764_1::info);
    connect(serial, &SerialBox5_wo_form::debug, this,   &Multimeter_V764_1::debug);
    connect(serial, &SerialBox5_wo_form::error, this,   &Multimeter_V764_1::error);
    connect(serial, &SerialBox5_wo_form::trace, this,   &Multimeter_V764_1::trace);

    connect(serial, &SerialBox5_wo_form::port_is_active,    this,   &Multimeter_V764_1::port_is_active);
    connect(serial, &SerialBox5_wo_form::output,            this,   &Multimeter_V764_1::port_read);
#else
    serial = new SerialPort_V764_1_test();

    connect(serial, &SerialPort_V764_1_test::info,  this,   &Multimeter_V764_1::info);
    connect(serial, &SerialPort_V764_1_test::debug, this,   &Multimeter_V764_1::debug);
    connect(serial, &SerialPort_V764_1_test::error, this,   &Multimeter_V764_1::error);
    connect(serial, &SerialPort_V764_1_test::trace, this,   &Multimeter_V764_1::trace);

    connect(serial, &SerialPort_V764_1_test::output,
            this,   &Multimeter_V764_1::port_read);

    //TODO для синусоиды
    connect(this,   &Multimeter_V764_1::get_test_value,
            serial, &SerialPort_V764_1_test::get_test_value);
#endif
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
void Multimeter_V764_1::analize_data(void)
{
    QString str = clean_data.replace(" V", "").replace('\r', "").replace('\n', "");

    bool ok = false;
    double temp = str.toDouble(&ok);

    if(ok)
    {
        value = temp;
        data_is_ready = true;
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::port_read(const QByteArray &data)
{
    QByteArray dirty_data;
    dirty_data.append(data);
    for(int n=0; n<dirty_data.length(); n++)
    {
        if(serial->isOpen() == false)   return;
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
void Multimeter_V764_1::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
#ifdef Q_OS_LINUX
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
#endif
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
#endif
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
        //    default:
        //        emit error(QString("Unknown error %1").arg(serial_error));
        //        break;
    }
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::set_state_silence(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::find_device(void)
{
    if(serial->isOpen()) serial->serial_close();

    data_is_ready = false;
    flag_closed = false;
    set_address(address);
#if 1
    QStringList sl_ports = serial->get_port_names();
    foreach (QString port_name, sl_ports)
#else
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
#endif
    {
        emit info(QString(tr("Ищем %1 на порту %2"))
                  .arg(MULTIMETER_NAME)
                  .arg(port_name));
        if(flag_closed) return false;

        serial->setPortName(port_name);
        bool ok = serial->setBaudRate(9600);
        if(ok)
        {
            if(serial->serial_open())
            {
                QElapsedTimer timer;
                timer.start();
#ifndef TEST
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
                    emit port_is_active(true);
                    return true;
                }
                else
                {
                    serial->serial_close();
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
void Multimeter_V764_1::set_delay_ms(int delay_ms)
{
    this->delay_ms = delay_ms;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::send_cmd_0x41(void)
{
    last_error = E_V764_1_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::send_cmd_0x42(unsigned char baud,
                                     unsigned char time)
{
    Q_UNUSED(baud)
    Q_UNUSED(time)

    last_error = E_V764_1_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::send_cmd_0x45(void)
{
    last_error = E_V764_1_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::send_cmd_0x46(void)
{
    if(is_silence == false)
    {
        emit info(V764_1_CMD_0x46_TEXT);
        emit debug(QString("address %1").arg(address));
    }
    if(serial->isOpen() == false)
    {
        last_error = E_V764_1_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    last_error = E_V764_1_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::send_cmd_0x47(int *value_uV)
{
    if(is_silence == false)
    {
        emit info(V764_1_CMD_0x47_TEXT);
    }
    if(serial->isOpen() == false)
    {
        last_error = E_V764_1_ERROR_PORT_NOT_OPEN;
        return last_error;
    }

    //???
    int cnt_error = 0;
    data_is_ready = false;

    emit get_test_value();
    while(!data_is_ready)
    {
        QCoreApplication::processEvents();

        cnt_error++;
        if(cnt_error>100)
        {
            last_error = E_V764_1_ILLEGAL_DATA_VALUE;
            return last_error;
        }
        wait_msec(DELAY_MS);
    }
    //---

    *value_uV = static_cast<int>(value * 1e6);
    emit trace(QString(tr("Значение: %1")).arg(value, 'f'));
    data_is_ready = false;

    last_error = E_V764_1_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::send_cmd_0x48(unsigned char new_address)
{
    Q_UNUSED(new_address)

    last_error = E_V764_1_NO_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::send_cmd(const char btn)
{
    QByteArray ba;
    ba.clear();
    ba.append(btn);
    ba.append(0x0D);

    //TODO надо сделать
    switch(btn)
    {
    case BUTTON_1:  emit measurement_mode(MODE::UDC);  break;
    case BUTTON_2:  emit measurement_mode(MODE::UAC);  break;
    case BUTTON_3:  emit measurement_mode(MODE::IDC);  break;
    case BUTTON_4:  emit measurement_mode(MODE::IAC);  break;
    case BUTTON_5:  emit measurement_mode(MODE::R);    break;
    case BUTTON_6:  emit measurement_mode(MODE::HZ);   break;
    case BUTTON_7:  emit measurement_mode(MODE::MHZ);  break;
    default:
        emit measurement_mode(MODE::UNKNOWN);   break;
        break;
    }
    //---

    emit debug(QString("send_cmd [%1]")
               .arg(ba.toHex().toUpper().data()));

    serial->write(ba);
    return true;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::send_cmd_0x53(unsigned char number_button_1_16)
{
    if(is_silence == false)
    {
        emit info(V764_1_CMD_0x53_TEXT);
    }
    if(serial->isOpen() == false)
    {
        last_error = E_V764_1_ERROR_PORT_NOT_OPEN;
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
        ok = send_cmd(static_cast<char>(number_button_1_16));
        break;

    default:
        ok = false;
        break;
    }

    //---

    if(!ok)
        last_error = E_V764_1_UNKNOWN_ERROR;
    else
        last_error = E_V764_1_NO_ERROR;

    return last_error;
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::msgError(const QString &message)
{
    QMessageBox::critical(nullptr, tr("Ошибка"), message);
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::check_multimeter_v764_1(void)
{
    int res = send_cmd_0x46();
    if(res == E_V764_1_NO_ERROR)
        emit info(tr("Мультиметр В7-64/1 найден"));
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::set_address(int new_address)
{
    address = new_address;
}
//--------------------------------------------------------------------------------
unsigned char Multimeter_V764_1::get_address(void)
{
    return static_cast<uchar>(address);
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::print_last_error(void)
{
    switch(last_error)
    {
    case E_V764_1_NO_ERROR:
        break;
    case E_V764_1_ERROR_INDEX_NOT_CORRECT:  emit error(tr("E_V764_1_ERROR_INDEX_NOT_CORRECT")); break;
    case E_V764_1_ERROR_PORT_NOT_OPEN:      emit error(tr("Порт мультиметра В7-64 не открыт!")); break;
    case E_V764_1_ERROR_BREAK:              emit error(tr("E_V764_1_ERROR_BREAK")); break;
    case E_V764_1_ERROR_MAX_REPEAT:         emit error(tr("E_V764_1_ERROR_MAX_REPEAT")); break;
    case E_V764_1_ERROR_NO_ANSWER:          emit error(tr("Мультиметр не отвечает")); break;
    case E_V764_1_ERROR_BIG_ANSWER:         emit error(tr("Неверная длина пакета (слишком большая)")); break;
    case E_V764_1_ERROR_SMALL_ANSWER:       emit error(tr("Неверная длина пакета (слишком маленькая)")); break;
    case E_V764_1_ERROR_BAD_SERNO:          emit error(tr("Неверный серийный номер")); break;
    case E_V764_1_ERROR_BAD_ADDRESS:        emit error(tr("Неверный адрес")); break;
    case E_V764_1_ERROR_BAD_CRC:            emit error(tr("Неверная контрольная сумма")); break;
    case E_V764_1_ERROR_BAD_COMMAND:        emit error(tr("Неверная команда")); break;
    case E_V764_1_ERROR_BAD_LEN:            emit error(tr("Неверная длина")); break;
    case E_V764_1_ERROR_UNKNOWN_COMMAND:    emit error(tr("Неизвестная команда")); break;
    case E_V764_1_ERROR_SIZE_NOT_CORRECT:   emit error(tr("Длина данных не соответствует завленной")); break;
    case E_V764_1_UNKNOWN_ERROR:            emit error(tr("Неизвестная ошибка")); break;
    case E_V764_1_INVALID_FLOAT:            emit error(tr("E_V764_1_INVALID_FLOAT")); break;
    case E_V764_1_INVALID_LIMIT:            emit error(tr("E_V764_1_INVALID_LIMIT")); break;
    case E_V764_1_INVALID_DELTA:            emit error(tr("E_V764_1_INVALID_DELTA")); break;
    case E_V764_1_BREAK_MEASURING:          emit error(tr("E_V764_1_BREAK_MEASURING")); break;
    case E_V764_1_ILLEGAL_FUNCTION:         emit error(tr("Команда не реализована (недопустимый номер функции")); break;
    case E_V764_1_ILLEGAL_DATA_VALUE:       emit error(tr("Не корректные данные или не верный CRC")); break;
    case E_V764_1_OVERLOAD:                 emit error(tr("Перегрузка")); break;
    case E_V764_1_LO_FREQ:                  emit error(tr("Низкая частота")); break;

    default: emit error(QString(tr("Unknown cmd %1")).arg(last_error)); break;
    }
    return last_error;
}
//--------------------------------------------------------------------------------
int Multimeter_V764_1::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::port_is_open(void)
{
    return serial->isOpen();
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::event(QEvent *e)
{
    Q_UNUSED(e);
    //    qDebug() << e->type();
    //    if(e->type() == QEvent::WindowDeactivate)
    //    {
    //        flag_closed = true;
    //        emit debug("Multimeter_V764_1 event:WindowDeactivate");
    //    }
    return true;
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_measuring_UDC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_UDC);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_measuring_UAC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_UAC);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_measuring_IDC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_IDC);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_measuring_IAC(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_IAC);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_measuring_R(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_R);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_measuring_HZ(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_HZ);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_measuring_MHZ(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_MHZ);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::set_state_AVR(void)
{
    int err = send_cmd_0x53(BUTTON_NUMBER_AVR);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::get_value(int *value_uV)
{
    int err = send_cmd_0x47(value_uV);
    return (err == E_V764_1_NO_ERROR);
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::wait_msec(int timeout_msec)
{
    QElapsedTimer time;

    emit debug(QString("Пауза %1 ms").arg(timeout_msec));
    if(timeout_msec < 1)
    {
        emit debug("timeout_msec < 1");
        return;
    }
    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        if(serial->isOpen() == false)
        {
            return;
        }
    }
}
//--------------------------------------------------------------------------------
#ifdef TEST
void Multimeter_V764_1::receive(const QVariant &data)
{
    serial->receive(data);
}
#endif
//--------------------------------------------------------------------------------
void Multimeter_V764_1::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Multimeter_V764_1::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Multimeter_V764_1::save_setting(void)
{

}
//--------------------------------------------------------------------------------
