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
#ifndef QHOSTADDRESS_H
#define QHOSTADDRESS_H
//--------------------------------------------------------------------------------
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
struct sockaddr;

class Q_NETWORK_EXPORT QIPv6Address
{
public:
    inline quint8 &operator [](int index) { return c[index]; }
    inline quint8 operator [](int index) const { return c[index]; }
    quint8 c[16];
};

typedef QIPv6Address Q_IPV6ADDR;

//--------------------------------------------------------------------------------
class Q_NETWORK_EXPORT QHostAddress
{
public:
    enum SpecialAddress {
        Null,
        Broadcast,
        LocalHost,
        LocalHostIPv6,
        Any,
        AnyIPv6
    };

    QHostAddress();
    explicit QHostAddress(quint32 ip4Addr);
    explicit QHostAddress(quint8 *ip6Addr);
    explicit QHostAddress(const Q_IPV6ADDR &ip6Addr);
    explicit QHostAddress(const sockaddr *sockaddr);
    explicit QHostAddress(const QString &address);
    QHostAddress(const QHostAddress &copy);
    QHostAddress(SpecialAddress address);
    ~QHostAddress();

    QHostAddress &operator=(const QHostAddress &other);
    QHostAddress &operator=(const QString &address);

    void setAddress ( quint32 ip4Addr );
    void setAddress ( quint8 * ip6Addr );
    void setAddress ( const Q_IPV6ADDR & ip6Addr );
    void setAddress ( const sockaddr * sockaddr );
    bool setAddress ( const QString & address );

    quint32 toIPv4Address() const;

    QString toString(void) const;

    bool operator ==(const QHostAddress &address) const;
    bool operator ==(SpecialAddress address) const;
    inline bool operator !=(const QHostAddress &address) const
    { return !operator==(address); }
    inline bool operator !=(SpecialAddress address) const
    { return !operator==(address); }

protected slots:

private:
};

inline bool operator ==(QHostAddress::SpecialAddress address1, const QHostAddress &address2)
{ return address2 == address1; }
//--------------------------------------------------------------------------------
#endif
