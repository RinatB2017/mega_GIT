/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef UDP_PACKET_SENDER_HPP
#define UDP_PACKET_SENDER_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QObject>
#include <QQueue>
//--------------------------------------------------------------------------------
class QUdpSocket;
//--------------------------------------------------------------------------------
class UDP_packet_sender : public QObject
{
    Q_OBJECT
public:
    explicit UDP_packet_sender(QUdpSocket *socket,
                               QHostAddress host,
                               quint16 port,
                               QObject *parent = 0);
    ~UDP_packet_sender();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void finished(void);

public slots:
    void process(void);

    void s_start(void);
    void s_stop(void);

private:
    bool flag_break;
    bool flag_process;

    QHostAddress udp_host;
    quint16 udp_port;
    QUdpSocket *udp_socket = 0;
};
//--------------------------------------------------------------------------------
#endif
