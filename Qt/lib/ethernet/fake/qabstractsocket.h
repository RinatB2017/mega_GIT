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
#ifndef QABSTRACTSOCKET_H
#define QABSTRACTSOCKET_H
//--------------------------------------------------------------------------------
#include <QIODevice>
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
class Q_NETWORK_EXPORT QAbstractSocket : public QIODevice
{
    Q_OBJECT
    Q_ENUMS(SocketType NetworkLayerProtocol SocketError SocketState SocketOption)

public:
    enum SocketType {
        TcpSocket,
        UdpSocket,
        UnknownSocketType = -1
    };
    enum NetworkLayerProtocol {
        IPv4Protocol,
        IPv6Protocol,
        UnknownNetworkLayerProtocol = -1
    };
    enum SocketError {
        ConnectionRefusedError,
        RemoteHostClosedError,
        HostNotFoundError,
        SocketAccessError,
        SocketResourceError,
        SocketTimeoutError,                     /* 5 */
        DatagramTooLargeError,
        NetworkError,
        AddressInUseError,
        SocketAddressNotAvailableError,
        UnsupportedSocketOperationError,        /* 10 */
        UnfinishedSocketOperationError,
        ProxyAuthenticationRequiredError,
        SslHandshakeFailedError,
        ProxyConnectionRefusedError,
        ProxyConnectionClosedError,             /* 15 */
        ProxyConnectionTimeoutError,
        ProxyNotFoundError,
        ProxyProtocolError,

        UnknownSocketError = -1
    };
    enum SocketState {
        UnconnectedState,
        HostLookupState,
        ConnectingState,
        ConnectedState,
        BoundState,
        ListeningState,
        ClosingState
#ifdef QT3_SUPPORT
        ,
        Idle = UnconnectedState,
        HostLookup = HostLookupState,
        Connecting = ConnectingState,
        Connected = ConnectedState,
        Closing = ClosingState,
        Connection = ConnectedState
#endif
    };
    enum SocketOption {
        LowDelayOption,         // TCP_NODELAY
        KeepAliveOption,        // SO_KEEPALIVE
        MulticastTtlOption,     // IP_MULTICAST_TTL
        MulticastLoopbackOption // IP_MULTICAST_LOOPBACK
    };

    QAbstractSocket();
    ~QAbstractSocket();
    SocketState state(void) const;
    QString errorString(void);

protected slots:

private:

};
//--------------------------------------------------------------------------------
#endif
