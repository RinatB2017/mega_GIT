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
#ifndef PET_BRG_SIM_CH_PAGE_HPP
#define PET_BRG_SIM_CH_PAGE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class PET_brg_sim_ch_page;
}
//--------------------------------------------------------------------------------
class PET_brg_sim_ch_page : public MyWidget
{
    Q_OBJECT

public:
    explicit PET_brg_sim_ch_page(uint16_t channel, QWidget *parent = 0);
    ~PET_brg_sim_ch_page();

    PET_RESULT set_ctrl(pet_brg_ch_ctrl_t p_data);
    PET_RESULT get_ctrl(pet_brg_ch_ctrl_t *p_data);

    PET_RESULT set_state(pet_brg_ch_state_t p_data);

    void block_ctrl_interface(bool state);
    void block_state_interface(bool state);

private:
    Ui::PET_brg_sim_ch_page *ui = 0;
    uint16_t channel;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
