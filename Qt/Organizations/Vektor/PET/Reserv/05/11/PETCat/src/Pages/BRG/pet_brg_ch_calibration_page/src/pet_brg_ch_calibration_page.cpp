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
#include <QtMath>
//--------------------------------------------------------------------------------
#include "ui_pet_brg_ch_page.h"
#include "pet_brg_ch_calibration_page.hpp"
#include "ui_pet_brg_ch_calibration_page.h"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
PET_brg_ch_calibration_page::PET_brg_ch_calibration_page(uint16_t channel, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::PET_brg_ch_calibration_page),
    channel(channel)
{
    init();
}
//--------------------------------------------------------------------------------
PET_brg_ch_calibration_page::~PET_brg_ch_calibration_page()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PET_brg_ch_calibration_page::init(void)
{
    ui->setupUi(this);

    ui->sb_channel->setValue(channel);

    ui->sb_channel->setRange(0, INT16_MAX);
    ui->sb_flags->setRange(INT32_MIN, INT32_MAX);
    ui->sb_timestamp->setRange(INT32_MIN, INT32_MAX);
    ui->sb_xp_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_xp_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_xn_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_xn_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_yp_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_yp_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_yn_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_yn_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_ts_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_offset_set_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_offset_set_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_offset_get_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_offset_get_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_threshold_set_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_threshold_set_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_threshold_get_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_in_thresholdga_get_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_adc_ref_set_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_adc_ref_set_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_adc_ref_get_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_adc_ref_get_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_pm_bias_set_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_pm_bias_set_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_pm_bias_get_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_pm_bias_get_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_readout_temp_get_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_readout_temp_get_gain->setRange(INT32_MIN, INT32_MAX);
    ui->sb_ifmodule_temp_get_offset->setRange(INT32_MIN, INT32_MAX);
    ui->sb_ifmodule_temp_get_gain->setRange(INT32_MIN, INT32_MAX);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_ch_calibration_page::set_ctrl(pet_brg_ch_calibration_t p_data)
{
    ui->sb_channel->setValue(p_data.channel);
    ui->sb_flags->setValue(p_data.flags);
    ui->sb_timestamp->setValue(p_data.timestamp);
    ui->sb_xp_offset->setValue(p_data.xp_offset);
    ui->sb_xp_gain->setValue(p_data.xp_gain);
    ui->sb_xn_offset->setValue(p_data.xn_offset);
    ui->sb_xn_gain->setValue(p_data.xn_gain);
    ui->sb_yp_offset->setValue(p_data.yp_offset);
    ui->sb_yp_gain->setValue(p_data.yp_gain);
    ui->sb_yn_offset->setValue(p_data.yn_offset);
    ui->sb_yn_gain->setValue(p_data.yn_gain);
    ui->sb_ts_offset->setValue(p_data.ts_offset);
    ui->sb_in_offset_set_offset->setValue(p_data.in_offset_set_offset);
    ui->sb_in_offset_set_gain->setValue(p_data.in_offset_set_gain);
    ui->sb_in_offset_get_offset->setValue(p_data.in_offset_get_offset);
    ui->sb_in_offset_get_gain->setValue(p_data.in_offset_get_gain);
    ui->sb_in_threshold_set_offset->setValue(p_data.in_threshold_set_offset);
    ui->sb_in_threshold_set_gain->setValue(p_data.in_threshold_set_gain);
    ui->sb_in_threshold_get_offset->setValue(p_data.in_threshold_get_offset);
    ui->sb_in_thresholdga_get_gain->setValue(p_data.in_thresholdga_get_gain);
    ui->sb_adc_ref_set_offset->setValue(p_data.adc_ref_set_offset);
    ui->sb_adc_ref_set_gain->setValue(p_data.adc_ref_set_gain);
    ui->sb_adc_ref_get_offset->setValue(p_data.adc_ref_get_offset);
    ui->sb_adc_ref_get_gain->setValue(p_data.adc_ref_get_gain);
    ui->sb_pm_bias_set_offset->setValue(p_data.pm_bias_set_offset);
    ui->sb_pm_bias_set_gain->setValue(p_data.pm_bias_set_gain);
    ui->sb_pm_bias_get_offset->setValue(p_data.pm_bias_get_offset);
    ui->sb_pm_bias_get_gain->setValue(p_data.pm_bias_get_gain);
    ui->sb_readout_temp_get_offset->setValue(p_data.readout_temp_get_offset);
    ui->sb_readout_temp_get_gain->setValue(p_data.readout_temp_get_gain);
    ui->sb_ifmodule_temp_get_offset->setValue(p_data.ifmodule_temp_get_offset);
    ui->sb_ifmodule_temp_get_gain->setValue(p_data.ifmodule_temp_get_gain);
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_ch_calibration_page::get_ctrl(pet_brg_ch_calibration_t *p_data)
{
    Q_CHECK_PTR(p_data);
    //---
    (*p_data).channel = ui->sb_channel->value();
    (*p_data).flags = ui->sb_flags->value();
    (*p_data).timestamp = ui->sb_timestamp->value();
    (*p_data).xp_offset = ui->sb_xp_offset->value();
    (*p_data).xp_gain = ui->sb_xp_gain->value();
    (*p_data).xn_offset = ui->sb_xn_offset->value();
    (*p_data).xn_gain = ui->sb_xn_gain->value();
    (*p_data).yp_offset = ui->sb_yp_offset->value();
    (*p_data).yp_gain = ui->sb_yp_gain->value();
    (*p_data).yn_offset = ui->sb_yn_offset->value();
    (*p_data).yn_gain = ui->sb_yn_gain->value();
    (*p_data).ts_offset = ui->sb_ts_offset->value();
    (*p_data).in_offset_set_offset = ui->sb_in_offset_set_offset->value();
    (*p_data).in_offset_set_gain = ui->sb_in_offset_set_gain->value();
    (*p_data).in_offset_get_offset = ui->sb_in_offset_get_offset->value();
    (*p_data).in_offset_get_gain = ui->sb_in_offset_get_gain->value();
    (*p_data).in_threshold_set_offset = ui->sb_in_threshold_set_offset->value();
    (*p_data).in_threshold_set_gain = ui->sb_in_offset_set_gain->value();
    (*p_data).in_threshold_get_offset = ui->sb_in_threshold_get_offset->value();
    (*p_data).in_thresholdga_get_gain = ui->sb_in_thresholdga_get_gain->value();
    (*p_data).adc_ref_set_offset = ui->sb_adc_ref_set_offset->value();
    (*p_data).adc_ref_set_gain = ui->sb_adc_ref_set_gain->value();
    (*p_data).adc_ref_get_offset = ui->sb_adc_ref_get_offset->value();
    (*p_data).adc_ref_get_gain = ui->sb_adc_ref_get_gain->value();
    (*p_data).pm_bias_set_offset = ui->sb_pm_bias_set_offset->value();
    (*p_data).pm_bias_set_gain = ui->sb_pm_bias_set_gain->value();
    (*p_data).pm_bias_get_offset = ui->sb_pm_bias_get_offset->value();
    (*p_data).pm_bias_get_gain = ui->sb_pm_bias_get_gain->value();
    (*p_data).readout_temp_get_offset = ui->sb_readout_temp_get_offset->value();
    (*p_data).readout_temp_get_gain = ui->sb_readout_temp_get_gain->value();
    (*p_data).ifmodule_temp_get_offset = ui->sb_ifmodule_temp_get_offset->value();
    (*p_data).ifmodule_temp_get_gain = ui->sb_ifmodule_temp_get_gain->value();
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_brg_ch_calibration_page::block_ctrl_interface(bool state)
{
    ui->sb_channel->setDisabled(state);
    ui->sb_flags->setDisabled(state);
    ui->sb_timestamp->setDisabled(state);
    ui->sb_xp_offset->setDisabled(state);
    ui->sb_xp_gain->setDisabled(state);
    ui->sb_xn_offset->setDisabled(state);
    ui->sb_xn_gain->setDisabled(state);
    ui->sb_yp_offset->setDisabled(state);
    ui->sb_yp_gain->setDisabled(state);
    ui->sb_yn_offset->setDisabled(state);
    ui->sb_yn_gain->setDisabled(state);
    ui->sb_ts_offset->setDisabled(state);
    ui->sb_in_offset_set_offset->setDisabled(state);
    ui->sb_in_offset_set_gain->setDisabled(state);
    ui->sb_in_offset_get_offset->setDisabled(state);
    ui->sb_in_offset_get_gain->setDisabled(state);
    ui->sb_in_threshold_set_offset->setDisabled(state);
    ui->sb_in_threshold_set_gain->setDisabled(state);
    ui->sb_in_threshold_get_offset->setDisabled(state);
    ui->sb_in_thresholdga_get_gain->setDisabled(state);
    ui->sb_adc_ref_set_offset->setDisabled(state);
    ui->sb_adc_ref_set_gain->setDisabled(state);
    ui->sb_adc_ref_get_offset->setDisabled(state);
    ui->sb_adc_ref_get_gain->setDisabled(state);
    ui->sb_pm_bias_set_offset->setDisabled(state);
    ui->sb_pm_bias_set_gain->setDisabled(state);
    ui->sb_pm_bias_get_offset->setDisabled(state);
    ui->sb_pm_bias_get_gain->setDisabled(state);
    ui->sb_readout_temp_get_offset->setDisabled(state);
    ui->sb_readout_temp_get_gain->setDisabled(state);
    ui->sb_ifmodule_temp_get_offset->setDisabled(state);
    ui->sb_ifmodule_temp_get_gain->setDisabled(state);
}
//--------------------------------------------------------------------------------
