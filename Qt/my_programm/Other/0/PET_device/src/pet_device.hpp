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
#ifndef PET_DEVICE_HPP
#define PET_DEVICE_HPP
//--------------------------------------------------------------------------------
#include <QToolBar>
#include <QObject>
#include <QWidget>
#include <QString>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
class Transceiver;
//--------------------------------------------------------------------------------
class PET_device : public QWidget
{
      Q_OBJECT

public:
    explicit PET_device(QWidget *parent = 0);

    static QString get_PET_RESULT_string(int value);
    static QString get_PET_VID_string(int value);
    static QString get_PET_PID_string(int value);
    static QString get_device_type_long_str(int type);
    static QString get_device_type_short_str(int type);

    bool bind(quint16 port);
    void set_param(PET_device_remote_t param);

    bool check_packet(uint8_t cmd, QByteArray packet);
    bool send_packet(QByteArray input_packet, QByteArray *output_packet);

    QByteArray get_received_packet(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private slots:
    void log(const QString text);
    void get_packet(QHostAddress host, quint16 port, QByteArray packet);

private:
    Transceiver *transceiver;
    PET_device_remote_t p_pet_config;

    uint16_t channel;
    uint8_t addr;

    bool is_ok;
    QByteArray receied_packet;

    void init_client(void);
    bool wait_answer(void);
};
//--------------------------------------------------------------------------------
#endif
