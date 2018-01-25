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
#ifndef PET_DEVICE_SIM_HPP
#define PET_DEVICE_SIM_HPP
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
#include "PET_device_sim.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
class PET_device_sim : public PET_device_base
{
public:
    explicit PET_device_sim(QWidget *parent);
    ~PET_device_sim();

    int get_PID(void);

    PET_RESULT get_test(void *i_data, int *i_data_len,
                        void *o_data, int *o_data_len);

    PET_RESULT get_device_info(pet_device_info_t *data, int *data_len);

    PET_RESULT get_ctrl(pet_bos_ctrl_t *data, int *data_len);
    PET_RESULT get_state(pet_bos_state_t *data, int *data_len);

    PET_RESULT get_ch_ctrl(uint16_t *i_data, int *i_data_len,
                           pet_bos_ch_ctrl_t *o_data, int *o_data_len);
    PET_RESULT get_ch_state(uint16_t *i_data, int *i_data_len,
                            pet_bos_ch_state_t *o_data, int *o_data_len);

public slots:
    void f_import(void);
    void f_export(void);

    void save_to_EEPROM(void);
    void set_default(void);
};
//--------------------------------------------------------------------------------
#endif
