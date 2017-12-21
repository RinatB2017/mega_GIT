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
#ifndef FAKESERIAL_HPP
#define FAKESERIAL_HPP
//--------------------------------------------------------------------------------
#include <QStringList>
#include <QByteArray>
#include <QDateTime>
#include <QIODevice>
#include <QString>
#include <QWidget>
//--------------------------------------------------------------------------------
class Emulator_V786_2;
//--------------------------------------------------------------------------------
class FakeSerial : public QWidget
{
    Q_OBJECT
public:
    explicit FakeSerial(QWidget *parent = 0);

    QStringList listBaudRate(void);
    QStringList listDataBits(void);
    QStringList listParity(void);
    QStringList listStopBits(void);
    QStringList listFlowControl(void);

    QString baudRate(void);
    QString dataBits(void);
    QString parity(void);
    QString stopBits(void);
    QString flowControl(void);

    void setDeviceName(const QString &text);
    void setBaudRate(const QString &text);
    void setDataBits(const QString &text);
    void setParity(const QString &text);
    void setStopBits(const QString &text);
    void setFlowControl(const QString &text);

    bool isOpen(void);

    void enableEmitStatus(bool state);

    bool open(QIODevice::OpenMode);
    bool close(void);
    QByteArray readAll(void);
    void write(const char *data, int len);
    void write(const QByteArray &data);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void output(const QByteArray &data);

    void ctsChanged(bool);
    void dsrChanged(bool);
    void ringChanged(bool);
    void readyRead(void);
    void signalStatus(QString, QDateTime);

private slots:
    void log(const QString &data);

private:
    QStringList list_deviceName;
    QStringList list_baudRate;
    QStringList list_dataBits;
    QStringList list_parity;
    QStringList list_stopBits;
    QStringList list_flowControl;

    QString string_deviceName;
    QString string_baudRate;
    QString string_dataBits;
    QString string_parity;
    QString string_stopBits;
    QString string_flowControl;

    QString default_deviceName;
    QString default_baudRate;
    QString default_dataBits;
    QString default_parity;
    QString default_stopBits;
    QString default_flowControl;

    Emulator_V786_2 *emulator_v786_2;

    bool emitStatus;

    bool state;

    QByteArray input_data;
    QByteArray output_data;

    void init(void);
    void connect_log(void);
    void wait_msec(int timeout_msec);

};
//--------------------------------------------------------------------------------
#endif // FAKESERIAL_HPP
