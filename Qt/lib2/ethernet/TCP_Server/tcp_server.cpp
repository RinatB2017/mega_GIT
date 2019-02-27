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
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "processor.hpp"
#include "tcp_server.hpp"
//--------------------------------------------------------------------------------
TCP_Server::TCP_Server(QWidget *parent) :
    MyWidget(parent),
    processor(0),
    tcpServer(0)
{
    processor = new Processor;
    connect(this, SIGNAL(output(QByteArray)), processor, SLOT(input(QByteArray)));
    connect(processor, SIGNAL(output(QByteArray)), this, SLOT(input(QByteArray)));
}
//--------------------------------------------------------------------------------
TCP_Server::~TCP_Server()
{
    delete tcpServer;
}
//--------------------------------------------------------------------------------
bool TCP_Server::createServerOnPort(const QHostAddress address, unsigned int port)
{
    emit info(QString("Создание сервера %1:%2")
              .arg(address.toString())
              .arg(port));

    clientConnection = 0;
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(address, port))
    {
        emit error(QString("Ошибка: %1").arg(tcpServer->errorString()));
        return false;
    }
    emit info("Сервер создан");

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    emit debug(QString("IP: %1").arg(ipAddress));
    emit debug(QString("Port: %1").arg(tcpServer->serverPort()));

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnect()));
    return true;
}
//--------------------------------------------------------------------------------
void TCP_Server::closeServer(void)
{
    clientConnection = 0;
    delete tcpServer;
}
//--------------------------------------------------------------------------------
void TCP_Server::newConnect(void)
{
    clientConnection = tcpServer->nextPendingConnection();
    emit info(QString("Клиент подключился: %1:%2")
              .arg(clientConnection->peerAddress().toString())
              .arg(clientConnection->peerPort()));
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
}
//--------------------------------------------------------------------------------
void TCP_Server::clientReadyRead(void)
{
    if(clientConnection->bytesAvailable())
    {
        QByteArray read_block;

        emit info("Получены данные");

        read_block = clientConnection->readAll();
        emit trace(read_block);
        emit debug(read_block.toHex().toUpper());

        emit output(read_block);
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
    clientConnection->deleteLater();
}
//--------------------------------------------------------------------------------
void TCP_Server::updateText(void)
{

}
//--------------------------------------------------------------------------------
