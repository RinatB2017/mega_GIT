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
#include <QCoreApplication>
#include <QByteArray>
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "H4_X_errors.hpp"
#include "H4_X_calibrator.hpp"
#include "H4_X_keys.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Calibrator::Calibrator(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Calibrator::~Calibrator()
{

}
//--------------------------------------------------------------------------------
void Calibrator::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data);
#endif
}
//--------------------------------------------------------------------------------
void Calibrator::init(void)
{
    timeout = TIME_CALIBRATION;

#ifdef TEST
    serial = new SerialPort_H4_X_test();
    connect(serial, &SerialPort_H4_X_test::s_info,    this,   &Calibrator::info);
    connect(serial, &SerialPort_H4_X_test::s_debug,   this,   &Calibrator::debug);
    connect(serial, &SerialPort_H4_X_test::s_error,   this,   &Calibrator::error);
    connect(serial, &SerialPort_H4_X_test::s_trace,   this,   &Calibrator::trace);
#else
    serial = new QSerialPort();
    Q_ASSERT(serial);
#endif

    //устанавливаем дефолтные значения
    serial->setBaudRate(9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //---

    connect(serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

    //connect(&serial, SIGNAL(output(QByteArray)), this, SLOT(raw_data(QByteArray)));
    //connect(this, SIGNAL(calibrator_send(QByteArray)), &serial, SLOT(sendData(QByteArray)));
}
//--------------------------------------------------------------------------------
void Calibrator::port_read(void)
{
    QByteArray ba = serial->readAll();
    if(ba.isEmpty() == false)
    {
        data_calibrator.append(ba);
    }
    emit output(ba);
}
//--------------------------------------------------------------------------------
void Calibrator::port_error(QSerialPort::SerialPortError serial_error)
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
bool Calibrator::find_device(void)
{
    Q_ASSERT(delay_ms >= 1000);

    if(serial->isOpen())
    {
        serial->close();
    }

    // используем последний порт
    QString last_portname = load_string(P_CALIBRATOR_PORTNAME);
    if(!last_portname.isEmpty())
    {
        if(open_device(last_portname, 1))
        {
            //emit info("Калибратор найден");
            return true;
        }
    }
    //---

    foreach(const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        QString portname = port.portName();
        if(portname == "COM1")
        {
            emit debug("COM1 found");
        }
        if(open_device(portname, 1))
        {
            //emit info("Калибратор найден");
            save_string(P_CALIBRATOR_PORTNAME, portname);
            return true;
        }
        close_device();
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Calibrator::open_device(const QString &port_name,
                             int max_repeat)
{
    Q_ASSERT(max_repeat > 0);
    Q_ASSERT(delay_ms >= 1000);

    QSerialPort::FlowControl flow;
    flow = QSerialPort::NoFlowControl;

    int speed = 9600;
    if(serial->isOpen())
    {
        serial->close();
    }
    foreach(const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        if(port_name == port.portName())
        {
            emit info(QString("Порт %1").arg(port.portName()));
            serial->setPort(port);
            bool ok = serial->setBaudRate(speed);
            if(!ok)
            {
                emit error(QString("setBaudRate(%1) is failed!").arg(speed));
                return false;
            }
            ok = serial->setFlowControl(flow);
            if(!ok)
            {
                emit error(QString("setFlowControl(%1) is failed!").arg(flow));
                return false;
            }
            if(serial->open(QIODevice::ReadWrite))
            {
                emit info(QString("Проверка наличия калибратора на порту %1").arg(port.portName()));
                ok = check_calibrator(max_repeat);
                if(ok)
                {
                    return ok;
                }
                serial->close();
                return false;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void Calibrator::close_device(void)
{
    if(serial->isOpen())
    {
        emit info("Порт калибратора закрыт");
        serial->close();
    }
}
//--------------------------------------------------------------------------------
bool Calibrator::input(const QByteArray &data)
{
    // здесь нужно просто послать в порт и НЕ анализировать возврат

    QByteArray ba_temp = data;
    emit debug(QString("send %1").arg(ba_temp.replace("\r", "").replace("\n", "").data()));

    if(serial->isOpen() == false)
    {
        emit error("Порт калибратора не открыт!");
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:send timeout == %1 ms").arg(timeout));
        return false;
    }

    data_calibrator.clear();
    serial->write(data);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::send(QByteArray data, int max_repeat)
{
    emit debug(QString("%1").arg(data.data()));

    if(serial->isOpen() == false)
    {
        emit error("Порт калибратора не открыт!");
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:send timeout == %1 ms").arg(timeout));
        return false;
    }

    data_calibrator.clear();
    serial->write(data);

    return check_calibrator(max_repeat);
}
//--------------------------------------------------------------------------------
void Calibrator::wait_msec(int timeout_msec, bool wait_answer)
{
#ifdef TEST
    timeout_msec = TEST_WAIT_MS;
    wait_answer = false;
#endif

    QElapsedTimer time;

    emit debug(QString("timeout %1").arg(timeout_msec));
    if(timeout_msec < 1)
    {
        emit error("timeout_msec < 1");
        return;
    }

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        if(!port_is_open())
        {
            //кто-то выключил порт, выходим
            return;
        }
        if(wait_answer)
        {
            if(!data_calibrator.isEmpty())
            {
                emit info(QString("Прошло %1 msec").arg(time.elapsed()));
                emit debug(QString("wait_msec: %1").arg(data_calibrator.data()));
                return;
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool Calibrator::check_answer_calibrator(void)
{
    QByteArray answer = data_calibrator.replace("\r", "").replace("\n", "");
    if(answer.isEmpty())
    {
        emit error(QString("Калибратор не отвечает (время %1)").arg(timeout));
        return false;
    }

    emit debug(QString("data_calibrator: [%1][%2]")
              .arg(answer.data())
              .arg(answer.toHex().data()));

    bool ok;
    int value = answer.toInt(&ok);
    if(ok)
    {
        switch(value)
        {
        case 00:
            emit info("OK");
            return true;
        case 10:
            emit error(QString("%1 Невосстановимая ошибка ЭНЗУ.").arg(value));
            return false;
        case 11:
        case 12:
            emit error(QString("%1 Восстановимая ошибка ЭНЗУ.").arg(value));
            return false;
        case 13:
            emit error(QString("%1 Аппаратная ошибка ЭНЗУ.").arg(value));
            return false;
        case 19:
            emit error(QString("%1 Прибор не может точно выставить заданное напряжение (ток).").arg(value));
            return false;
        case 20:
            emit error(QString("%1 Попытка ввести слишком большое число.").arg(value));
            return false;
        case 21:
            emit error(QString("%1 Вводимое напряжение (ток) выходит за выбранный предел.").arg(value));
            return false;
        case 22:
            emit error(QString("%1 Попытка ввести отрицательное напряжение (ток) в режимах \"Hz\", \"kHz\".").arg(value));
            return false;
        case 23:
            emit error(QString("%1 Попытка калибровки не на калибровочной частоте в режимах \"Hz\", \"kHz\".").arg(value));
            return false;
        case 24:
            emit error(QString("%1 Вводимая калибровочная константа слишком отличается от исходной.").arg(value));
            return false;
        case 25:
            emit error(QString("%1 Слишком большая калибровочная константа при калибровке октад.").arg(value));
            return false;
        case 26:
            emit error(QString("%1 Попытка калибровки при выключенном тумблере разрешения калибровки.").arg(value));
            return false;
        case 27:
            emit error(QString("%1 Вводимое напряжение (ток) выходит за возможности прибора.").arg(value));
            return false;
        case 28:
            emit error(QString("%1 Вводимое напряжение (ток) выходит за установленный оператором уровень ограничения.").arg(value));
            return false;
        case 40:
            emit error(QString("%1 Перегрузка платы ЦАП.").arg(value));
            return false;
        case 51:
            emit error(QString("%1 Переполнение входного буфера (скорость выше 9600 бит/с).").arg(value));
            return false;
        case 52:
            emit error(QString("%1 Непонятная команда (ошибка оператора).").arg(value));
            return false;
        default:
            emit error(QString("%1: Неизвестная ошибка").arg(value));
            return false;
        }
        return true;
    }
    else
    {
        emit error(QString("Калибратор вернул: %1")
                   .arg(data_calibrator.data()));
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Calibrator::check_calibrator(int max_error)
{
    Q_ASSERT(timeout > 1000);
    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:check_calibrator timeout == %1 ms").arg(timeout));
        return false;
    }

    int cnt_error = 0;
    while(cnt_error < max_error)
    {
        data_calibrator.clear();

        QByteArray ba;
        ba.clear();
        ba.append("T0");
        ba.append((char)0x0D);

        serial->write(ba);
#ifndef TEST
        wait_msec(timeout);
#endif
        bool ok = check_answer_calibrator();
        if(ok)
        {
            return true;
        }
        else
        {
            cnt_error++;
        }
    }

    return false;
}
//--------------------------------------------------------------------------------
bool Calibrator::step_to_step(QString text,
                              int step_to_step_timeout,
                              int calibrator_timeout,
                              bool no_delay)
{
    if(text.isEmpty())
    {
        emit error("calibrator_step_to_step 1: text is Empty");
        return false;
    }

    text = text.remove(' ');

    if(text.isEmpty())
    {
        emit error("calibrator_step_to_step 2: text is Empty");
        return false;
    }
    if(text.length() % 2)
    {
        emit error("calibrator_step_to_step 3: text is Empty");
        return false;
    }

    int len = text.length();
    QByteArray ba_temp;
    for(int n=0; n<len; n+=2)
    {
        ba_temp = text.mid(n, 2).toLatin1();
        ba_temp.append((char)0x0D);
        serial->write(ba_temp);
#ifndef TEST
        wait_msec(step_to_step_timeout);
#else
        Q_UNUSED(step_to_step_timeout)
#endif
    }

    if(no_delay == false)
    {
#ifndef TEST
        wait_msec(calibrator_timeout);
#else
        Q_UNUSED(calibrator_timeout)
#endif
    }

    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::reset_U(void)
{
    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:reset_U timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QByteArray data;
    data.append("UD0\r");
    serial->write(data);
#ifndef TEST
    wait_msec(timeout);
#endif
    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::reset_I(void)
{
    if(serial->isOpen() == false)
    {
        emit error("Порт калибратора не открыт!");
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:reset_I timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QByteArray data;
    data.clear();
    data.append("ID0\r");
    serial->write(data);
#ifndef TEST
    wait_msec(timeout);
#endif
    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::check_calibrator(void)
{
    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    //emit info("Тест калибратора");
    bool ok = check_calibrator(1);
    if(!ok)
    {
        //emit error("Калибратор не найден");
        last_error = E_CALIBRATOR_ERROR;
        return false;
    }

    //emit info("Калибратор найден");
    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::port_is_open(void)
{
    return serial->isOpen();
}
//--------------------------------------------------------------------------------
bool Calibrator::set_U_V(float value)
{
    emit debug(QString("value %1").arg(value));

    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_U timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QString temp = QString("UD%1\r").arg(value);
    emit debug(temp);
    serial->write(temp.toLocal8Bit());
#ifndef TEST
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_U_V(float value, float freq_khz)
{
    emit debug(QString("value %1 freq %2")
               .arg(value)
               .arg(freq_khz));

    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_U(freq) timeout == %1 ms")
                   .arg(timeout));
    }

    data_calibrator.clear();

    QString temp;
    if(value < 0.1)
        temp = QString("UA%1F%2\r")
                .arg(value)
                .arg(freq_khz);
    else
        temp = QString("UA%1F%2\r")
                .arg(value, 0, 'f')
                .arg(freq_khz);

    emit info(temp.replace("\r", "").replace("\n", ""));
    serial->write(temp.toLocal8Bit());
#ifndef TEST
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_I_mA(float value)
{
    emit debug(QString("value %1").arg(value));

    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_I timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QString temp = QString("ID%1\r").arg(value);
    emit debug(temp);
    serial->write(temp.toLocal8Bit());
#ifndef TEST
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_I_mA(float value, float freq_khz)
{
    emit debug(QString("value %1 freq %2").arg(value).arg(freq_khz));

    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_I(freq) timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QString temp = QString("IA%1F%2\r").arg(value).arg(freq_khz);
    emit debug(temp);
    serial->write(temp.toLocal8Bit());
#ifndef TEST
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_R(unsigned int value)
{
    emit debug(QString("value %1").arg(value));

    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_R timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    serial->write(QString("R%1\r").arg(value).toLocal8Bit());
#ifndef TEST
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
void Calibrator::set_timeout(int value)
{
    timeout = value;
}
//--------------------------------------------------------------------------------
int Calibrator::get_timeout(void)
{
    return timeout;
}
//--------------------------------------------------------------------------------
void Calibrator::set_delay_ms(int value)
{
    delay_ms = value;
}
//--------------------------------------------------------------------------------
int Calibrator::get_delay_ms(void)
{
    return delay_ms;
}
//--------------------------------------------------------------------------------
int Calibrator::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
int Calibrator::set_safe_mode(void)
{
    Q_ASSERT(serial);

    if(serial->isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    data_calibrator.clear();
    QByteArray data;
    data.append("R0T0\r");
    serial->write(data);
#ifndef TEST
    wait_msec(timeout);
#endif

    bool ok = check_answer_calibrator();
    if(ok)
    {
        last_error = E_CALIBRATOR_NO_ERROR;
        return last_error;
    }

    last_error = E_CALIBRATOR_ERROR;
    return last_error;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_V(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_V);
        break;
    case H4_7:
        data.append(H4_7_KEY_V);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_V");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_I(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_I);
        break;
    case H4_7:
        data.append(H4_7_KEY_I);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_I");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_R(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_R);
        break;
    case H4_7:
        data.append(H4_7_KEY_R);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_R");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_VI(double value)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_ENTER_VI);
        data.append(get_value_string(value));
        data.append(H4_6_KEY_ENTER_VI);
        break;
    case H4_7:
        data.append(H4_7_KEY_ENTER_VI);
        data.append(get_value_string(value));
        data.append(H4_7_KEY_ENTER_VI);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }

    emit info(QString("Подаём %1").arg(value));
    emit debug(QString("set_VI(%1)").arg(value));

#ifdef STEP_BY_STEP
    int len = data.length();
    QString temp_str;
    for(int n=0; n<len; n+=2)
    {
        temp_str = QString("%1\r").arg(data.mid(n, 2));
        serial->write(temp_str.toLocal8Bit());
        wait_msec(delay_ms, false);
    }
#else
    data.append((char)0x0D);
    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_F(double value)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_ENTER_F);
        data.append(get_value_string(value));
        data.append(H4_6_KEY_ENTER_F);
        break;
    case H4_7:
        data.append(H4_7_KEY_ENTER_F);
        data.append(get_value_string(value));
        data.append(H4_7_KEY_ENTER_F);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }

    emit debug(QString("set_F(%1)").arg(value));

#ifdef STEP_BY_STEP
    int len = data.length();
    QString temp_str;
    for(int n=0; n<len; n+=2)
    {
        temp_str = QString("%1\r").arg(data.mid(n, 2));
        serial->write(temp_str.toLocal8Bit());
        wait_msec(delay_ms, false);
    }
#else
    data.append((char)0x0D);
    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_Hz(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_Hz);
        break;
    case H4_7:
        data.append(H4_7_KEY_Hz);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_Hz");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_kHz(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_kHz);
        break;
    case H4_7:
        data.append(H4_7_KEY_kHz);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_kHz");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_limit_1000(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_RIGHT);
        data.append(H4_6_KEY_RIGHT);
        break;
    case H4_7:
        data.append(H4_7_KEY_RIGHT);
        data.append(H4_7_KEY_RIGHT);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_limit_1000");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_limit_10000(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_RIGHT);
        data.append(H4_6_KEY_RIGHT);
        data.append(H4_6_KEY_RIGHT);
        break;
    case H4_7:
        data.append(H4_7_KEY_RIGHT);
        data.append(H4_7_KEY_RIGHT);
        data.append(H4_7_KEY_RIGHT);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_limit_10000");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_OFF(void)
{
    QString data;
    switch(type_calibrator)
    {
    case H4_6:
        data.append(H4_6_KEY_OFF);
        break;
    case H4_7:
        data.append(H4_7_KEY_OFF);
        break;
    default:
        qFatal("bad type calibrator");
        break;
    }
    data.append((char)0x0D);

    emit debug("set_OFF");

    serial->write(data.toLocal8Bit());
    wait_msec(delay_ms, false);
    return true;
}
//--------------------------------------------------------------------------------
QString Calibrator::get_value_string(qreal value)
{
    QString value_str = QString::number(value);
    //emit error(QString("QString::number: [%1]").arg(value_str));
    int len_value_str = value_str.length();
    QString temp;
    for(int n=0; n<len_value_str; n++)
    {
        switch(type_calibrator)
        {
        case H4_6:
            if(value_str.at(n) == '-') temp.append(H4_6_KEY_PLUS_MINUS);
            if(value_str.at(n) == '.') temp.append(H4_6_KEY_RIGHT);
            if(value_str.at(n) == '0') temp.append(H4_6_KEY_0);
            if(value_str.at(n) == '1') temp.append(H4_6_KEY_1);
            if(value_str.at(n) == '2') temp.append(H4_6_KEY_2);
            if(value_str.at(n) == '3') temp.append(H4_6_KEY_3);
            if(value_str.at(n) == '4') temp.append(H4_6_KEY_4);
            if(value_str.at(n) == '5') temp.append(H4_6_KEY_5);
            if(value_str.at(n) == '6') temp.append(H4_6_KEY_6);
            if(value_str.at(n) == '7') temp.append(H4_6_KEY_7);
            if(value_str.at(n) == '8') temp.append(H4_6_KEY_8);
            if(value_str.at(n) == '9') temp.append(H4_6_KEY_9);
            break;
        case H4_7:
            if(value_str.at(n) == '-') temp.append(H4_7_KEY_PLUS_MINUS);
            if(value_str.at(n) == '.') temp.append(H4_7_KEY_DOT);
            if(value_str.at(n) == '0') temp.append(H4_7_KEY_0);
            if(value_str.at(n) == '1') temp.append(H4_7_KEY_1);
            if(value_str.at(n) == '2') temp.append(H4_7_KEY_2);
            if(value_str.at(n) == '3') temp.append(H4_7_KEY_3);
            if(value_str.at(n) == '4') temp.append(H4_7_KEY_4);
            if(value_str.at(n) == '5') temp.append(H4_7_KEY_5);
            if(value_str.at(n) == '6') temp.append(H4_7_KEY_6);
            if(value_str.at(n) == '7') temp.append(H4_7_KEY_7);
            if(value_str.at(n) == '8') temp.append(H4_7_KEY_8);
            if(value_str.at(n) == '9') temp.append(H4_7_KEY_9);
            break;
        default:
            qFatal("bad type calibrator");
            break;
        }
    }
    return temp;
}
//--------------------------------------------------------------------------------
int Calibrator::get_type_calibrator(void)
{
    return type_calibrator;
}
//--------------------------------------------------------------------------------
void Calibrator::set_type_calibrator(int type_c)
{
    Q_ASSERT(type_c >= H4_6);
    Q_ASSERT(type_c <= H4_7);

    type_calibrator = type_c;
}
//--------------------------------------------------------------------------------
void Calibrator::print_last_error(void)
{
    switch(last_error)
    {
    case E_CALIBRATOR_PORT_NOT_OPEN:
        emit error("Порт калибратора не открыт!");
        break;

    case E_CALIBRATOR_ERROR:
        emit error("Ошибка калибратора!");
        break;

    default:
        emit error(QString("unknown error %1").arg(last_error));
        break;
    }
}
//--------------------------------------------------------------------------------
void Calibrator::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Calibrator::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Calibrator::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Calibrator::save_setting(void)
{

}
//--------------------------------------------------------------------------------
