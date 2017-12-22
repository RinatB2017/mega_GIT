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
#ifndef TRANSCIEVER_HPP
#define TRANSCIEVER_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QUdpSocket>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
class Transceiver : public QObject
{
    Q_OBJECT
public:
    explicit Transceiver(QObject *parent = 0);
    ~Transceiver();

    bool bind(QHostAddress host);
    bool bind(quint16 port);
    QString get_last_error(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

    void get_packet(QHostAddress host, quint16 port, QByteArray packet);

public slots:
    bool send_packet(QHostAddress host, quint16 port, QByteArray packet);

private slots:
    void processPendingDatagrams_cmd(void);

private:
#ifdef UDP_PROXY
    QUdpSocket_proxy *udp_socket_cmd;
#else
    QUdpSocket *udp_socket_cmd;
#endif
    //---
    QByteArray datagram_cmd;
    QHostAddress local_host;
    quint16 local_port;

    QString last_error;

    bool send_raw_packet(QHostAddress host,
                         quint16 port,
                         QByteArray packet);
    void init_cmd_socket(void);
};
//--------------------------------------------------------------------------------
#endif
