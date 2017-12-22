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
#ifndef DEVICE_BRG_HPP
#define DEVICE_BRG_HPP
//--------------------------------------------------------------------------------
#include "demo_device_base.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
class Device_BRG : public Device_base
{
    Q_OBJECT
public:
    explicit Device_BRG(QObject *parent);
    PET_RESULT check(QByteArray input_packet, QByteArray *output_packet);

    PET_RESULT f_PET_CMD_SET_CALIBRATION(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_GET_CALIBRATION(QByteArray input_packet, QByteArray *output_packet);

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
    typedef PET_RESULT (Device_BRG::*brg_func)(QByteArray, QByteArray*);
    typedef PET_RESULT (Device_base::*base_func)(QByteArray, QByteArray*);
    QList<brg_func>     brg_func_list;
    QList<base_func>    base_func_list;

    void init(void);
    void connect_log(void);

    pet_brg_calibration     calibration;
    pet_brg_ctrl            control;
    pet_brg_state           state;
    pet_brg_ch_state        ch_state;
    pet_brg_ch_ctrl         ch_ctrl;
};
//--------------------------------------------------------------------------------
#endif // DEVICE_BRG_HPP
