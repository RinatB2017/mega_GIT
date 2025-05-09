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
#include "tcp_server.hpp"
//--------------------------------------------------------------------------------
TCP_Server::TCP_Server(QWidget *parent) :
    MyWidget(parent)
{
    is_open = false;

    QTimer::singleShot(0, [this]{
        emit server_is_open(is_open);
    });

    setVisible(false);
}
//--------------------------------------------------------------------------------
TCP_Server::~TCP_Server()
{
    delete tcpServer;
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
void TCP_Server::closeServer(void)
{
    if(tcpServer)
    {
        tcpServer->close();
    }
    if(clientConnection)
    {
        delete clientConnection;
        clientConnection = nullptr;
    }

    is_open = false;
    emit server_is_open(is_open);
}
//--------------------------------------------------------------------------------
void TCP_Server::newConnect(void)
{
    clientConnection = tcpServer->nextPendingConnection();
    emit info(QString("Клиент подключился: %1:%2")
              .arg(clientConnection->peerAddress().toString())
              .arg(clientConnection->peerPort()));
    connect(clientConnection,   &QTcpSocket::disconnected,  this,   &TCP_Server::clientDisconnected);
    connect(clientConnection,   &QTcpSocket::readyRead,     this,   &TCP_Server::clientReadyRead);
}
//--------------------------------------------------------------------------------
void TCP_Server::clientReadyRead(void)
{
    if(clientConnection->bytesAvailable())
    {
        QByteArray read_block;

        emit info("Получены данные");

        read_block = clientConnection->readAll();
        //emit trace(read_block);
        emit debug(read_block.toHex().toUpper());
        emit output(read_block);
        // возвращаем ответ
        //clientConnection->write(read_block);
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::input(const QByteArray &data)
{
    clientConnection->write(data);
}
//--------------------------------------------------------------------------------
void TCP_Server::clientDisconnected(void)
{
    emit info("Клиент отключился");
    delete clientConnection;
}
//--------------------------------------------------------------------------------
bool TCP_Server::is_opened(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
void TCP_Server::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool TCP_Server::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void TCP_Server::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void TCP_Server::save_setting(void)
{

}
//--------------------------------------------------------------------------------
