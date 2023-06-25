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
#ifndef SERIALWIDGET_HPP
#define SERIALWIDGET_HPP
//--------------------------------------------------------------------------------
#define MAX_TIME_MSEC   100
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
class SerialWidget : public MyWidget
{
    Q_OBJECT

signals:
    void port_open(void);
    void port_close(void);
    void readyRead(void);
    void readChannelFinished(void);
    void output(const QByteArray &data);    // выходные данные
    void port_is_active(bool);

    void s_baudRateChanged(qint32 baudRate, QSerialPort::Directions directions);
    void s_breakEnabledChanged(bool set);
    void s_dataBitsChanged(QSerialPort::DataBits dataBits);
    void s_dataTerminalReadyChanged(bool set);
    void s_flowControlChanged(QSerialPort::FlowControl flowControl);
    void s_parityChanged(QSerialPort::Parity parity);
    void s_requestToSendChanged(bool set);
    void s_stopBitsChanged(QSerialPort::StopBits stopBits);

public:
    explicit SerialWidget(QWidget *parent = nullptr);
    virtual ~SerialWidget();

    bool serial_open(void);
    bool serial_close(void);

    void setPortName(QString name);
    bool setBaudRate(qint32 value);
    bool setDataBits(QSerialPort::DataBits value);
    bool setParity(QSerialPort::Parity value);
    bool setStopBits(QSerialPort::StopBits value);
    bool setFlowControl(QSerialPort::FlowControl value);

//    void setSettingsRestoredOnClose(bool state);

    QString portName(void);
    int baudRate(void);
    QSerialPort::DataBits dataBits(void);
    QSerialPort::Parity parity(void);
    QSerialPort::StopBits stopBits(void);
    QSerialPort::FlowControl flowControl(void);

    QString errorString(void);

    bool isOpen(void);
    QByteArray readAll(void);
    qint64 bytesAvailable(void);

    qint64 write(const char *data, qint64 len);
    qint64 write(const char *data);
    qint64 write(const QByteArray &data);

    QStringList get_port_names(void);

    void procSerialDataReceive(void);

private slots:
    void serial5_error(QSerialPort::SerialPortError err);
    void timer_stop(void);

private:
    QSerialPort *serial5 = nullptr;
    QTimer *timer = nullptr;

    void init(void);

    void baudRateChanged(qint32 baudRate, QSerialPort::Directions directions);
    void breakEnabledChanged(bool set);
    void dataBitsChanged(QSerialPort::DataBits dataBits);
    void dataTerminalReadyChanged(bool set);
    void flowControlChanged(QSerialPort::FlowControl flowControl);
    void parityChanged(QSerialPort::Parity parity);
    void requestToSendChanged(bool set);
    void stopBitsChanged(QSerialPort::StopBits stopBits);
};
//--------------------------------------------------------------------------------
#endif // SERIALWIDGET_HPP
