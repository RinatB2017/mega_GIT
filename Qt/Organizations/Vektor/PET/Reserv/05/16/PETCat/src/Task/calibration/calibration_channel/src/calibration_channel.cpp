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
#include <QPushButton>
//--------------------------------------------------------------------------------
#include "calibration_channel.hpp"
#include "ui_calibration_channel.h"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "imitator_brg.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
// pet_brg_ch_calibration
//--------------------------------------------------------------------------------
Calibration_channel::Calibration_channel(int num_channel,
                                         QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Calibration_channel),
    channel(num_channel)
{
    init();
}
//--------------------------------------------------------------------------------
Calibration_channel::~Calibration_channel()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Calibration_channel::init(void)
{
    ui->setupUi(this);

    ui->sb_K_xp->setRange(INT32_MIN, INT32_MAX);
    ui->sb_K_xn->setRange(INT32_MIN, INT32_MAX);
    ui->sb_K_yp->setRange(INT32_MIN, INT32_MAX);
    ui->sb_K_yn->setRange(INT32_MIN, INT32_MAX);
    ui->sb_B_xp->setRange(INT32_MIN, INT32_MAX);
    ui->sb_B_xn->setRange(INT32_MIN, INT32_MAX);
    ui->sb_B_yp->setRange(INT32_MIN, INT32_MAX);
    ui->sb_B_yn->setRange(INT32_MIN, INT32_MAX);

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

    ui->dsb_readout_temp_get_offset->setRange(-100, 100);
    ui->dsb_readout_temp_get_gain->setRange(-100, 100);
    ui->dsb_ifmodule_temp_get_offset->setRange(-100, 100);
    ui->dsb_ifmodule_temp_get_gain->setRange(-100, 100);

    //---
    connect(ui->btn_coord,          SIGNAL(clicked(bool)),    this,   SLOT(run_calib_coord()));

    connect(ui->btn_adc_ref,        SIGNAL(clicked(bool)),    this,   SIGNAL(run_calib_adc_ref()));
    connect(ui->btn_ifmodule_temp,  SIGNAL(clicked(bool)),    this,   SIGNAL(run_calib_ifmodule_temp()));
    connect(ui->btn_offset,         SIGNAL(clicked(bool)),    this,   SIGNAL(run_calib_offset()));
    connect(ui->btn_pm_bias,        SIGNAL(clicked(bool)),    this,   SIGNAL(run_calib_pm_bias()));
    connect(ui->btn_readout_temp,   SIGNAL(clicked(bool)),    this,   SIGNAL(run_calib_readout_temp()));
    connect(ui->btn_threshold,      SIGNAL(clicked(bool)),    this,   SIGNAL(run_calib_threshold()));
    connect(ui->btn_ts_offset,      SIGNAL(clicked(bool)),    this,   SIGNAL(run_calib_ts_offset()));

    ui->btn_coord->setProperty("num_channel", channel);
    ui->btn_adc_ref->setProperty("num_channel", channel);
    ui->btn_ifmodule_temp->setProperty("num_channel", channel);
    ui->btn_offset->setProperty("num_channel", channel);
    ui->btn_pm_bias->setProperty("num_channel", channel);
    ui->btn_readout_temp->setProperty("num_channel", channel);
    ui->btn_threshold->setProperty("num_channel", channel);
    ui->btn_ts_offset->setProperty("num_channel", channel);

    //ui->btn_coord->setDisabled(true);
    ui->btn_adc_ref->setDisabled(true);
    ui->btn_ifmodule_temp->setDisabled(true);
    ui->btn_offset->setDisabled(true);
    ui->btn_pm_bias->setDisabled(true);
    ui->btn_readout_temp->setDisabled(true);
    ui->btn_threshold->setDisabled(true);
    ui->btn_ts_offset->setDisabled(true);
    //---
}
//--------------------------------------------------------------------------------
void Calibration_channel::run_calib_coord(void)
{
    emit run_calib_coord(channel);
}
//--------------------------------------------------------------------------------
bool Calibration_channel::set(pet_brg_ch_calibration data)
{
    ui->sb_B_xp->setValue(data.xp_offset);
    ui->sb_K_xp->setValue(data.xp_gain);
    ui->sb_B_xn->setValue(data.xn_offset);
    ui->sb_K_xn->setValue(data.xn_gain);
    ui->sb_B_yp->setValue(data.yp_offset);
    ui->sb_K_yp->setValue(data.yp_gain);
    ui->sb_B_yn->setValue(data.yn_offset);
    ui->sb_K_yn->setValue(data.yn_gain);

    ui->sb_ts_offset->setValue(data.ts_offset);

    ui->sb_in_offset_set_offset->setValue(data.in_offset_set_offset);
    ui->sb_in_offset_set_gain->setValue(data.in_offset_set_gain);
    ui->sb_in_offset_get_offset->setValue(data.in_offset_get_offset);
    ui->sb_in_offset_get_gain->setValue(data.in_offset_get_gain);

    ui->sb_in_threshold_set_offset->setValue(data.in_threshold_set_offset);
    ui->sb_in_threshold_set_gain->setValue(data.in_threshold_set_gain);
    ui->sb_in_threshold_get_offset->setValue(data.in_threshold_get_offset);
    ui->sb_in_thresholdga_get_gain->setValue(data.in_thresholdga_get_gain);

    ui->sb_adc_ref_set_offset->setValue(data.adc_ref_set_offset);
    ui->sb_adc_ref_set_gain->setValue(data.adc_ref_set_gain);
    ui->sb_adc_ref_get_offset->setValue(data.adc_ref_get_offset);
    ui->sb_adc_ref_get_gain->setValue(data.adc_ref_get_gain);

    ui->sb_pm_bias_set_offset->setValue(data.pm_bias_set_offset);
    ui->sb_pm_bias_set_gain->setValue(data.pm_bias_set_gain);
    ui->sb_pm_bias_get_offset->setValue(data.pm_bias_get_offset);
    ui->sb_pm_bias_get_gain->setValue(data.pm_bias_get_gain);

    ui->dsb_readout_temp_get_offset->setValue(data.readout_temp_get_offset / 10.0);
    ui->dsb_readout_temp_get_gain->setValue(data.readout_temp_get_gain / 10.0);
    ui->dsb_ifmodule_temp_get_offset->setValue(data.ifmodule_temp_get_offset / 10.0);
    ui->dsb_ifmodule_temp_get_gain->setValue(data.ifmodule_temp_get_gain / 10.0);

    return true;
}
//--------------------------------------------------------------------------------
bool Calibration_channel::get(pet_brg_ch_calibration *data)
{
    (*data).xp_offset = ui->sb_B_xp->value();
    (*data).xp_gain = ui->sb_K_xp->value();
    (*data).xn_offset = ui->sb_B_xn->value();
    (*data).xn_gain = ui->sb_K_xn->value();
    (*data).yp_offset = ui->sb_B_yp->value();
    (*data).yp_gain = ui->sb_K_yp->value();
    (*data).yn_offset = ui->sb_B_yn->value();
    (*data).yn_gain = ui->sb_K_yn->value();

    (*data).ts_offset = ui->sb_ts_offset->value();

    (*data).in_offset_set_offset = ui->sb_in_threshold_set_offset->value();
    (*data).in_offset_set_gain = ui->sb_in_offset_set_gain->value();
    (*data).in_offset_get_offset = ui->sb_in_offset_get_offset->value();
    (*data).in_offset_get_gain = ui->sb_in_offset_get_gain->value();

    (*data).in_threshold_set_offset = ui->sb_in_threshold_set_offset->value();
    (*data).in_threshold_set_gain = ui->sb_in_threshold_set_gain->value();
    (*data).in_threshold_get_offset = ui->sb_in_threshold_get_offset->value();
    (*data).in_thresholdga_get_gain = ui->sb_in_thresholdga_get_gain->value();

    (*data).adc_ref_set_offset = ui->sb_adc_ref_set_offset->value();
    (*data).adc_ref_set_gain = ui->sb_adc_ref_set_gain->value();
    (*data).adc_ref_get_offset = ui->sb_adc_ref_get_offset->value();
    (*data).adc_ref_get_gain = ui->sb_adc_ref_get_gain->value();

    (*data).pm_bias_set_offset = ui->sb_pm_bias_set_offset->value();
    (*data).pm_bias_set_gain = ui->sb_pm_bias_set_gain->value();
    (*data).pm_bias_get_offset = ui->sb_pm_bias_get_offset->value();
    (*data).pm_bias_get_gain = ui->sb_pm_bias_get_gain->value();

    (*data).readout_temp_get_offset = ui->dsb_readout_temp_get_offset->value() * 10.0;
    (*data).readout_temp_get_gain = ui->dsb_readout_temp_get_gain->value() * 10.0;
    (*data).ifmodule_temp_get_offset = ui->dsb_ifmodule_temp_get_offset->value() * 10.0;
    (*data).ifmodule_temp_get_gain = ui->dsb_ifmodule_temp_get_gain->value() * 10.0;

    return true;
}
//--------------------------------------------------------------------------------
