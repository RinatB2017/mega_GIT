/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include <QTime>
//--------------------------------------------------------------------------------
#include "serialbox5_thread.hpp"
//--------------------------------------------------------------------------------
SerialBox5_thread::SerialBox5_thread(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
SerialBox5_thread::~SerialBox5_thread()
{

}
//--------------------------------------------------------------------------------
void SerialBox5_thread::process(void)
{
    emit info("process");

    serial5 = new QSerialPort();
    if(serial5 == nullptr)
    {
        emit finished();
        return;
    }

//    serial_data.resize(100);
    while(!flag_exit)
    {
        if(serial5->waitForReadyRead(5000))
        {
//            serial_data = serial5->read(10000); //TODO maxSize
            serial_data = serial5->readAll(); //TODO maxSize
            if(!serial_data.isEmpty())
            {
                emit info(serial_data);
//                emit readyRead();
//                emit readChannelFinished();
            }
        }
    }
    emit finished();
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::serial5_error(QSerialPort::SerialPortError err)
{
    if(err == QSerialPort::NoError)
    {
        return;
    }

    switch(err)
    {
    case QSerialPort::DeviceNotFoundError:          emit error("DeviceNotFoundError");          break;
    case QSerialPort::PermissionError:              emit error("PermissionError");              break;
    case QSerialPort::OpenError:                    emit error("OpenError");                    break;
    case QSerialPort::ParityError:                  emit error("ParityError");                  break;
    case QSerialPort::FramingError:                 emit error("FramingError");                 break;
    case QSerialPort::BreakConditionError:          emit error("BreakConditionError");          break;
    case QSerialPort::WriteError:                   emit error("WriteError");                   break;
    case QSerialPort::ReadError:                    emit error("ReadError");                    break;
    case QSerialPort::ResourceError:                emit error("ResourceError");                break;
    case QSerialPort::UnsupportedOperationError:    emit error("UnsupportedOperationError");    break;
    case QSerialPort::UnknownError:                 emit error("UnknownError");                 break;
    case QSerialPort::TimeoutError:                 emit error("TimeoutError");                 break;
    case QSerialPort::NotOpenError:                 emit error("NotOpenError");                 break;

    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }

    if(err != QSerialPort::NoError)
    {
        if(serial5->isOpen())
        {
            serial5->close();
        }
    }

//    setCloseState();
//    refresh();
}
//--------------------------------------------------------------------------------
bool SerialBox5_thread::set_fix_baudrate(int value)
{
    bool ok = false;
    fix_baudrate = value;
    if(serial5)
    {
        ok = serial5->setBaudRate(value);
    }
    return ok;
}
//--------------------------------------------------------------------------------
qint64 SerialBox5_thread::bytesAvailable(void)
{
    return serial5->bytesAvailable();
}
//--------------------------------------------------------------------------------
qint64 SerialBox5_thread::write(const char *data)
{
    return serial5->write(data);
}
//--------------------------------------------------------------------------------
qint64 SerialBox5_thread::write(const char *data, qint64 len)
{
    return serial5->write(data, len);
}
//--------------------------------------------------------------------------------
bool SerialBox5_thread::isOpen(void)
{
    return serial5->isOpen();
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::close(void)
{
    serial5->close();
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::setPortName(const QString &name)
{
    serial5->setPortName(name);
}
//--------------------------------------------------------------------------------
bool SerialBox5_thread::setBaudRate(qint32 baudRate)
{
    return serial5->setBaudRate(baudRate);
}
//--------------------------------------------------------------------------------
bool SerialBox5_thread::open(QIODevice::OpenMode mode)
{
    return serial5->open(mode);
}
//--------------------------------------------------------------------------------
QString SerialBox5_thread::portName(void)
{
    return serial5->portName();
}
//--------------------------------------------------------------------------------
QString SerialBox5_thread::errorString(void)
{
    return serial5->errorString();
}
//--------------------------------------------------------------------------------
QByteArray SerialBox5_thread::readAll(void)
{
    return serial5->readAll();
}
//--------------------------------------------------------------------------------
qint32 SerialBox5_thread::baudRate(void)
{
    return serial5->baudRate();
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits SerialBox5_thread::dataBits(void)
{
    return serial5->dataBits();
}
//--------------------------------------------------------------------------------
QSerialPort::Parity	SerialBox5_thread::parity(void)
{
    return serial5->parity();
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits SerialBox5_thread::stopBits(void)
{
    return serial5->stopBits();
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl SerialBox5_thread::flowControl(void)
{
    return serial5->flowControl();
}
//--------------------------------------------------------------------------------
void SerialBox5_thread::start(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5_thread::stop(void)
{
    flag_exit = true;
}
//--------------------------------------------------------------------------------
