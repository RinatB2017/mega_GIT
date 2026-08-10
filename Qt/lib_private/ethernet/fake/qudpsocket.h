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
#ifndef QUDPSOCKET_H
#define QUDPSOCKET_H
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QByteArray>
#include <QObject>
//--------------------------------------------------------------------------------
enum BindFlag {
    DefaultForPlatform = 0x0,
    ShareAddress = 0x1,
    DontShareAddress = 0x2,
    ReuseAddressHint = 0x4
};
Q_DECLARE_FLAGS(BindMode, BindFlag)
//--------------------------------------------------------------------------------
class QUdpSocket : public QObject
{
    Q_OBJECT
public:
    explicit QUdpSocket(QWidget *parent = nullptr);
    bool bind(const QHostAddress &address, quint16 port);
    bool bind(quint16 port = 0);
    bool bind(const QHostAddress &address, quint16 port, BindMode mode);
    bool bind(quint16 port, BindMode mode);
    qint64 readDatagram(char *data, qint64 maxlen, QHostAddress *host = nullptr, quint16 *port = nullptr);
    qint64 writeDatagram(const char *data, qint64 len, const QHostAddress &host, quint16 port);
    qint64 writeDatagram(const QByteArray &datagram, const QHostAddress &host, quint16 port);
    QString errorString(void);
    bool hasPendingDatagrams(void);
    qint64 pendingDatagramSize(void);

signals:
    void readyRead(void);
    
public slots:

protected:

private:
    QString mode_to_string(BindMode mode);
};
//--------------------------------------------------------------------------------
#endif
