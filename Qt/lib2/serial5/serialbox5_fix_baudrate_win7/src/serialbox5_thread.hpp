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
#ifndef SERIALBOX5_THREAD_HPP
#define SERIALBOX5_THREAD_HPP
//--------------------------------------------------------------------------------
#include <QDateTime>
#include <QWidget>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
class SerialBox5_thread : public QObject
{
    Q_OBJECT

public:
    explicit SerialBox5_thread(QObject *parent = nullptr);
    ~SerialBox5_thread();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void finished(void);

    void readyRead(void);
    void readChannelFinished(void);

    void port_bytes_avialable(void);
    void port_get_state(bool state);
    void port_get_name(QString port_name);
    void port_get_baudrate(qint32 value);
    void port_get_bits(QSerialPort::DataBits value);
    void port_get_stop_bits(QSerialPort::StopBits value);
    void port_get_patity(QSerialPort::Parity value);
    void port_get_flow_control(QSerialPort::FlowControl value);
    void port_read_all(QByteArray data);
    void port_ready_read(bool state);
    void port_error(QSerialPort::SerialPortError err);

public slots:
    void process(void);

    void port_open(void);
    void port_close(void);
    void port_set_name(QString name);
    void port_set_baudrate(qint32 value);
    void port_set_bits(QSerialPort::DataBits value);
    void port_set_stop_bits(QSerialPort::StopBits value);
    void port_set_parity(QSerialPort::Parity value);
    void port_set_flow_control(QSerialPort::FlowControl value);
    void port_write(const char *data);
    void port_write(const char *data, qint64 maxSize);

    void start(void);
    void stop(void);

private:
    enum STATES {
        OPEN = 0,
        CLOSE,
        IDLE
    };
    int state = IDLE;

    bool flag_exit = false;
    int fix_baudrate = 9600;

    QString port_name;
    qint32 baudrate;
    QSerialPort::DataBits data_bits;
    QSerialPort::StopBits stop_bits;
    QSerialPort::Parity parity;
    QSerialPort::FlowControl flow_control;

    bool flag_port_open = false;

    bool flag_port_name = false;
    bool flag_baudrate = false;
    bool flag_data_bits = false;
    bool flag_stop_bits = false;
    bool flag_parity = false;
    bool flag_flow_control = false;

    QByteArray serial_data;

    QSerialPort *serial5 = nullptr;
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX5_THREAD_HPP
