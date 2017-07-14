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
#ifndef PACKET_WORKER_HPP
#define PACKET_WORKER_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QObject>
//--------------------------------------------------------------------------------
class Packet_worker : public QObject
{
    Q_OBJECT
public:
    explicit Packet_worker(QObject *parent = 0);

    int get_error(void);
    QString get_error_text(void);

    bool set_address(int address);
    bool set_param_channel(int channel);
    bool set_param_id(int id);
    bool set_param_value(int value);

    QByteArray get_input_buffer(void);
    QByteArray get_output_buffer(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

    void get_packet(uint8_t id_packet);
    void get_error_packet(int error_code);

public slots:
    void get_data(QByteArray data);

private:
    QByteArray input_buffer;
    QByteArray output_buffer;

    int     last_error;
    QString last_error_text;

    int address;
    int param_channel;
    int param_id;
    int param_value;

    bool check_main_header(void);
};
//--------------------------------------------------------------------------------
#endif
