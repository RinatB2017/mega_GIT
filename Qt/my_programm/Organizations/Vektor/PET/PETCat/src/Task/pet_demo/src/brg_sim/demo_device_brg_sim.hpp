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
#ifndef DEVICE_BRG_SIM_HPP
#define DEVICE_BRG_SIM_HPP
//--------------------------------------------------------------------------------
#include "demo_device_base.hpp"
#include "demo_device_brg_sim.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg_sim.h"
//--------------------------------------------------------------------------------
class Device_BRG_SIM : public Device_base
{
    Q_OBJECT
public:
    explicit Device_BRG_SIM(QObject *parent);
    PET_RESULT check(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_SET_CONTROL(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_GET_CONTROL(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_GET_STATE(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_GET_CH_STATE(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_SET_CH_CONTROL(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_GET_CH_CONTROL(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_UPDATE(QByteArray input_packet, QByteArray *output_packet);

    void set_pet_address(uint8_t value);
    uint8_t get_pet_address(void);

private:
    typedef PET_RESULT (Device_BRG_SIM::*brg_sim_func)(QByteArray, QByteArray*);
    typedef PET_RESULT (Device_base::*base_func)(QByteArray, QByteArray*);
    QList<brg_sim_func> brg_sim_func_list;
    QList<base_func>    base_func_list;

    void init(void);
    void connect_log(void);

    pet_brg_sim_ctrl_t      control;
    pet_brg_sim_state_t     state;
    pet_brg_sim_ch_state_t  ch_state;
    pet_brg_sim_ch_ctrl_t   ch_ctrl;
};
//--------------------------------------------------------------------------------
#endif // DEVICE_BRG_SIM_HPP
