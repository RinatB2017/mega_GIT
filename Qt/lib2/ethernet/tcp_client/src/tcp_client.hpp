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
#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QTcpSocket;
class QByteArray;
class QWidget;
//--------------------------------------------------------------------------------
class TCP_Client : public MyWidget
{
    Q_OBJECT

public:
    explicit TCP_Client(QWidget* parent = nullptr);
    void setAddress(const QHostAddress &);
    void setPort(unsigned int);

    void connect_to_host(QString address, quint16 port);
    void disconnect_from_host(void);
    qint64 write_data(QByteArray data);
    QTcpSocket::SocketState get_state(void);
    QByteArray readAll(void);
    QString get_errorString(void);

signals:
    void readyRead(void);
    void disconnected(void);
    void socket_error(QAbstractSocket::SocketError);
    void state_changed(QAbstractSocket::SocketState);
    void output(const QByteArray &);

public slots:
    QByteArray input(const QByteArray &data);

private slots:
    QByteArray send_data(const QByteArray &);

private:
    QPointer<QTcpSocket> tcpSocket;
    QString address;
    uint port = 0;
    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

    void readyData(void);
};
//--------------------------------------------------------------------------------
#endif
