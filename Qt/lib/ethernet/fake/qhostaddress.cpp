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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qhostaddress.h"
//--------------------------------------------------------------------------------
QHostAddress::QHostAddress()
{
    qDebug() << "QHostAddress()";
}
//--------------------------------------------------------------------------------
QHostAddress::~QHostAddress()
{
    qDebug() << "~QHostAddress()";
}
//--------------------------------------------------------------------------------
QHostAddress::QHostAddress(const QString &address)
{
    qDebug() << address;
}
//--------------------------------------------------------------------------------
QHostAddress::QHostAddress(const QHostAddress &copy)
{
    Q_UNUSED(copy)
}
//--------------------------------------------------------------------------------
QHostAddress::QHostAddress(SpecialAddress address)
{
    Q_UNUSED(address)
}
//--------------------------------------------------------------------------------
void QHostAddress::setAddress( quint32 ip4Addr )
{
    qDebug() << QString("setAddress(%1)").arg(ip4Addr);
}
//--------------------------------------------------------------------------------
void QHostAddress::setAddress( quint8 * ip6Addr )
{
    qDebug() << QString("setAddress(%1)").arg(*ip6Addr);
}
//--------------------------------------------------------------------------------
void QHostAddress::setAddress( const Q_IPV6ADDR & ip6Addr )
{
    qDebug() << QString("setAddress(%1)").arg(ip6Addr.c[0]);
}
//--------------------------------------------------------------------------------
bool QHostAddress::setAddress( const QString & address )
{
    qDebug() << QString("setAddress(%1)").arg(address);
    return true;
}
//--------------------------------------------------------------------------------
void QHostAddress::setAddress( const sockaddr * sockaddr )
{
    Q_UNUSED(sockaddr);
}
//--------------------------------------------------------------------------------
QString QHostAddress::toString(void) const
{
    return QString("127.0.0.1");
}
//--------------------------------------------------------------------------------
quint32 QHostAddress::toIPv4Address(void) const
{
    return 0;
}
//--------------------------------------------------------------------------------
QHostAddress &QHostAddress::operator=(const QHostAddress &address)
{
    Q_UNUSED(address)
    return *this;
}
//--------------------------------------------------------------------------------
QHostAddress &QHostAddress::operator=(const QString &address)
{
    Q_UNUSED(address)
    return *this;
}
//--------------------------------------------------------------------------------
