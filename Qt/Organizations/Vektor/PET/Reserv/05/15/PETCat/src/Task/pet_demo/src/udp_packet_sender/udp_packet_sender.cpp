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
#include <QCoreApplication>
#include <QHostAddress>
#include <QUdpSocket>
#include <QtMath>
#include <QTime>
//--------------------------------------------------------------------------------
#include "udp_packet_sender.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
UDP_packet_sender::UDP_packet_sender(QUdpSocket *socket,
                                     QHostAddress host,
                                     quint16 port,
                                     QObject *parent) :
    QObject(parent),
    udp_host(host),
    udp_port(port),
    udp_socket(socket)
{
    flag_break   = false;
    flag_process = false;
}
//--------------------------------------------------------------------------------
UDP_packet_sender::~UDP_packet_sender()
{
    flag_break = true;
}
//--------------------------------------------------------------------------------
void UDP_packet_sender::s_start(void)
{
    emit debug("UDP_packet_sender::start");
    flag_process = true;
}
//--------------------------------------------------------------------------------
void UDP_packet_sender::s_stop(void)
{
    emit debug("UDP_packet_sender::stop");
    flag_process = false;
}
//--------------------------------------------------------------------------------
void UDP_packet_sender::process(void)
{
    emit debug("UDP_packet_sender started");

    pet_event event;
    QByteArray ba;

    quint64 ts = 0;
    int cnt_process = 0;
    while(!flag_break)
    {
        cnt_process++;
        if(cnt_process > 1000)
        {
            QCoreApplication::processEvents();
            cnt_process = 0;
        }
        if(flag_process)
        {
            double xx = 0x8000 + double(25000)*qCos(double(ts)*double(M_PI)/double(180.0));
            double yy = 0x8000 + double(25000)*qSin(double(ts)*double(M_PI)/double(180.0));

            xx += rand() % 5000;
            yy += rand() % 2000;

            event.src = 0;
            event.flags = 0;

            event.ts_fract = 0;
            event.ts = ts;

            event.xp = xx;
            event.xn = 0xFFFF - xx;
            event.yp = yy;
            event.yn = 0xFFFF - yy;

#ifdef USE_CRC16
            packet.body.crc16 = CRC::pet_crc16((unsigned char *)&packet.buf, sizeof(PACKET_DATA) - 2);
#endif

            pet_frame_t frame;
            frame.addr = 0;
            frame.cmd = PET_CMD_EVENT_STREAM;
            frame.len = sizeof(event);

            ba.clear();
            ba.append((char *)&frame, sizeof(frame));
            ba.append((char *)&event, sizeof(event));
            udp_socket->writeDatagram(ba, udp_host, udp_port);
            ts++;
        }
    }
    emit finished();
}
//--------------------------------------------------------------------------------
