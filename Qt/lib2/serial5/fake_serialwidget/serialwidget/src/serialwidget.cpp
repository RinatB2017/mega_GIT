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
// FAKE
//--------------------------------------------------------------------------------
#include "serialwidget.hpp"
//--------------------------------------------------------------------------------
SerialWidget::SerialWidget(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
SerialWidget::~SerialWidget()
{
    if(worker_fake)
    {
        worker_fake->disconnect();
        worker_fake->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void SerialWidget::init(void)
{
    worker_fake = new Worker_fake(this);
    worker_fake->show();

    connect(worker_fake,    &Worker_fake::output,       this,           &SerialWidget::output);
    connect(this,           &SerialWidget::port_open,   worker_fake,    &Worker_fake::port_open);
    connect(this,           &SerialWidget::port_close,  worker_fake,    &Worker_fake::port_close);
}
//--------------------------------------------------------------------------------
bool SerialWidget::isOpen(void)
{
    return f_port_open;
}
//--------------------------------------------------------------------------------
bool SerialWidget::serial_open(void)
{
    f_port_open = true;
    emit port_open();
    return f_port_open;
}
//--------------------------------------------------------------------------------
bool SerialWidget::serial_close(void)
{
    f_port_open = false;
    emit port_close();
    return true;
}
//--------------------------------------------------------------------------------
QByteArray SerialWidget::readAll(void)
{
    return port_data;
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::bytesAvailable(void)
{
    return port_data.count();
}
//--------------------------------------------------------------------------------
void SerialWidget::setPortName(QString name)
{
    if(name.isEmpty())  return;
    port_name = name;
}
//--------------------------------------------------------------------------------
bool SerialWidget::setBaudRate(qint32 value)
{
    port_BaudRate = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialWidget::setDataBits(QSerialPort::DataBits value)
{
    port_DataBits = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialWidget::setParity(QSerialPort::Parity value)
{
    port_Parity = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialWidget::setStopBits(QSerialPort::StopBits value)
{
    port_StopBits = value;
    return true;
}
//--------------------------------------------------------------------------------
bool SerialWidget::setFlowControl(QSerialPort::FlowControl value)
{
    port_FlowControl = value;
    return true;
}
//--------------------------------------------------------------------------------
QString SerialWidget::portName(void)
{
    return port_name;
}
//--------------------------------------------------------------------------------
int SerialWidget::baudRate(void)
{
    return port_BaudRate;
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits SerialWidget::dataBits(void)
{
    return port_DataBits;
}
//--------------------------------------------------------------------------------
QSerialPort::Parity SerialWidget::parity(void)
{
    return port_Parity;
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits SerialWidget::stopBits(void)
{
    return port_StopBits;
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl SerialWidget::flowControl(void)
{
    return port_FlowControl;
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const char *data, qint64 len)
{
    port_data.append(data, static_cast<int>(len));
    worker_fake->input(data);
//    emit readyRead();
//    emit output(readAll());
    return true;
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const char *data)
{
    port_data.append(data);
    worker_fake->input(data);
//    emit readyRead();
//    emit output(readAll());
    return true;
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const QByteArray &data)
{
    port_data.append(data);
    worker_fake->input(data);
//    emit readyRead();
//    emit output(readAll());
    return true;
}
//--------------------------------------------------------------------------------
QStringList SerialWidget::get_port_names(void)
{
    QStringList sl_ports;
    sl_ports.append(port_name);
    return sl_ports;
}
//--------------------------------------------------------------------------------
QString SerialWidget::errorString(void)
{
    return "no error";
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
