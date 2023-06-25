/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include "tcp_local_server.hpp"
//--------------------------------------------------------------------------------
TCP_Server::TCP_Server(QWidget *parent) :
    MyWidget(parent)
{
    setVisible(false);
}
//--------------------------------------------------------------------------------
TCP_Server::~TCP_Server()
{
    if(tcpServer) delete tcpServer;
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

    connect(tcpServer,  &QTcpServer::newConnection,
            this,       &TCP_Server::newConnect);
    opened = true;
    emit port_is_active(opened);
    return true;
}
//--------------------------------------------------------------------------------
void TCP_Server::closeServer(void)
{
    if(tcpServer)
    {
        emit info("Сервер закрыт");
        tcpServer->close();

        opened = false;
        emit port_is_active(opened);
    }
    if(clientConnection)
    {
        delete clientConnection;
        clientConnection = nullptr;
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::newConnect(void)
{
    clientConnection = tcpServer->nextPendingConnection();
    Q_ASSERT(clientConnection);
    if(clientConnection)
    {
        emit info(QString("Клиент подключился: %1:%2")
                  .arg(clientConnection->peerAddress().toString())
                  .arg(clientConnection->peerPort()));
        connect(clientConnection,   &QTcpSocket::disconnected,
                this,               &TCP_Server::clientDisconnected);
        connect(clientConnection,   &QTcpSocket::readyRead,
                this,               &TCP_Server::clientReadyRead);
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::clientReadyRead(void)
{
    Q_ASSERT(clientConnection);
    if(clientConnection->bytesAvailable())
    {
        QByteArray read_block;

        read_block = clientConnection->readAll();
        bool is_string = is_letter_or_number(read_block);
        if(is_string)
        {
            QByteArray temp = read_block;
            emit info(QString("TCP_Server: получены данные: [%1] %2 bytes")
                      .arg(temp.replace("\r", "").replace("\n", "").data())
                      .arg(temp.length()));
        }
        else
        {
            emit info(QString("TCP_Server: получены данные: [%1] %2 bytes")
                      .arg(read_block.toHex().toUpper().data())
                      .arg(read_block.length()));
        }

        emit output(read_block);
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::input(QByteArray data)
{
    bool is_string = is_letter_or_number(data);
    if(is_string)
    {
        QByteArray temp = data;
        emit info(QString("TCP_Server::input: [%1")
                  .arg(temp.replace("\r", "").replace("\n", "").toUpper().data()));
        emit info(QString("clientConnection->write: [%1]")
                  .arg(temp.replace("\r", "").replace("\n", "").toUpper().data()));
    }
    else
    {
        emit info(QString("TCP_Server::input: [%1")
                  .arg(data.toHex().toUpper().data()));
        emit info(QString("clientConnection->write: [%1]")
                  .arg(data.toHex().toUpper().data()));
    }
    Q_ASSERT(clientConnection);
    if(clientConnection)
    {
        clientConnection->write(data);
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::clientDisconnected(void)
{
    emit info("Клиент отключился");
}
//--------------------------------------------------------------------------------
bool TCP_Server::is_open(void)
{
    return opened;
}
//--------------------------------------------------------------------------------
void TCP_Server::tcp_open(void)
{
    createServerOnPort(address, port);
}
//--------------------------------------------------------------------------------
void TCP_Server::tcp_close(void)
{
    closeServer();
}
//--------------------------------------------------------------------------------
void TCP_Server::set_address(const QString new_address)
{
    address = QHostAddress(new_address);
}
//--------------------------------------------------------------------------------
void TCP_Server::set_port(int new_port)
{
    port = new_port;
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
