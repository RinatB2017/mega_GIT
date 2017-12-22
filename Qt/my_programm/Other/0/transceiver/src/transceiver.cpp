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
#include <QCoreApplication>
#include <QUdpSocket>
#include <QTime>
//--------------------------------------------------------------------------------
#include "transceiver.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
Transceiver::Transceiver(QObject *parent) :
    QObject(parent)
{
    udp_socket_cmd  = 0;

    init_cmd_socket();
}
//--------------------------------------------------------------------------------
Transceiver::~Transceiver()
{
    if(udp_socket_cmd)  udp_socket_cmd->deleteLater();
}
//--------------------------------------------------------------------------------
void Transceiver::processPendingDatagrams_cmd(void)
{
    while (udp_socket_cmd->hasPendingDatagrams())
    {
        datagram_cmd.resize(udp_socket_cmd->pendingDatagramSize());

        QHostAddress remote_host;
        quint16 remote_port = 0;
        qint64 bytes = udp_socket_cmd->readDatagram(datagram_cmd.data(), datagram_cmd.size(), &remote_host, &remote_port);
        emit debug(QString("Received %1 bytes").arg(bytes));
        if(bytes == datagram_cmd.size())
        {
            QHostAddress remote_host_ipv4 = QHostAddress(remote_host.toIPv4Address());
            emit debug(QString("Received CMD datagram: %1 bytes [%2:%3]")
                       .arg(datagram_cmd.size())
                       .arg(remote_host_ipv4.toString())
                       .arg(remote_port));
            if(remote_host != local_host)
            {
                emit get_packet(remote_host_ipv4, remote_port, datagram_cmd);
                return;
            }
        }
    }
}
//--------------------------------------------------------------------------------
void Transceiver::init_cmd_socket(void)
{
#ifdef UDP_PROXY
    udp_socket_cmd  = new QUdpSocket_proxy(this);
#else
    udp_socket_cmd  = new QUdpSocket(this);
#endif
    connect(udp_socket_cmd, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams_cmd()));
}
//--------------------------------------------------------------------------------
bool Transceiver::bind(QHostAddress host)
{
    bool ok = false;

    ok = udp_socket_cmd->bind(host);
    if(!ok)
    {
        last_error = QString("Transceiver::bind host %1 errorString: %2")
                .arg(host.toString())
                .arg(udp_socket_cmd->errorString());
        emit error(last_error);
        return false;
    }
    local_host = udp_socket_cmd->localAddress().toString();
    local_port = udp_socket_cmd->localPort();
    return true;
}
//--------------------------------------------------------------------------------
bool Transceiver::bind(quint16 port)
{
    bool ok = false;

    ok = udp_socket_cmd->bind(QHostAddress::AnyIPv4, port);
    if(!ok)
    {
        last_error = QString("Transceiver::bind port %1 errorString: %2")
                .arg(port)
                .arg(udp_socket_cmd->errorString());
        emit error(last_error);
        return false;
    }
    local_host = udp_socket_cmd->localAddress().toString();
    local_port = udp_socket_cmd->localPort();
    return true;
}
//--------------------------------------------------------------------------------
bool Transceiver::send_packet(QHostAddress host, quint16 port, QByteArray packet)
{
    datagram_cmd.clear();
    return send_raw_packet(host, port, packet);
}
//--------------------------------------------------------------------------------
bool Transceiver::send_raw_packet(QHostAddress host, quint16 port, QByteArray packet)
{
    //    emit debug(QString("send_raw_packet: host   [%1]").arg(host.toString()));
    //    emit debug(QString("send_raw_packet: port   [%1]").arg(port));
    //    emit debug(QString("send_raw_packet: packet [%1]").arg(packet.toHex().data()));

    qint64 bytes = udp_socket_cmd->writeDatagram(packet, host, port);
    if(bytes != packet.length())
    {
        last_error = QString("send_raw_packet: bytes(%1) != packet.length(%2)")
                .arg(bytes)
                .arg(packet.length());
        emit error(last_error);
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
QString Transceiver::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
