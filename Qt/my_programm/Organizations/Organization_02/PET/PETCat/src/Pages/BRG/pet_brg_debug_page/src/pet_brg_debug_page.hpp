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
#ifndef PET_BRG_DEBUG_PAGE_HPP
#define PET_BRG_DEBUG_PAGE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class PET_brg_debug_page;
}
//--------------------------------------------------------------------------------
class PET_brg_debug_page : public PET_device_brg
{
    Q_OBJECT

public:
    explicit PET_brg_debug_page(QWidget *parent = nullptr);
    ~PET_brg_debug_page();

    PET_RESULT set(pet_brg_ctrl_t p_data);
    PET_RESULT get(pet_brg_ctrl_t *p_data);

    void block_ctrl_interface(bool state);
    void block_state_interface(bool state);

public slots:
    void save_to_EEPROM(void);
    void set_default(void);

private slots:
    void btn_save_to_EEPROM_click(void);
    void btn_set_default_click(void);

private:
    Ui::PET_brg_debug_page *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
