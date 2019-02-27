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
#include <QByteArray>
#include <QTime>
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
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Calibrator::Calibrator(QWidget *parent) :
    QWidget(parent)
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

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

    //connect(&serial, SIGNAL(output(QByteArray)), this, SLOT(raw_data(QByteArray)));
    //connect(this, SIGNAL(calibrator_send(QByteArray)), &serial, SLOT(sendData(QByteArray)));

#ifdef FAKE
    connect(&serial, SIGNAL(send(QVariant)), this, SIGNAL(send(QVariant)));
#endif
}
//--------------------------------------------------------------------------------
void Calibrator::port_read(void)
{
    QByteArray ba = serial.readAll();
    if(ba.isEmpty() == false)
    {
        data_calibrator.append(ba);
    }
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
bool Calibrator::open_device(const QString &port_name,
                             QSerialPort::FlowControl flow)
{
    int speed = 9600;
    if(serial.isOpen())
    {
        serial.close();
    }
    foreach(const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        if(port_name == port.portName())
        {
            emit info(QString(tr("Порт %1")).arg(port.portName()));
            serial.setPort(port);
            bool ok = serial.setBaudRate(speed);
            if(!ok)
            {
                emit error(QString(tr("setBaudRate(%1) is failed!")).arg(speed));
                return false;
            }
            ok = serial.setFlowControl(flow);
            if(!ok)
            {
                emit error(QString(tr("setFlowControl(%1) is failed!")).arg(flow));
                return false;
            }
            if(serial.open(QIODevice::ReadWrite))
            {
                emit info(QString(tr("Калибратор выбран на порту %1")).arg(port.portName()));
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Calibrator::send(QByteArray data)
{
    emit debug(QString("%1").arg(data.data()));

    if(serial.isOpen() == false)
    {
        emit error(tr("Порт калибратора не открыт!"));
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:send timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    // is_ready = false;
    serial.write(data);
#ifndef FAKE
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::send_wo_pause(QByteArray data)
{
    emit debug(QString("%1").arg(data.data()));

    if(serial.isOpen() == false)
    {
        emit error(tr("Порт калибратора не открыт!"));
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:send timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    // is_ready = false;
    serial.write(data);

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
void Calibrator::wait_msec(int timeout_msec)
{
    QTime time;

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
    }
}
//--------------------------------------------------------------------------------
bool Calibrator::check_calibrator(int max_error)
{
    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:check_calibrator timeout == %1 ms").arg(timeout));
    }

    int cnt_error = 0;

    while(cnt_error < max_error)
    {
        data_calibrator.clear();
        // is_ready = false;
        serial.write(QByteArray("T0\r\n"));
#ifndef FAKE
        wait_msec(timeout);
#endif

        QString temp = data_calibrator.left(2).data();
        if(temp.isEmpty())
        {
            emit error(QString(tr("Калибратор не отвечает (команда Т0, время %1)")).arg(timeout));
            cnt_error++;
        }
        else
        {
            for(int n=0; n<temp.length(); n++)
            {
                if(temp.at(n).isDigit() == false)
                    temp.data()[n] = 'X';
            }
            if(temp != "00")
            {
                emit error(QString(tr("Ошибка %1").arg(temp)));
                cnt_error++;
            }
            else
            {
                return true;
            }
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
        emit error(tr("calibrator_step_to_step 1: text is Empty"));
        return false;
    }

    text = text.remove(' ');

    if(text.isEmpty())
    {
        emit error(tr("calibrator_step_to_step 2: text is Empty"));
        return false;
    }
    if(text.length() % 2)
    {
        emit error(tr("calibrator_step_to_step 3: text is Empty"));
        return false;
    }

    for(int n=0; n<text.length(); n+=2)
    {
        QByteArray temp;
        temp.append(text.mid(n, 2));
        temp.append((char)0x0D);
#ifndef FAKE
        send_wo_pause(temp);
        wait_msec(step_to_step_timeout);
#else
        Q_UNUSED(step_to_step_timeout)
#endif
    }

    if(no_delay == false)
    {
#ifndef FAKE
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
    if(serial.isOpen() == false)
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
    data.clear();
    data.append("UD0\r\n");
    serial.write(data);
#ifndef FAKE
    wait_msec(timeout);
#endif
    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::reset_I(void)
{
    if(serial.isOpen() == false)
    {
        emit error(tr("Порт калибратора не открыт!"));
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
    data.append("ID0\r\n");
    serial.write(data);
#ifndef FAKE
    wait_msec(timeout);
#endif
    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::check_calibrator(void)
{
    if(serial.isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    emit info(tr("Тест калибратора"));
    bool ok = check_calibrator(1);
    if(!ok)
    {
        emit error(tr("Калибратор не найден"));
        last_error = E_CALIBRATOR_ERROR;
        return false;
    }

    emit info(tr("Калибратор найден"));
    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::port_is_open(void)
{
    return serial.isOpen();
}
//--------------------------------------------------------------------------------
bool Calibrator::set_U_V(float value)
{
    emit debug(QString("value %1").arg(value));

    if(serial.isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_U timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QString temp = QString("UD%1\r\n").arg(value);
    emit debug(temp);
    serial.write(temp.toLocal8Bit());
#ifndef FAKE
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_U_V(float value, float freq)
{
    emit debug(QString("value %1 freq %2").arg(value).arg(freq));

    if(serial.isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_U(freq) timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QString temp = QString("UA%1F%2\r\n").arg(value).arg(freq);
    emit debug(temp);
    serial.write(temp.toLocal8Bit());
#ifndef FAKE
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_I_mA(float value)
{
    emit debug(QString("value %1").arg(value));

    if(serial.isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_I timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QString temp = QString("ID%1\r\n").arg(value);
    emit debug(temp);
    serial.write(temp.toLocal8Bit());
#ifndef FAKE
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_I_mA(float value, float freq)
{
    emit debug(QString("value %1 freq %2").arg(value).arg(freq));

    if(serial.isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_I(freq) timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    QString temp = QString("IA%1F%2\r\n").arg(value).arg(freq);
    emit debug(temp);
    serial.write(temp.toLocal8Bit());
#ifndef FAKE
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
bool Calibrator::set_R(unsigned int value)
{
    emit debug(QString("value %1").arg(value));

    if(serial.isOpen() == false)
    {
        last_error = E_CALIBRATOR_PORT_NOT_OPEN;
        return false;
    }

    if(timeout < MIN_TIME_CALIBRATION)
    {
        emit error(QString("Calibration:set_R timeout == %1 ms").arg(timeout));
    }

    data_calibrator.clear();
    serial.write(QString("R%1\r\n").arg(value).toLocal8Bit());
#ifndef FAKE
    wait_msec(timeout);
#endif

    last_error = E_CALIBRATOR_NO_ERROR;
    return true;
}
//--------------------------------------------------------------------------------
void Calibrator::set_timeout(unsigned int value)
{
    timeout = value;
}
//--------------------------------------------------------------------------------
int Calibrator::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
void Calibrator::print_last_error(void)
{
    switch(last_error)
    {
    case E_CALIBRATOR_PORT_NOT_OPEN:
        emit error(tr("Порт калибратора не открыт!"));
        break;

    case E_CALIBRATOR_ERROR:
        emit error(tr("Ошибка калибратора!"));
        break;

    default:
        emit error(QString("unknown error %1").arg(last_error));
        break;
    }
}
//--------------------------------------------------------------------------------
