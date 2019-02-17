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
#ifndef PET_UPDATE_DATA_PAGE_HPP
#define PET_UPDATE_DATA_PAGE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class PET_update_data_page;
}
//--------------------------------------------------------------------------------
class PET_update_data_page : public MyWidget
{
    Q_OBJECT

public:
    explicit PET_update_data_page(QWidget *parent = nullptr);
    ~PET_update_data_page();

    PET_RESULT set(pet_update_data_t p_data);
    PET_RESULT get(pet_update_data_t *p_data);

    void block_ctrl_interface(bool state);
    void block_state_interface(bool state);

private:
    Ui::PET_update_data_page *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
