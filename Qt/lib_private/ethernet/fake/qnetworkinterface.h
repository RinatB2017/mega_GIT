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
#ifndef QNETWORKINTERFASE_H
#define QNETWORKINTERFASE_H
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
class Q_NETWORK_EXPORT QNetworkInterface
{
public:
    enum InterfaceFlag {
        IsUp = 0x1,
        IsRunning = 0x2,
        CanBroadcast = 0x4,
        IsLoopBack = 0x8,
        IsPointToPoint = 0x10,
        CanMulticast = 0x20
    };
    Q_DECLARE_FLAGS(InterfaceFlags, InterfaceFlag)

    QNetworkInterface();
    QNetworkInterface(const QNetworkInterface &other);
    QNetworkInterface &operator=(const QNetworkInterface &other);
    ~QNetworkInterface();

    static QNetworkInterface interfaceFromName(const QString &name);
    static QNetworkInterface interfaceFromIndex(int index);
    static QList<QNetworkInterface> allInterfaces();
    static QList<QHostAddress> allAddresses();

private:

};
//--------------------------------------------------------------------------------
#endif
