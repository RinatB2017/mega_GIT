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
#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP
//--------------------------------------------------------------------------------
#include <QNetworkInterface>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Processor;
class QTcpServer;
class QTcpSocket;
//--------------------------------------------------------------------------------
class TCP_Server : public MyWidget
{
    Q_OBJECT

public:
    explicit TCP_Server(QWidget *parent = nullptr);
    virtual ~TCP_Server();

    bool is_opened(void);

signals:
    void output(const QByteArray &);
    void server_is_open(bool);
    void newConnection(void);

public slots:    
    bool createServerOnPort(const QHostAddress address, quint16 port);
    void closeServer(void);
    void input(const QByteArray &data);

private slots:
    void newConnect(void);
    void clientReadyRead(void);
    void clientDisconnected(void);

private:
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *clientConnection = nullptr;

    bool is_open = false;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
