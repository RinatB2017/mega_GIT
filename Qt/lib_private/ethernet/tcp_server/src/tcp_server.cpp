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
#include "tcp_server.hpp"
//--------------------------------------------------------------------------------
TCP_Server::TCP_Server(QObject *parent) :
    QObject(parent)
{
    is_open = false;

    QTimer::singleShot(0, [this]{
        emit server_is_open(is_open);
    });
}
//--------------------------------------------------------------------------------
TCP_Server::~TCP_Server()
{
    if(tcpServer)
    {
        delete tcpServer;
    }
}
//--------------------------------------------------------------------------------
bool TCP_Server::createServerOnPort(const QHostAddress address, quint16 port)
{
    emit info(QString("Создание сервера на порту %1").arg(port));

    if(tcpServer)
    {
        tcpServer->close();
        delete tcpServer;
    }

    tcpServer = new QTcpServer(this);
    connect(tcpServer,  &QTcpServer::acceptError,   this,   &TCP_Server::print_error);

    if (!tcpServer->listen(address, port))
    {
        emit error(QString("Ошибка: %1").arg(tcpServer->errorString()));
        return false;
    }
    emit info("Сервер создан");
    emit info(QString("IP: %1").arg(tcpServer->serverAddress().toString()));
    emit info(QString("Port: %1").arg(tcpServer->serverPort()));

    is_open = true;
    emit server_is_open(is_open);

    connect(tcpServer,  &QTcpServer::newConnection, this,   &TCP_Server::newConnect);
    return true;
}
//--------------------------------------------------------------------------------
void TCP_Server::closeServer()
{
    if(tcpServer)
    {
        tcpServer->close();
    }

    is_open = false;
    emit server_is_open(is_open);
}
//--------------------------------------------------------------------------------
void TCP_Server::newConnect()
{
    while(tcpServer->hasPendingConnections())
    {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        if (!clientSocket) continue;

        emit info(QString("Клиент подключился: %1:%2")
                  .arg(clientSocket->peerAddress().toString())
                  .arg(clientSocket->peerPort()));

        // Подписываемся на события конкретного сокета
        connect(clientSocket, &QTcpSocket::readyRead,           this, &TCP_Server::clientReadyRead);
        connect(clientSocket, &QAbstractSocket::errorOccurred,  this, &TCP_Server::print_error);
        connect(clientSocket, &QTcpSocket::disconnected,        this, &TCP_Server::clientDisconnected);
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::clientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    emit info(QString("Клиент %1 отключился").arg(clientSocket->peerAddress().toString()));
    clientSocket->deleteLater();
}
//--------------------------------------------------------------------------------
void TCP_Server::clientReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    emit info(QString("Принято от %1:%2")
              .arg(clientSocket->peerAddress().toString())
              .arg(data.trimmed()));
}
//--------------------------------------------------------------------------------
void TCP_Server::print_error(QAbstractSocket::SocketError socketError)
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    if (socketError != QAbstractSocket::RemoteHostClosedError) {
        emit error(QString("Ошибка сокета для IP %1:%2")
                   .arg(clientSocket->peerAddress().toString())
                   .arg(clientSocket->errorString()));
    }
}
//--------------------------------------------------------------------------------
bool TCP_Server::is_opened()
{
    return is_open;
}
//--------------------------------------------------------------------------------
