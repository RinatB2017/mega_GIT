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
#ifndef QTCPSOCKET_H
#define QTCPSOCKET_H
//--------------------------------------------------------------------------------
#include <QAbstractSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QVector>
#include <QObject>
//--------------------------------------------------------------------------------
enum OpenModeFlag {
    NotOpen = 0x0000,
    ReadOnly = 0x0001,
    WriteOnly = 0x0002,
    ReadWrite = ReadOnly | WriteOnly,
    Append = 0x0004,
    Truncate = 0x0008,
    Text = 0x0010,
    Unbuffered = 0x0020
};
Q_DECLARE_FLAGS(OpenMode, OpenModeFlag)
//--------------------------------------------------------------------------------
struct DATA
{
    QByteArray in;
    QByteArray out;
};
//--------------------------------------------------------------------------------
class Q_NETWORK_EXPORT QTcpSocket : public QObject
{
    Q_OBJECT
public:
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

    explicit QTcpSocket(QObject *parent = nullptr);
    virtual ~QTcpSocket();
    void connectToHost(const QHostAddress &address, quint16 port, OpenMode mode = ReadWrite);
    void connectToHost(const QString & hostName, quint16 port, OpenMode openMode = ReadWrite);
    void disconnectFromHost(void);
    bool waitForConnected ( int msecs = 30000 );
    bool waitForBytesWritten ( int msecs = 30000 );
    bool waitForReadyRead ( int msecs = 30000 );
    qint64 write ( const QByteArray &data );
    QByteArray readAll(void);
    void abort(void);
    bool waitForDisconnected ( int msecs = 30000 );
    QAbstractSocket::SocketState state(void) const;
    QString errorString(void);

signals:
    void connected(void);
    void disconnected(void);
    void error(QAbstractSocket::SocketError);
    void stateChanged(QAbstractSocket::SocketState);
    void readyRead(void);

protected slots:
    void connectToHostImplementation(QString &hostName, quint16 port, OpenMode mode);

private:
    QByteArray input_data;
    QByteArray output_data;
    QByteArray processing(const QByteArray &data);

    QVector <DATA> process;
};
//--------------------------------------------------------------------------------
#endif
