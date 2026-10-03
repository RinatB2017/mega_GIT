/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
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
#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP
//--------------------------------------------------------------------------------
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QPointer>

#include <QHostAddress>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
class TCP_Client : public QObject
{
    Q_OBJECT

signals:
    void readyRead();
    void disconnected();
    void socket_error(QAbstractSocket::SocketError);
    void state_changed(QAbstractSocket::SocketState);
    void output(const QByteArray &);

public:
    explicit TCP_Client(QObject* parent = nullptr);
    virtual ~TCP_Client();

    void setAddress(const QHostAddress &);
    void setPort(unsigned int);

    void connect_to_host(QString address, quint16 port);
    void disconnect_from_host();
    qint64 write_data(QByteArray data);
    QTcpSocket::SocketState get_state();
    QByteArray readAll();
    QString get_errorString();

    QByteArray input(const QByteArray &data);

public slots:
    void send_info(QString text);
    void send_debug(QString text);
    void send_error(QString text);
    void send_trace(QString text);

private slots:
    QByteArray send_data(const QByteArray &);

private:
    QPointer<QTcpSocket> tcpSocket;
    QHostAddress address;
    uint port = 0;

    void init();
    void readyData();
};
//--------------------------------------------------------------------------------
#endif
