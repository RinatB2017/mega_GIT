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
#include <QLineEdit>
#include <QSpinBox>
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
//--------------------------------------------------------------------------------
#include "pet_device_info_page.hpp"
#include "ui_pet_device_info_page.h"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
PET_device_info_page::PET_device_info_page(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::PET_device_info_page)
{
    init();
}
//--------------------------------------------------------------------------------
PET_device_info_page::~PET_device_info_page()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PET_device_info_page::init(void)
{
    ui->setupUi(this);

    ui->le_vid->setReadOnly(true);
    ui->le_pid->setReadOnly(true);
    ui->le_uid->setReadOnly(true);
    ui->le_hw_version->setReadOnly(true);
    ui->le_sw_version->setReadOnly(true);
    ui->le_prot_version->setReadOnly(true);
    ui->le_work_time->setReadOnly(true);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_info_page::set(pet_device_info_t p_data)
{
    ui->le_vid->setText(QString("%1").arg(PET_device_base::get_PET_VID_string(p_data.vid)));
    ui->le_pid->setText(QString("%1").arg(PET_device_base::get_PET_PID_string(p_data.pid)));
    ui->le_uid->setText(PET_device_base::get_PET_UID_string(p_data.uid));
    ui->le_hw_version->setText(PET_device_base::convert_version_to_string(p_data.hw_version));
    ui->le_sw_version->setText(PET_device_base::convert_version_to_string(p_data.sw_version));
    ui->le_prot_version->setText(PET_device_base::convert_version_to_string(p_data.prot_version));
    //ui->le_hw_timestamp->setText(PET_device_base::convert_timestamp_to_datatime_string(p_data.hw_timestamp));
    //ui->le_sw_timestamp->setText(PET_device_base::convert_timestamp_to_datatime_string(p_data.sw_timestamp));
    ui->le_work_time->setText(PET_device_base::convert_timestamp_to_datatime_string(p_data.work_time));

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_device_info_page::block_ctrl_interface(bool state)
{
    ui->le_vid->setDisabled(state);
    ui->le_pid->setDisabled(state);
    ui->le_uid->setDisabled(state);
    ui->le_hw_version->setDisabled(state);
    ui->le_sw_version->setDisabled(state);
    ui->le_prot_version->setDisabled(state);
    ui->le_work_time->setDisabled(state);

    ui->doubleSpinBox->setDisabled(state);
    ui->doubleSpinBox_2->setDisabled(state);
}
//--------------------------------------------------------------------------------
void PET_device_info_page::block_state_interface(bool state)
{
    Q_UNUSED(state)
}
//--------------------------------------------------------------------------------
