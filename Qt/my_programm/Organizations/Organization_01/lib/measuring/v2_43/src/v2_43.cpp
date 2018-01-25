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
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "v2_43.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
V243::V243(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
V243::~V243()
{

}
//--------------------------------------------------------------------------------
void V243::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void V243::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data);
#endif
}
//--------------------------------------------------------------------------------
void V243::init(void)
{
    connect_log();

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));

    //connect(&serial, SIGNAL(output(QByteArray)), this, SLOT(raw_data(QByteArray)));
    //connect(this, SIGNAL(calibrator_send(QByteArray)), &serial, SLOT(sendData(QByteArray)));

#ifdef FAKE
    connect(&serial, SIGNAL(send(QVariant)), this, SIGNAL(send(QVariant)));
#endif
}
//--------------------------------------------------------------------------------
void V243::port_read(void)
{
    QByteArray ba = serial.readAll();
    if(ba.isEmpty() == false)
    {
        dirty_data.append(ba);
    }
}
//--------------------------------------------------------------------------------
void V243::port_error(QSerialPort::SerialPortError serial_error)
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
bool V243::open_device(const QString &port_name,
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
bool V243::send(QByteArray data)
{
    return true;
}
//--------------------------------------------------------------------------------
bool V243::send_wo_pause(QByteArray data)
{
    return true;
}
//--------------------------------------------------------------------------------
void V243::wait_msec(int timeout_msec)
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
bool V243::port_is_open(void)
{
    return serial.isOpen();
}
//--------------------------------------------------------------------------------
int V243::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
void V243::print_last_error(void)
{
    switch(last_error)
    {

    default:
        emit error(QString("unknown error %1").arg(last_error));
        break;
    }
}
//--------------------------------------------------------------------------------
