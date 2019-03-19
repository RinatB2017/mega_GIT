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
#include <QCoreApplication>
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

#if 0
    emit info(QString("size %1 bytes").arg(sizeof(PACKET)));
#else
    udpSocket = new QUdpSocket(this);

#if 1
    int cnt_repeat = 97;
#else
    int cnt_repeat = 1000000;
#endif

    int packet_err = 0;
    send_raw_data(cnt_repeat, &packet_err);
    //send_data(100,   cnt_repeat, &packet_err);
    //send_data(1000,  cnt_repeat, &packet_err);
    //send_data(10000, cnt_repeat, &packet_err);
#endif

    emit finished();
}
//--------------------------------------------------------------------------------
void Client::send_data(int size_packet,
                       int max_num_packet,
                       int *err_cnt)
{
    QByteArray datagram;
    int packet_cnt = 0;
    int packet_err = 0;

    datagram.clear();
    int packet_size = size_packet;
    for(int n=0; n<packet_size; n++)
    {
        datagram.append(char(0));
    }

    QTime timer;
    timer.start();
    while(packet_cnt < max_num_packet)
    {
        qint64 r = udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, PORT);
        //qint64 r = udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost, PORT);
        if(r < 0)
        {
            packet_err++;
        }
        packet_cnt++;
    }

    int te = timer.elapsed();
    if(te == 0) te = 1;
    int data_size = (packet_cnt - packet_err) * packet_size;
    emit info("-------------------");
    emit info(QString("elapsed %1 msec").arg(te));
    emit info(QString("packet_size %1").arg(packet_size));
    emit info(QString("max_num_packet %1").arg(max_num_packet));
    emit info("---");
    emit info(QString("send data %1").arg(data_size));
    emit info(QString("data %1 in 1 msec").arg(static_cast<qreal>(data_size) / static_cast<qreal>(te)));
    emit info(QString("speed %1 MBit/s").arg(static_cast<qreal>(data_size) / static_cast<qreal>(te) * 8000.0 / 1000000.0));
    emit info(QString("packet_cnt %1").arg(packet_cnt - packet_err));
    if(packet_err > 0)
    {
        emit error(QString("packet_err %1").arg(packet_err));
    }

    *err_cnt = packet_err;
}
//--------------------------------------------------------------------------------
void Client::send_raw_data(int max_num_packet,
                           int *err_cnt)
{
    QByteArray datagram;
    int packet_cnt = 0;
    int packet_err = 0;
    int packet_size = sizeof(PACKET);

    PACKET packet;
    packet.body.num_packet = 0;
    for(int n=0; n<MAX_EVENT_IN_PACKET; n++)
    {
        packet.body.event[n].TS = 0;
        packet.body.event[n].XP = 0;
        packet.body.event[n].XN = 0;
        packet.body.event[n].YP = 0;
        packet.body.event[n].YN = 0;
    }
    packet.body.crc16 = 0;

    datagram.clear();
    for(int n=0; n<packet_size; n++)
    {
        datagram.append(static_cast<char>(packet.buf[n]));
    }

    QTime timer;
    timer.start();
    while(packet_cnt < max_num_packet)
    {
        //qint64 r = udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, PORT);
        qint64 r = udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost, PORT);
        if(r < 0)
        {
            packet_err++;
        }
        packet_cnt++;
    }

    int te = timer.elapsed();
    if(te == 0) te = 1;
    int data_size = (packet_cnt - packet_err) * packet_size;
    emit info("-------------------");
    emit info(QString("elapsed %1 msec").arg(te));
    emit info(QString("packet_size %1").arg(packet_size));
    emit info(QString("max_num_packet %1").arg(max_num_packet));
    emit info("---");
    emit info(QString("send data %1").arg(data_size));
    emit info(QString("data %1 in 1 msec").arg(static_cast<qreal>(data_size) / static_cast<qreal>(te)));
    emit info(QString("speed %1 MBit/s").arg(static_cast<qreal>(data_size) / static_cast<qreal>(te) * 8000.0 / 1024.0 / 1024.0));
    emit info(QString("packet_cnt %1").arg(packet_cnt - packet_err));
    if(packet_err > 0)
    {
        emit error(QString("packet_err %1").arg(packet_err));
    }

    *err_cnt = packet_err;
}
//--------------------------------------------------------------------------------
