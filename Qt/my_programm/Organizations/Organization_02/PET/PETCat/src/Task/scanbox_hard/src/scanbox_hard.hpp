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
#ifndef SCANBOX_HARD_HPP
#define SCANBOX_HARD_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class ScanBox_hard;
}
//--------------------------------------------------------------------------------
class QUdpSocket;
class QFile;
//--------------------------------------------------------------------------------
class ScanBox_hard : public MyWidget
{
    Q_OBJECT

public:
    explicit ScanBox_hard(pet_discovery_data pet_config,
                          QWidget *parent);
    ~ScanBox_hard();

    void get_answer(QString *answer);

private slots:
    void choose_file(void);

    void f_start_stop(void);
    void f_update(void);

    void f_timer_start(void);
    void f_timer_stop(void);

    void processPendingDatagrams(void);
    void get_packet(QHostAddress host, quint16 port, QByteArray packet);

private:
    Ui::ScanBox_hard *ui;

    QUdpSocket *scan_socket;
    QByteArray scan_datagram;

    QTimer *timer;
    QFile *file;

    quint32 cnt_second;

    pet_discovery_data p_pet_config;

    quint64 cnt_all_packet;
    quint64 cnt_good_packet;
    quint64 cnt_good_packet_last_sec;
    quint64 cnt_packet_bad_cmd;
    quint64 cnt_packet_bad_len;

    bool block_received_packet;

    void init(void);
    bool bind(void);
    void init_timer(void);
    void init_udp_socket(void);
    QString convert_int_to_string(quint64 value);
};
//--------------------------------------------------------------------------------
#endif
