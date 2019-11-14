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
    SerialBox5_thread(QObject *parent = nullptr);
    ~SerialBox5_thread();

    bool set_fix_baudrate(int value);
    qint64 bytesAvailable(void);
    qint64 write(const char *data);
    qint64 write(const char *data, qint64 len);
    bool isOpen(void);
    void close(void);
    void setPortName(const QString &name);
    bool setBaudRate(qint32 baudRate);
    bool open(QIODevice::OpenMode mode);
    QString portName(void);
    QString errorString(void);
    QByteArray readAll(void);

    qint32 baudRate(void);
    QSerialPort::DataBits dataBits(void);
    QSerialPort::Parity	parity(void);
    QSerialPort::StopBits stopBits(void);
    QSerialPort::FlowControl flowControl(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void finished(void);

    void readyRead(void);
    void readChannelFinished(void);
    void error(QSerialPort::SerialPortError);

public slots:
    void process(void);

    void start(void);
    void stop(void);

private:
    bool flag_exit = false;
    int fix_baudrate = 9600;

    QByteArray serial_data;

    QSerialPort *serial5 = nullptr;
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX5_THREAD_HPP
