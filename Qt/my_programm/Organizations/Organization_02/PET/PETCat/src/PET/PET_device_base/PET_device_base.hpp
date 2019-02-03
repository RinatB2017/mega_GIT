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
#ifndef PET_DEVICE_BASE_HPP
#define PET_DEVICE_BASE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_comm.hpp"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
class PET_device_base : public MyWidget
{
    Q_OBJECT

public:
    explicit PET_device_base(QWidget *parent);
    ~PET_device_base();

    static PET_RESULT discovery(QWidget *mw,
                                PET_comm *comm,
                                QHostAddress host,
                                quint16 port,
                                uint8_t pet_addr,
                                uint16_t max_timeout_msec,
                                QList<pet_discovery_data_t> *l_discovery);

    static QString get_PET_RESULT_string(int value);
    static QString get_PET_VID_string(int value);
    static QString get_PET_PID_string(int value);
    static QString get_PET_UID_string(uint8_t uid[]);
    static QString get_device_type_long_str(int type);
    static QString get_device_type_short_str(int type);

    static QString get_ip(uint8_t ip[]);
    static QString get_netmask(uint8_t netmask[]);
    static QString get_gateway(uint8_t gateway[]);
    static QString get_mac(uint8_t mac[]);

    static QString convert_timestamp_to_datatime_string(int32_t timestamp);
    static QString convert_version_to_string(uint16_t version);

    PET_RESULT reset(void);
    PET_RESULT shutdown(void);
    PET_RESULT config_save(void);
    PET_RESULT config_default(void);
    PET_RESULT update(void *i_data, int *i_data_len,
                      void *o_data, int *o_data_len);
    PET_RESULT run(void);
    PET_RESULT stop(void);

    QString get_last_error(void);

    PET_RESULT set_comm_param(QHostAddress host, quint16 port);
    bool bind(QHostAddress address, quint16 port);
    bool bind(QHostAddress address);

    void    set_pet_addr(uint8_t value);
    uint8_t get_pet_addr(void);

    void set_max_waiting_msec(uint16_t value);
    uint16_t get_max_waiting_msec(void);

    PET_comm *comm;

private:
    PET_RESULT send_packet(QByteArray question, QByteArray *answer);
    QString last_error;
    uint8_t addr;

    PET_RESULT check_packet(QByteArray question, uint8_t cmd);

protected:
    virtual int get_PID() = 0;

    PET_RESULT _get_test(void *i_data, int *i_data_len,
                         void *o_data, int *o_data_len);

    PET_RESULT _set_ctrl(void *i_data, int *i_data_len,
                         void *o_data, int *o_data_len);
    PET_RESULT _get_ctrl(void *o_data, int *o_data_len);

    PET_RESULT _set_ch_ctrl(void *i_data, int *i_data_len,
                            void *o_data, int *o_data_len);
    PET_RESULT _get_ch_ctrl(void *i_data, int *i_data_len,
                            void *o_data, int *o_data_len);

    PET_RESULT _get_ch_state(void *i_data, int *i_data_len,
                             void *o_data, int *o_data_len);
    PET_RESULT _get_state(void *o_data, int *o_data_len);

    PET_RESULT _set_calibration(void *i_data, int *i_data_len,
                                void *o_data, int *o_data_len);
    PET_RESULT _get_calibration(void *i_data, int *i_data_len,
                                void *o_data, int *o_data_len);

    PET_RESULT _get_device_info(void *o_data, int *o_data_len);

    PET_RESULT _set_update(void *i_data, int *i_data_len,
                           void *o_data, int *o_data_len);

    PET_RESULT _get_wave(void *i_data, int *i_data_len,
                         void *o_data, int *o_data_len);
};
//--------------------------------------------------------------------------------
#endif
