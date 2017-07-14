/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QtNetwork>
#include <QObject>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "server.hpp"
//--------------------------------------------------------------------------------
Server::Server(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
Server::~Server()
{

}
//--------------------------------------------------------------------------------
void Server::process(void)
{
    emit info("Server started!");

    udpSocket = new QUdpSocket(this);
    bool ok = udpSocket->bind(PORT, QUdpSocket::ShareAddress);
    if(!ok)
    {
        emit error("error server bind");
        emit finished();
        return;
    }

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    QTime time;
    while(1)
    {
        //emit info("---");
        time.start();
        while(time.elapsed() < 1000)
        {

        }
    }
}
//--------------------------------------------------------------------------------
void Server::processPendingDatagrams(void)
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        emit info(QString("Received datagram: \"%1\"").arg(datagram.data()));
    }
}
//--------------------------------------------------------------------------------
