/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef PCA10000_HPP
#define PCA10000_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#include <QSerialPort>
//--------------------------------------------------------------------------------
class PCA10000 : public QObject
{
    Q_OBJECT
public:
    explicit PCA10000(QObject *parent = 0);

    bool find_device(void);
    void print_info(const QSerialPortInfo &port);
    void test(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifdef FAKE
    PCA10000_test serial;
#else
    QSerialPort serial;
#endif

    void init(void);

    QByteArray sniffer_data;

    void encodeToSLIP(void);
    void decodeFromSLIP(void);
    void handlePacketHistory(void);
    void sendPacket(void);
    void sendScan(void);
    void sendFollow(void);
    void sendPingReq(void);
    void sendTK(void);
    void sendSwitchBaudRate(void);
    void switchBaudRate(void);
    void sendHopSequence(void);
    void sendGoIdle(void);
    void findSerialPort(void);
    void readStaticHeader(void);
    void writeStaticHeader(void);
    void readDynamicHeader(void);
    void writeDynamicHeader(void);
    void readPayload(void);
    void readFlags(void);
    void getList(void);
    void asString(void);
    void validatePacketList(void);
};
//--------------------------------------------------------------------------------
#endif
