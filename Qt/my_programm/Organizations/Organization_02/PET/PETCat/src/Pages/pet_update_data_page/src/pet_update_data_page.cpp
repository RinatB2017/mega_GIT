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
#include <QMouseEvent>
#include <QSpinBox>
#include <QLineEdit>
//--------------------------------------------------------------------------------
#include "pet_update_data_page.hpp"
#include "ui_pet_update_data_page.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
PET_update_data_page::PET_update_data_page(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::PET_update_data_page)
{
    init();
}
//--------------------------------------------------------------------------------
PET_update_data_page::~PET_update_data_page()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PET_update_data_page::init(void)
{
    ui->setupUi(this);

    ui->sb_cnt->setRange(0, INT16_MAX);
    ui->sb_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_length->setRange(INT32_MIN, INT32_MAX);
    ui->sb_crc->setRange(INT32_MIN, INT32_MAX);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_update_data_page::set(pet_update_data_t p_data)
{
    ui->sb_cnt->setValue(p_data.cnt);
    ui->sb_offset->setValue(p_data.offset);
    ui->sb_length->setValue(p_data.length);
    ui->sb_crc->setValue(p_data.crc);

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_update_data_page::get(pet_update_data_t *p_data)
{
    Q_CHECK_PTR(p_data);
    //---
    (*p_data).cnt = ui->sb_cnt->value();
    (*p_data).offset = ui->sb_offset->value();
    (*p_data).length = ui->sb_length->value();
    (*p_data).crc = ui->sb_crc->value();

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_update_data_page::block_ctrl_interface(bool state)
{
    ui->sb_cnt->setDisabled(state);
    ui->sb_offset->setDisabled(state);
    ui->sb_length->setDisabled(state);
    ui->sb_crc->setDisabled(state);
}
//--------------------------------------------------------------------------------
void PET_update_data_page::block_state_interface(bool state)
{
    Q_UNUSED(state)
}
//--------------------------------------------------------------------------------
