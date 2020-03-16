/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                      **
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
#ifndef FAKE_SERIALBOX5_HPP
#define FAKE_SERIALBOX5_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>

#include <QAbstractSocket>
//--------------------------------------------------------------------------------
class FakeSerialBox5 : public QWidget
{
    Q_OBJECT

signals:
    void readyRead(void);
    void readChannelFinished(void);
    void error(QSerialPort::SerialPortError);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    //TODO
    void fake_output(const QByteArray &data);

public slots:
    //TODO
    int fake_input(const QByteArray &sending_data);
    int fake_input(const QString &data);

public:
    FakeSerialBox5(QWidget *parent = nullptr);

    bool setBaudRate(qint32 baudRate, QSerialPort::Directions directions = QSerialPort::AllDirections);
    qint64 bytesAvailable(void);

    QString portName(void);
    void setPortName(const QString &name);
    QString errorString(void);

    bool isOpen(void);
    bool close(void);
    bool open(QIODevice::OpenMode mode);
    qint64 write(const char *data, qint64 maxSize);
    qint64 write(const char *data);
    qint64 write(const QByteArray &byteArray);
    QByteArray readAll(void);
    qint32 baudRate(QSerialPort::Directions directions = QSerialPort::AllDirections);
    QSerialPort::DataBits dataBits(void);
    QSerialPort::Parity	parity(void);
    QSerialPort::StopBits stopBits(void);
    QSerialPort::FlowControl flowControl(void);

private:
    bool is_open = false;
    QByteArray fake_data;
};
//--------------------------------------------------------------------------------
#endif
