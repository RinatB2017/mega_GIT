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
#include <QUdpSocket>
#include <QTime>
//--------------------------------------------------------------------------------
#include "PET_comm.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet_errno.h"
#include "pet.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
PET_comm::PET_comm(void)
{
    remote_host = QHostAddress::LocalHost;
    remote_port = PETCat_options::get_port_cmd();

    max_waiting_msec = 1000;

    last_error.clear();

    socket = new QUdpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}
//--------------------------------------------------------------------------------
PET_comm::~PET_comm()
{
    if(socket)
    {
        socket->deleteLater();
    }
}
//--------------------------------------------------------------------------------
bool PET_comm::bind(QHostAddress address, quint16 port)
{
    bool ok = socket->bind(address, port);
    if(ok == false)
    {
        last_error = QString("PET_comm not binding: %1")
                .arg(socket->errorString());
        return false;
    }
    else
    {
        emit debug(QString("BIND addr %1 port %2")
                     .arg(socket->localAddress().toString())
                     .arg(socket->localPort()));
    }
    return true;
}
//--------------------------------------------------------------------------------
bool PET_comm::bind(QHostAddress address)
{
    bool ok = socket->bind(address);
    if(ok == false)
    {
        last_error = QString("PET_comm not binding: %1")
                .arg(socket->errorString());
        emit debug(last_error);
        return false;
    }
    else
    {
        //emit debug(QString("BIND2 addr %1 port %2")
        //             .arg(socket->localAddress().toString())
        //             .arg(socket->localPort()));
        emit debug(QString("BIND port %1").arg(socket->localPort()));
    }
    return true;
}
//--------------------------------------------------------------------------------
bool PET_comm::get_remote_data(udp_data *u_data)
{
    if(!q_data.isEmpty())
    {
        udp_data temp = q_data.dequeue();
        (*u_data).remote_host = temp.remote_host;
        (*u_data).remote_port = temp.remote_port;

        (*u_data).remote_data.clear();
        (*u_data).remote_data.append(temp.remote_data);
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
QString PET_comm::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_comm::set_comm_param(QHostAddress host, quint16 port)
{
    if(host.isNull())
    {
        return PET_ERR_BAD_ADDR;
    }
    if(port<MIN_UDP_PORT)
    {
        return PET_ERR_BAD_PORT;
    }
    if(port>=MAX_UDP_PORT)
    {
        return PET_ERR_BAD_PORT;
    }

    remote_host = host;
    remote_port = port;
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_comm::cmd(QByteArray question, QByteArray *answer)
{
    Q_CHECK_PTR(answer);
    if(question.isEmpty())
    {
        last_error = "PET_comm::cmd question.isEmpty()";
        return PET_ERR_BAD_FRAME;
    }

    emit send_net_log(QString("%1:%2 [%3]")
                      .arg(remote_host.toString())
                      .arg(remote_port)
                      .arg(question.toHex().data()));
    //---
    pet_frame *frame = (pet_frame *)question.data();
    i_addr = frame->addr;
    i_cmd = frame->cmd;
    //---
    qint64 bytes = socket->writeDatagram(question,
                                         remote_host,
                                         remote_port);
    if(bytes != question.length())
    {
        last_error = socket->errorString();
        return PET_ERR_FAULT;
    }
    QTime time;
    time.start();
    while(time.elapsed() < max_waiting_msec)
    {
        QCoreApplication::processEvents();
        if(!q_data.isEmpty())
        {
            udp_data t_data = q_data.dequeue();
            pet_frame *frame = (pet_frame *)t_data.remote_data.data();
            bool err = frame->cmd & PET_FRAME_CMD_FAULT_MSK;
            uint8_t cmd = frame->cmd & ~PET_FRAME_CMD_FAULT_MSK;
            if((frame->addr == i_addr) && (cmd == i_cmd))
            {
                (*answer).clear();
                (*answer).append(t_data.remote_data);
                if(err)
                {
                    pet_frame_error_t *err_frame = (pet_frame_error_t *)t_data.remote_data.data();
                    return (PET_RESULT)err_frame->result_cmd;
                }
                else
                {
                    return PET_SUCCESS;
                }
            }
        }
    }
    return PET_ERR_TIMEOUT;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_comm::send_raw_data(QByteArray question)
{
    if(question.isEmpty())
    {
        last_error = "PET_comm::send_raw_data question.isEmpty()";
        return PET_ERR_BAD_FRAME;
    }

    pet_frame *frame = (pet_frame *)question.data();
    i_addr = frame->addr;
    i_cmd = frame->cmd;

#if 1
    emit debug(QString("writeDatagram %1:%2 [%3]")
               .arg(remote_host.toString())
               .arg(remote_port)
               .arg(question.toHex().data()));
#endif
    qint64 bytes = socket->writeDatagram(question, remote_host, remote_port);
    if(bytes != question.length())
    {
        last_error = socket->errorString();
        emit debug(QString("last_error %1").arg(last_error));
        return PET_ERR_FAULT;
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_comm::readPendingDatagrams(void)
{
    QHostAddress    temp_host;
    quint16         temp_port;
    QByteArray      temp_data;

    temp_data.clear();
    while (socket->hasPendingDatagrams())
    {
        temp_data.resize(socket->pendingDatagramSize());
        qint64 bytes = socket->readDatagram(temp_data.data(), temp_data.size(), &temp_host, &temp_port);
        if(bytes != -1)
        {
            append_remote_data(temp_host, temp_port, temp_data);
        }
        //else
        //{
        //    emit debug(QString("PET_comm::readPendingDatagrams Ошибка: %1")
        //               .arg(socket->errorString()));
        //}
    }
}
//--------------------------------------------------------------------------------
void PET_comm::append_remote_data(QHostAddress r_host,
                                  quint16 r_port,
                                  QByteArray r_data)
{
    emit send_net_log(QString("%1:%2 [%3]")
                      .arg(r_host.toString())
                      .arg(r_port)
                      .arg(r_data.toHex().data()));

    if(r_data.length() < (int)(sizeof(pet_frame_t)))
    {
        return;
    }
    pet_frame *frame = (pet_frame *)r_data.data();
    if(r_data.length() != ((int)(sizeof(pet_frame_t) + frame->len)))
    {
        qDebug() << "r_data.length()"
                 << r_data.length()
                 << "sizeof(pet_frame_t)"
                 << sizeof(pet_frame_t)
                 << "frame->len"
                 << frame->len;
        return;
    }

    udp_data u_data;
    u_data.remote_host = r_host;
    u_data.remote_port = r_port;
    u_data.remote_data.clear();
    u_data.remote_data.append(r_data);
    q_data.enqueue(u_data);

    if(frame->cmd == PET_CMD_DISCOVERY)
    {
        pet_discovery_info *p_info = (pet_discovery_info *)frame->data;

        pet_discovery_data_t p_data;
        p_data.addr = frame->addr;
        p_data.host = r_host;
        p_data.port = r_port;
        p_data.vid = p_info->vid;
        p_data.pid = p_info->pid;
        memcpy((char *)&p_data.uid, p_info->uid, sizeof(p_info->uid));
        emit send_pet_discovery_data(p_data);
    }
}
//--------------------------------------------------------------------------------
void PET_comm::set_max_waiting_msec(uint16_t value)
{
    if(value == 0) return;
    max_waiting_msec = value;
}
//--------------------------------------------------------------------------------
uint16_t PET_comm::get_max_waiting_msec(void)
{
    return max_waiting_msec;
}
//--------------------------------------------------------------------------------
