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
#ifndef SIMPLE_TCP_SERVER_HPP
#define SIMPLE_TCP_SERVER_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include <QMap>
//--------------------------------------------------------------------------------
class SimpleRemoteConsole : public QObject
{
    Q_OBJECT
public:
    explicit SimpleRemoteConsole(quint16 port = 2222, QObject *parent = nullptr);
    ~SimpleRemoteConsole();

    bool startServer();
    void stopServer();

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();
    void onProcessReadyRead();

private:
    quint16 m_port;
    QTcpServer *m_tcpServer;

    // Храним связку: какой сокет клиента управляет каким процессом ОС
    QMap<QTcpSocket*, QProcess*> m_clients;
};
//--------------------------------------------------------------------------------
#endif