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
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qudpsocket.h"
//--------------------------------------------------------------------------------
QUdpSocket::QUdpSocket(QWidget *parent) :
    QObject(parent)
{
    qDebug() << "new QUdpSocket";
}
//--------------------------------------------------------------------------------
bool QUdpSocket::bind(const QHostAddress &address, quint16 port)
{
    qDebug() << QString("bind address %1 port %2")
                .arg(address.toString())
                .arg(port);
    return true;
}
//--------------------------------------------------------------------------------
bool QUdpSocket::bind(quint16 port)
{
    qDebug() << QString("bind port %1")
                .arg(port);
    return true;
}
//--------------------------------------------------------------------------------
bool QUdpSocket::bind(const QHostAddress &address, quint16 port, BindMode mode)
{
    qDebug() << QString("bind address %1 port %2 mode %3")
                .arg(address.toString())
                .arg(port)
                .arg(mode_to_string(mode));
    return true;
}
//--------------------------------------------------------------------------------
bool QUdpSocket::bind(quint16 port, BindMode mode)
{
    qDebug() << QString("bind port %1 mode %2")
                .arg(port)
                .arg(mode_to_string(mode));
    return true;
}
//--------------------------------------------------------------------------------
qint64 QUdpSocket::readDatagram(char *data, qint64 maxlen, QHostAddress *host, quint16 *port)
{
    memcpy((char *)data, (char *)"hello", 5);
    qDebug() << QString("readDatagram [hello] maxlen %1 host %2 port %3")
                .arg(maxlen)
                .arg(host->toString())
                .arg(*port);
    return 5;
}
//--------------------------------------------------------------------------------
qint64 QUdpSocket::pendingDatagramSize(void)
{
    return 5;
}
//--------------------------------------------------------------------------------
qint64 QUdpSocket::writeDatagram(const char *data, qint64 len, const QHostAddress &host, quint16 port)
{
    qDebug() << QString("writeDatagram(1) [%1] len %2 host %3 port %4")
                .arg(data)
                .arg(len)
                .arg(host.toString())
                .arg(port);
    return len;
}
//--------------------------------------------------------------------------------
qint64 QUdpSocket::writeDatagram(const QByteArray &datagram, const QHostAddress &host, quint16 port)
{
    qDebug() << QString("writeDatagram(2) [%1] host %2 port %3")
                .arg(datagram.data())
                .arg(host.toString())
                .arg(port);
    return datagram.length();
}
//--------------------------------------------------------------------------------
QString QUdpSocket::mode_to_string(BindMode mode)
{
    QString temp;

    switch(mode)
    {
    case DefaultForPlatform:
        temp = "DefaultForPlatform";
        break;
    case ShareAddress:
        temp = "ShareAddress";
        break;
    case DontShareAddress:
        temp = "DontShareAddress";
        break;
    case ReuseAddressHint:
        temp = "ReuseAddressHint";
        break;
    default:
        temp = "unknown mode";
        break;
    }

    return temp;
}
//--------------------------------------------------------------------------------
QString QUdpSocket::errorString(void)
{
    return "no error";
}
//--------------------------------------------------------------------------------
bool QUdpSocket::hasPendingDatagrams(void)
{
    return true;
}
//--------------------------------------------------------------------------------
