/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
#include "serialwidget.hpp"
//--------------------------------------------------------------------------------
SerialWidget::SerialWidget(QWidget *parent) :
    MyWidget(parent)
{
    init();;
}
//--------------------------------------------------------------------------------
SerialWidget::~SerialWidget()
{
    if(timer)
    {
        timer->stop();
        disconnect(timer,  &QTimer::timeout,  this,   &SerialWidget::timer_stop);

        timer->deleteLater();
    }
    if(serial5)
    {
        disconnect(serial5, &QSerialPort::errorOccurred,               this,   &SerialWidget::serial5_error);

        serial5->close();
        serial5->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void SerialWidget::init(void)
{
    serial5 = new QSerialPort(this);

    //TODO
    timer = new QTimer(this);
    connect(timer,  &QTimer::timeout,  this,   &SerialWidget::timer_stop);

    connect(serial5, &QSerialPort::readyRead,                   this,   &SerialWidget::procSerialDataReceive);
    connect(serial5, &QSerialPort::errorOccurred,               this,   &SerialWidget::serial5_error);

    connect(serial5, &QSerialPort::baudRateChanged,             this,   &SerialWidget::baudRateChanged);
    connect(serial5, &QSerialPort::breakEnabledChanged,         this,   &SerialWidget::breakEnabledChanged);
    connect(serial5, &QSerialPort::dataBitsChanged,             this,   &SerialWidget::dataBitsChanged);
    connect(serial5, &QSerialPort::dataTerminalReadyChanged,    this,   &SerialWidget::dataTerminalReadyChanged);
    connect(serial5, &QSerialPort::flowControlChanged,          this,   &SerialWidget::flowControlChanged);
    connect(serial5, &QSerialPort::parityChanged,               this,   &SerialWidget::parityChanged);
    connect(serial5, &QSerialPort::requestToSendChanged,        this,   &SerialWidget::requestToSendChanged);
    connect(serial5, &QSerialPort::stopBitsChanged,             this,   &SerialWidget::stopBitsChanged);

    QTimer::singleShot(0, [this]{
        emit port_is_active(isOpen());
    });
}
//--------------------------------------------------------------------------------
bool SerialWidget::isOpen(void)
{
    Q_ASSERT(serial5);
    return serial5->isOpen();
}
//--------------------------------------------------------------------------------
bool SerialWidget::serial_open(void)
{
    Q_ASSERT(serial5);
    return serial5->open(QIODevice::ReadWrite);
}
//--------------------------------------------------------------------------------
bool SerialWidget::serial_close(void)
{
    Q_ASSERT(serial5);
    serial5->close();
    return true;
}
//--------------------------------------------------------------------------------
QByteArray SerialWidget::readAll(void)
{
    Q_ASSERT(serial5);
    return serial5->readAll();
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::bytesAvailable(void)
{
    Q_ASSERT(serial5);
    return serial5->bytesAvailable();
}
//--------------------------------------------------------------------------------
void SerialWidget::setPortName(QString name)
{
    Q_ASSERT(serial5);
    if(name.isEmpty())  return;
    serial5->setPortName(name);
}
//--------------------------------------------------------------------------------
bool SerialWidget::setBaudRate(qint32 value)
{
    Q_ASSERT(serial5);
    return serial5->setBaudRate(value);
}
//--------------------------------------------------------------------------------
//void SerialWidget::setSettingsRestoredOnClose(bool state)
//{
//    Q_ASSERT(serial5);
//    serial5->setSettingsRestoredOnClose(state);
//}
//--------------------------------------------------------------------------------
bool SerialWidget::setDataBits(QSerialPort::DataBits value)
{
    Q_ASSERT(serial5);
    return serial5->setDataBits(value);
}
//--------------------------------------------------------------------------------
bool SerialWidget::setParity(QSerialPort::Parity value)
{
    Q_ASSERT(serial5);
    return serial5->setParity(value);
}
//--------------------------------------------------------------------------------
bool SerialWidget::setStopBits(QSerialPort::StopBits value)
{
    Q_ASSERT(serial5);
    return serial5->setStopBits(value);
}
//--------------------------------------------------------------------------------
bool SerialWidget::setFlowControl(QSerialPort::FlowControl value)
{
    Q_ASSERT(serial5);
    return serial5->setFlowControl(value);
}
//--------------------------------------------------------------------------------
QString SerialWidget::portName(void)
{
    Q_ASSERT(serial5);
    return serial5->portName();
}
//--------------------------------------------------------------------------------
int SerialWidget::baudRate(void)
{
    Q_ASSERT(serial5);
    return serial5->baudRate();
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits SerialWidget::dataBits(void)
{
    Q_ASSERT(serial5);
    return serial5->dataBits();
}
//--------------------------------------------------------------------------------
QSerialPort::Parity SerialWidget::parity(void)
{
    Q_ASSERT(serial5);
    return serial5->parity();
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits SerialWidget::stopBits(void)
{
    Q_ASSERT(serial5);
    return serial5->stopBits();
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl SerialWidget::flowControl(void)
{
    Q_ASSERT(serial5);
    return serial5->flowControl();
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const char *data, qint64 len)
{
    Q_ASSERT(serial5);
    return serial5->write(data, len);
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const char *data)
{
    Q_ASSERT(serial5);
    return serial5->write(data);
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const QByteArray &data)
{
    Q_ASSERT(serial5);
    return serial5->write(data);
}
//--------------------------------------------------------------------------------
QStringList SerialWidget::get_port_names(void)
{
    QStringList sl_ports;
    foreach (QSerialPortInfo p_info, QSerialPortInfo::availablePorts())
    {
        sl_ports.append(p_info.portName());
    }
    return sl_ports;
}
//--------------------------------------------------------------------------------
void SerialWidget::procSerialDataReceive(void)
{
    if(!isOpen())
    {
        return;
    }

    if (!timer->isActive())
        timer->singleShot(MAX_TIME_MSEC, this, &SerialWidget::timer_stop);
    else
        timer->stop();
}
//--------------------------------------------------------------------------------
void SerialWidget::serial5_error(QSerialPort::SerialPortError err)
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
#ifdef Q_OS_LINUX
    case QSerialPort::ParityError:                  emit error("ParityError");                  break;
    case QSerialPort::FramingError:                 emit error("FramingError");                 break;
    case QSerialPort::BreakConditionError:          emit error("BreakConditionError");          break;
#endif
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

    if(err != QSerialPort::NoError && err != QSerialPort::ReadError)
    {
        if(serial5->isOpen())
        {
            serial5->close();
        }
        emit port_close();
    }
}
//--------------------------------------------------------------------------------
void SerialWidget::timer_stop(void)
{
    emit output(readAll()); //TODO проверить
}
//--------------------------------------------------------------------------------
QString SerialWidget::errorString(void)
{
    return serial5->errorString();
}
//--------------------------------------------------------------------------------
void SerialWidget::baudRateChanged(qint32 baudRate, QSerialPort::Directions directions)
{
    emit s_baudRateChanged(baudRate, directions);
}
//--------------------------------------------------------------------------------
void SerialWidget::breakEnabledChanged(bool set)
{
    emit s_breakEnabledChanged(set);
}
//--------------------------------------------------------------------------------
void SerialWidget::dataBitsChanged(QSerialPort::DataBits dataBits)
{
    emit s_dataBitsChanged(dataBits);
}
//--------------------------------------------------------------------------------
void SerialWidget::dataTerminalReadyChanged(bool set)
{
    emit s_dataTerminalReadyChanged(set);
}
//--------------------------------------------------------------------------------
void SerialWidget::flowControlChanged(QSerialPort::FlowControl flowControl)
{
    emit s_flowControlChanged(flowControl);
}
//--------------------------------------------------------------------------------
void SerialWidget::parityChanged(QSerialPort::Parity parity)
{
    emit s_parityChanged(parity);
}
//--------------------------------------------------------------------------------
void SerialWidget::requestToSendChanged(bool set)
{
    emit s_requestToSendChanged(set);
}
//--------------------------------------------------------------------------------
void SerialWidget::stopBitsChanged(QSerialPort::StopBits stopBits)
{
    emit s_stopBitsChanged(stopBits);
}
//--------------------------------------------------------------------------------
