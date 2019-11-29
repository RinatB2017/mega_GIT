/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QTime>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "client.hpp"
//--------------------------------------------------------------------------------
Client::Client(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
Client::~Client()
{

}
//--------------------------------------------------------------------------------
void Client::process(void)
{
    emit info("Client started!");

    udpSocket = new QUdpSocket(this);
    int messageNo = 0;

    QElapsedTimer time;
    int cnt = 0;
    while(cnt < 10000)
    {
        //emit info(tr("Now broadcasting datagram %1").arg(messageNo));

        QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
        qint64 r = udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, PORT);
#if 1
        if(r != datagram.size())
        {
            emit error(QString("cnt = %1").arg(cnt));
            emit error(udpSocket->errorString());
            emit error(QString("%1 != %2").arg(r).arg(datagram.size()));
        }
#endif
        messageNo++;

#if 1
        time.start();
        while(time.elapsed() < 1000)
        {

        }
#endif
        cnt++;
    }
    emit finished();
}
//--------------------------------------------------------------------------------
