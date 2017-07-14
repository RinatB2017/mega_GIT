/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef DEVICE_SIM_HPP
#define DEVICE_SIM_HPP
//--------------------------------------------------------------------------------
#include "demo_device_base.hpp"
#include "demo_device_sim.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_sim.h"
//--------------------------------------------------------------------------------
#include "pet_errno.h"
//--------------------------------------------------------------------------------
class Device_SIM : public Device_base
{
    Q_OBJECT
public:
    explicit Device_SIM(QObject *parent);
    PET_RESULT check(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_SET_CONTROL(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_GET_CONTROL(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_GET_STATE(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_GET_CH_STATE(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_SET_CH_CONTROL(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_GET_CH_CONTROL(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_UPDATE(QByteArray input_packet, QByteArray *output_packet);

    void set_net_address(QHostAddress value);
    QHostAddress get_net_address(void);

    void set_event_address(QHostAddress value);
    QHostAddress get_event_address(void);

    void set_net_mask(quint32 value);
    void set_net_gateway(quint32 value);

    void set_mac_address(uint8_t mac_0,
                         uint8_t mac_1,
                         uint8_t mac_2,
                         uint8_t mac_3,
                         uint8_t mac_4,
                         uint8_t mac_5);
    QString get_str_mac_address(void);

    void set_cmd_port(quint16 value);
    quint16 get_cmd_port(void);

    void set_event_port(quint16 value);
    quint16 get_event_port(void);

    void set_pet_address(uint8_t value);
    uint8_t get_pet_address(void);

private:
    typedef PET_RESULT (Device_SIM::*sim_func)(QByteArray, QByteArray*);
    typedef PET_RESULT (Device_base::*base_func)(QByteArray, QByteArray*);
    QList<sim_func>     sim_func_list;
    QList<base_func>    base_func_list;

    void init(void);
    void connect_log(void);

    pet_sim_state_t     state;
    pet_sim_ch_state_t  ch_state;
    pet_sim_ctrl_t      control;
    pet_sim_ch_ctrl_t   ch_ctrl;
};
//--------------------------------------------------------------------------------
#endif // DEVICE_SIM_HPP
