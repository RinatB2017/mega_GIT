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
// TEST
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
#ifdef USE_B588
    if(worker_fake_b588)
    {
        delete worker_fake_b588;
    }
#endif

#ifdef USE_B590
    if(worker_fake_b590)
    {
        delete worker_fake_b590;
    }
#endif

#ifdef V764
    if(worker_fake_v764)
    {
        delete worker_fake_v764;
    }
#endif

#ifdef V7642
    if(worker_fake_v764_2)
    {
        delete worker_fake_v764_2;
    }
#endif

#ifdef V780
    if(worker_fake_v780)
    {
        delete worker_fake_v780;
    }
#endif

#ifdef V7862
    if(worker_fake_v786_2)
    {
        delete worker_fake_v786_2;
    }
#endif
}
//--------------------------------------------------------------------------------
void SerialWidget::init(void)
{
#ifdef USE_B588
    worker_fake_b588 = new Worker_fake_b588(this);

    connect(worker_fake_b588,   &Worker_fake_b588::info,        this,               &SerialWidget::info);
    connect(worker_fake_b588,   &Worker_fake_b588::debug,       this,               &SerialWidget::debug);
    connect(worker_fake_b588,   &Worker_fake_b588::error,       this,               &SerialWidget::error);
    connect(worker_fake_b588,   &Worker_fake_b588::trace,       this,               &SerialWidget::trace);

    connect(worker_fake_b588,   &Worker_fake_b588::readyRead,   this,               &SerialWidget::readyRead);

    connect(worker_fake_b588,   &Worker_fake_b588::output,      this,               &SerialWidget::output);
    connect(worker_fake_b588,   &Worker_fake_b588::output,      this,               &SerialWidget::write_ba_output);

    connect(this,               &SerialWidget::port_open,       worker_fake_b588,   &Worker_fake_b588::port_open);
    connect(this,               &SerialWidget::port_close,      worker_fake_b588,   &Worker_fake_b588::port_close);
#endif

#ifdef USE_B590
    worker_fake_b590 = new Worker_fake_b590(this);
    
    connect(worker_fake_b590,   &Worker_fake_b590::info,        this,               &SerialWidget::info);
    connect(worker_fake_b590,   &Worker_fake_b590::debug,       this,               &SerialWidget::debug);
    connect(worker_fake_b590,   &Worker_fake_b590::error,       this,               &SerialWidget::error);
    connect(worker_fake_b590,   &Worker_fake_b590::trace,       this,               &SerialWidget::trace);

    connect(worker_fake_b590,   &Worker_fake_b590::readyRead,   this,               &SerialWidget::readyRead);

    connect(worker_fake_b590,   &Worker_fake_b590::output,      this,               &SerialWidget::output);
    connect(worker_fake_b590,   &Worker_fake_b590::output,      this,               &SerialWidget::write_ba_output);

    connect(this,               &SerialWidget::port_open,       worker_fake_b590,   &Worker_fake_b590::port_open);
    connect(this,               &SerialWidget::port_close,      worker_fake_b590,   &Worker_fake_b590::port_close);
#endif

#ifdef V7862
    worker_fake_v786_2 = new Worker_fake_v786_2(this);

    connect(worker_fake_v786_2,     &Worker_fake_v786_2::info,      this,               &SerialWidget::info);
    connect(worker_fake_v786_2,     &Worker_fake_v786_2::debug,     this,               &SerialWidget::debug);
    connect(worker_fake_v786_2,     &Worker_fake_v786_2::error,     this,               &SerialWidget::error);
    connect(worker_fake_v786_2,     &Worker_fake_v786_2::trace,     this,               &SerialWidget::trace);

    connect(worker_fake_v786_2,     &Worker_fake_v786_2::readyRead, this,               &SerialWidget::readyRead);

    connect(worker_fake_v786_2,     &Worker_fake_v786_2::output,    this,               &SerialWidget::output);
    connect(worker_fake_v786_2,     &Worker_fake_v786_2::output,    this,               &SerialWidget::write_ba_output);

    connect(this,                   &SerialWidget::port_open,       worker_fake_v786_2, &Worker_fake_v786_2::port_open);
    connect(this,                   &SerialWidget::port_close,      worker_fake_v786_2, &Worker_fake_v786_2::port_close);

    QTimer::singleShot(0, [this]{
       worker_fake_v786_2->setObjectName(objectName());
    });
    worker_fake_v786_2->show();
#endif

#ifdef V764
    worker_fake_v764 = new Worker_fake_v764(this);

    connect(worker_fake_v764,   &Worker_fake_v764::info,        this,               &SerialWidget::info);
    connect(worker_fake_v764,   &Worker_fake_v764::debug,       this,               &SerialWidget::debug);
    connect(worker_fake_v764,   &Worker_fake_v764::error,       this,               &SerialWidget::error);
    connect(worker_fake_v764,   &Worker_fake_v764::trace,       this,               &SerialWidget::trace);

    connect(worker_fake_v764,   &Worker_fake_v764::readyRead,   this,               &SerialWidget::readyRead);

    connect(worker_fake_v764,   &Worker_fake_v764::output,      this,               &SerialWidget::output);
    connect(worker_fake_v764,   &Worker_fake_v764::output,      this,               &SerialWidget::write_ba_output);

    connect(this,               &SerialWidget::port_open,       worker_fake_v764,   &Worker_fake_v764::port_open);
    connect(this,               &SerialWidget::port_close,      worker_fake_v764,   &Worker_fake_v764::port_close);

    QTimer::singleShot(0, [this]{
       worker_fake_v764->setObjectName(objectName());
    });
    worker_fake_v764->show();
#endif

#ifdef V7642
    worker_fake_v764_2 = new Worker_fake_v764_2(this);

    connect(worker_fake_v764_2, &Worker_fake_v764_2::info,      this,               &SerialWidget::info);
    connect(worker_fake_v764_2, &Worker_fake_v764_2::debug,     this,               &SerialWidget::debug);
    connect(worker_fake_v764_2, &Worker_fake_v764_2::error,     this,               &SerialWidget::error);
    connect(worker_fake_v764_2, &Worker_fake_v764_2::trace,     this,               &SerialWidget::trace);

    connect(worker_fake_v764_2, &Worker_fake_v764_2::readyRead, this,               &SerialWidget::readyRead);

    connect(worker_fake_v764_2, &Worker_fake_v764_2::output,    this,               &SerialWidget::output);
    connect(worker_fake_v764_2, &Worker_fake_v764_2::output,    this,               &SerialWidget::write_ba_output);

    connect(this,               &SerialWidget::port_open,       worker_fake_v764_2, &Worker_fake_v764_2::port_open);
    connect(this,               &SerialWidget::port_close,      worker_fake_v764_2, &Worker_fake_v764_2::port_close);

    QTimer::singleShot(0, [this]{
       worker_fake_v764_2->setObjectName(objectName());
    });
    worker_fake_v764_2->show();
#endif

#ifdef V780
    worker_fake_v780 = new Worker_fake_v780(this);

    connect(worker_fake_v780,    &Worker_fake_v780::info,        this,           &SerialWidget::info);
    connect(worker_fake_v780,    &Worker_fake_v780::debug,       this,           &SerialWidget::debug);
    connect(worker_fake_v780,    &Worker_fake_v780::error,       this,           &SerialWidget::error);
    connect(worker_fake_v780,    &Worker_fake_v780::trace,       this,           &SerialWidget::trace);

    connect(worker_fake_v780,    &Worker_fake_v780::readyRead,   this,           &SerialWidget::readyRead);

    connect(worker_fake_v780,    &Worker_fake_v780::output,      this,           &SerialWidget::output);
    connect(worker_fake_v780,    &Worker_fake_v780::output,      this,           &SerialWidget::write_ba_output);

    connect(this,           &SerialWidget::port_open,       worker_fake_v780,    &Worker_fake_v780::port_open);
    connect(this,           &SerialWidget::port_close,      worker_fake_v780,    &Worker_fake_v780::port_close);

    QTimer::singleShot(0, [this]{
       worker_fake_v780->setObjectName(objectName());
    });
    worker_fake_v780->show();
#endif
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
void SerialWidget::write_ba_output(QByteArray ba)
{
    ba_output.clear();
    ba_output.append(ba);
}
//--------------------------------------------------------------------------------
QByteArray SerialWidget::readAll(void)
{
    return ba_output;
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::bytesAvailable(void)
{
    return ba_output.count();
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
    ba_input.clear();
    ba_input.append(data, static_cast<int>(len));
#ifdef USE_B588
    worker_fake_b588->input(ba_input);
#endif
#ifdef USE_B590
    worker_fake_b590->input(ba_input);
#endif
#ifdef V764
    worker_fake_v764->input(ba_input);
#endif
#ifdef V7642
    worker_fake_v764_2->input(ba_input);
#endif
#ifdef V780
    worker_fake_v780->input(ba_input);
#endif
#ifdef V7862
    worker_fake_v786_2->input(ba_input);
#endif
    return true;
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const char *data)
{
    ba_input.clear();
    ba_input.append(data);
#ifdef USE_B588
    worker_fake_b588->input(ba_input);
#endif
#ifdef USE_B590
    worker_fake_b590->input(ba_input);
#endif
#ifdef V764
    worker_fake_v764->input(ba_input);
#endif
#ifdef V7642
    worker_fake_v764_2->input(ba_input);
#endif
#ifdef V780
    worker_fake_v780->input(ba_input);
#endif
#ifdef V7862
    worker_fake_v786_2->input(ba_input);
#endif
    return true;
}
//--------------------------------------------------------------------------------
qint64 SerialWidget::write(const QByteArray &data)
{
    ba_input.clear();
    ba_input.append(data);
#ifdef USE_B588
    worker_fake_b588->input(ba_input);
#endif
#ifdef USE_B590
    worker_fake_b590->input(ba_input);
#endif
#ifdef V764
    worker_fake_v764->input(ba_input);
#endif
#ifdef V7642
    worker_fake_v764_2->input(ba_input);
#endif
#ifdef V780
    worker_fake_v780->input(ba_input);
#endif
#ifdef V7862
    worker_fake_v786_2->input(ba_input);
#endif
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
