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
#ifndef PET_COMM_HPP
#define PET_COMM_HPP
//--------------------------------------------------------------------------------
#include <QQueue>
#include <QUdpSocket>
#include <QHostAddress>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_errno.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
typedef struct udp_data
{
    QHostAddress    remote_host;
    quint16         remote_port;
    QByteArray      remote_data;
} udp_data_t;
//--------------------------------------------------------------------------------
class PET_comm : public QObject
{
    Q_OBJECT

public:
    explicit PET_comm(void);
    ~PET_comm();

    PET_RESULT set_comm_param(QHostAddress host, quint16 port);
    PET_RESULT cmd(QByteArray question, QByteArray *answer);
    PET_RESULT send_raw_data(QByteArray question);

    bool bind(QHostAddress address, quint16 port);
    bool bind(QHostAddress address);

    void set_max_waiting_msec(uint16_t value);
    uint16_t get_max_waiting_msec(void);

    bool get_remote_data(udp_data *u_data);

    QString get_last_error(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

    void send_net_log(const QString &);
    void receive_net_log(const QString &);

    void send_pet_discovery_data(pet_discovery_data_t);

private slots:
    void readPendingDatagrams(void);

private:
    QHostAddress    remote_host;
    quint16         remote_port;

    uint8_t  i_addr;
    uint8_t  i_cmd;

    QQueue<udp_data> q_data;
    void append_remote_data(QHostAddress r_host,
                            quint16      r_port,
                            QByteArray   r_data);

    QUdpSocket *socket = 0;
    QString last_error;

    int max_waiting_msec;
};
//--------------------------------------------------------------------------------
#endif
