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
#include "database.hpp"
#include "ui_database.h"
//--------------------------------------------------------------------------------
QList<StructProperty_t> l_pet_discovery_info ;
QList<StructProperty_t> l_pet_device_info ;
QList<StructProperty_t> l_pet_update_data ;
QList<StructProperty_t> l_pet_wave_point ;
QList<StructProperty_t> l_pet_event ;

QList<StructProperty_t> l_pet_bos_ctrl ;
QList<StructProperty_t> l_pet_bos_state ;
QList<StructProperty_t> l_pet_bos_ch_ctrl ;
QList<StructProperty_t> l_pet_bos_ch_state ;

QList<StructProperty_t> l_pet_brg_ctrl ;
QList<StructProperty_t> l_pet_brg_state ;
QList<StructProperty_t> l_pet_brg_ch_ctrl ;
QList<StructProperty_t> l_pet_brg_ch_state ;
QList<StructProperty_t> l_pet_brg_ch_calibration ;
QList<StructProperty_t> l_pet_brg_calibration ;
//--------------------------------------------------------------------------------
Database::Database(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Database)
{
    init();
}
//--------------------------------------------------------------------------------
Database::~Database()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Database::init(void)
{
    ui->setupUi(this);

    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();
    //---
    QtProperty *i_pet_discovery_info = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_discovery_info"));
    l_pet_discovery_info.clear();
    add_property(variantManager, &l_pet_discovery_info, tr("vid"),   "vid",  QVariant::Int);
    add_property(variantManager, &l_pet_discovery_info, tr("pid"),   "pid",  QVariant::Int);
    add_property(variantManager, &l_pet_discovery_info, tr("uid"),   "uid",  QVariant::String);
    foreach (StructProperty_t prop, l_pet_discovery_info)
    {
        i_pet_discovery_info->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_device_info = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_device_info"));
    l_pet_device_info.clear();
    add_property(variantManager, &l_pet_device_info, tr("vid"),             "vid",          QVariant::Int);
    add_property(variantManager, &l_pet_device_info, tr("pid"),             "pid",          QVariant::Int);
    add_property(variantManager, &l_pet_device_info, tr("uid"),             "uid",          QVariant::String);
    add_property(variantManager, &l_pet_device_info, tr("hw_version"),      "hw_version",   QVariant::Int);
    add_property(variantManager, &l_pet_device_info, tr("sw_version"),      "sw_version",   QVariant::Int);
    add_property(variantManager, &l_pet_device_info, tr("prot_version"),    "prot_version", QVariant::Int);
    add_property(variantManager, &l_pet_device_info, tr("hw_timestamp"),    "hw_timestamp", QVariant::Int);
    add_property(variantManager, &l_pet_device_info, tr("sw_timestamp"),    "sw_timestamp", QVariant::Int);
    add_property(variantManager, &l_pet_device_info, tr("work_time"),       "work_time",    QVariant::Int);
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        i_pet_device_info->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_update_data = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_update_data"));
    l_pet_update_data.clear();
    add_property(variantManager, &l_pet_update_data, tr("ctrl"),    "ctrl",  QVariant::Int);
    add_property(variantManager, &l_pet_update_data, tr("offset"),  "offset",  QVariant::Int);
    add_property(variantManager, &l_pet_update_data, tr("length"),  "length",  QVariant::Int);
    add_property(variantManager, &l_pet_update_data, tr("crc"),     "crc",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_update_data)
    {
        i_pet_update_data->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_wave_point = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_wave_point"));
    l_pet_wave_point.clear();
    add_property(variantManager, &l_pet_wave_point, tr("xp"),   "xp",  QVariant::Int);
    add_property(variantManager, &l_pet_wave_point, tr("xn"),   "xn",  QVariant::Int);
    add_property(variantManager, &l_pet_wave_point, tr("yp"),   "yp",  QVariant::Int);
    add_property(variantManager, &l_pet_wave_point, tr("yn"),   "yn",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_wave_point)
    {
        i_pet_wave_point->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_event = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_event"));
    l_pet_event.clear();
    add_property(variantManager, &l_pet_event, tr("src"),       "src",  QVariant::Int);
    add_property(variantManager, &l_pet_event, tr("flags"),     "flags",  QVariant::Int);
    add_property(variantManager, &l_pet_event, tr("ts_fract"),  "ts_fract",  QVariant::Int);
    add_property(variantManager, &l_pet_event, tr("ts"),        "ts",  QVariant::Int);
    add_property(variantManager, &l_pet_event, tr("xp"),        "xp",  QVariant::Int);
    add_property(variantManager, &l_pet_event, tr("xn"),        "xn",  QVariant::Int);
    add_property(variantManager, &l_pet_event, tr("yp"),        "yp",  QVariant::Int);
    add_property(variantManager, &l_pet_event, tr("yn"),        "yn",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_event)
    {
        i_pet_event->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_bos_ctrl = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_bos_ctrl"));
    l_pet_bos_ctrl.clear();
    add_property(variantManager, &l_pet_bos_ctrl, tr("trn_ctrl"),   "trn_ctrl",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("timeout"),    "timeout",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("net_ctrl"),   "net_ctrl",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("ip_address"), "ip_address",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("netmask"),    "netmask",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("gateway"),    "gateway",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("mac_addr"),   "mac_addr",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("cmd_port"),   "cmd_port",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("event_port"), "event_port",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ctrl, tr("event_addr"), "event_addr",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_bos_ctrl)
    {
        i_pet_bos_ctrl->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_bos_state = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_bos_state"));
    l_pet_bos_state.clear();
    add_property(variantManager, &l_pet_bos_state, tr("state"),         "state",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_state, tr("flags"),         "flags",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_state, tr("sync_cycle"),    "sync_cycle",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_state, tr("utilisation"),   "utilisation",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_bos_state)
    {
        i_pet_bos_state->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_bos_ch_ctrl = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_bos_ch_ctrl"));
    l_pet_bos_ch_ctrl.clear();
    add_property(variantManager, &l_pet_bos_ch_ctrl, tr("channel"), "channel",  QVariant::Int);
    add_property(variantManager, &l_pet_bos_ch_ctrl, tr("ctrl"),    "ctrl",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_bos_ch_ctrl)
    {
        i_pet_bos_ch_ctrl->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_bos_ch_state = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_bos_ch_state"));
    l_pet_bos_ch_state.clear();
    add_property(variantManager, &l_pet_bos_ch_state, tr("channel"),        "channel",      QVariant::Int);
    add_property(variantManager, &l_pet_bos_ch_state, tr("state"),          "state",        QVariant::Int);
    add_property(variantManager, &l_pet_bos_ch_state, tr("flags"),          "flags",        QVariant::Int);
    add_property(variantManager, &l_pet_bos_ch_state, tr("sync_cycle"),     "sync_cycle",   QVariant::Int);
    add_property(variantManager, &l_pet_bos_ch_state, tr("utilisation"),    "utilisation",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_bos_ch_state)
    {
        i_pet_bos_ch_state->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_brg_ctrl = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_brg_ctrl"));
    l_pet_brg_ctrl.clear();
    add_property(variantManager, &l_pet_brg_ctrl, tr("brg_mode"),   "brg_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ctrl, tr("sync_mode"),   "sync_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ctrl, tr("sync_period"),   "sync_period",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ctrl, tr("log_severity"),   "log_severity",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ctrl, tr("tp1_sel"),   "tp1_sel",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ctrl, tr("tp2_sel"),   "tp2_sel",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ctrl, tr("rf1_sel"),   "rf1_sel",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ctrl, tr("rf2_sel"),   "rf2_sel",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_brg_ctrl)
    {
        i_pet_brg_ctrl->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_brg_state = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_brg_state"));
    l_pet_brg_state.clear();
    add_property(variantManager, &l_pet_brg_state, tr("state"),   "state",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_state, tr("flags"),   "flags",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_state, tr("sync_cycle"),   "sync_cycle",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_state, tr("utilisation"),   "utilisation",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_brg_state)
    {
        i_pet_brg_state->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_brg_ch_ctrl = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_brg_ch_ctrl"));
    l_pet_brg_ch_ctrl.clear();
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("channel"),   "channel",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("mode"),   "mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("gate_width"),   "gate_width",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("event_src"),   "event_src",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("pm_bias"),   "pm_bias",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("pm_bias_deviation"),   "pm_bias_deviation",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("readout_temp_max"),   "readout_temp_max",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("adc_reference"),   "adc_reference",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("in_offset"),   "in_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("in_polarity"),   "in_polarity",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("in_scale"),   "in_scale",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("ifmodule_temp_max"),   "ifmodule_temp_max",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("cfd_mode"),   "cfd_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("cfd_lpf_mode"),   "cfd_lpf_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("cfd_l_threshold"),   "cfd_l_threshold",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("cfd_d_threshold"),   "cfd_d_threshold",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("cfd_fract"),   "cfd_fract",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("blr_mode"),   "blr_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("blr_fc"),   "blr_fc",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("blr_gate_width"),   "blr_gate_width",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("blr_offset"),   "blr_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("blr_threshold"),   "blr_threshold",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("fir_mode"),   "fir_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("fir_fc"),   "fir_fc",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("qdc_mode"),   "qdc_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("qdc_predelay"),   "qdc_predelay",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("qdc_scale"),   "qdc_scale",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("tdc_mode"),   "tdc_mode",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("tdc_adj"),   "tdc_adj",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("dso_time_step"),   "dso_time_step",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("dso_value_step"),   "dso_value_step",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("dso_prefetch"),   "dso_prefetch",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_ctrl, tr("in_gain"),   "in_gain",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_brg_ch_ctrl)
    {
        i_pet_brg_ch_ctrl->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_brg_ch_state = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_brg_ch_state"));
    l_pet_brg_ch_state.clear();
    add_property(variantManager, &l_pet_brg_ch_state, tr("channel"),   "channel",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("state"),   "state",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("flags"),   "flags",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("in_offset"),   "in_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("in_threshold"),   "in_threshold",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("adc_reference"),   "adc_reference",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("pm_bias"),   "pm_bias",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("readout_temp"),   "readout_temp",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("ifmodule_temp"),   "ifmodule_temp",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("sync_cycle"),   "sync_cycle",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("utilisation"),   "utilisation",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("adc_underflow"),   "adc_underflow",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("adc_overflow"),   "adc_overflow",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("qdc_underflow"),   "qdc_underflow",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("qdc_overflow"),   "qdc_overflow",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("gate_overlay"),   "gate_overlay",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_state, tr("ts_invalid"),   "ts_invalid",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_brg_ch_state)
    {
        i_pet_brg_ch_state->addSubProperty(prop.item);
    }
    //---
    QtProperty *i_pet_brg_ch_calibration = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_brg_ch_calibration"));
    l_pet_brg_ch_calibration.clear();
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("channel"),   "channel",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("flags"),   "flags",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("timestamp"),   "timestamp",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("xp_offset"),   "xp_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("xp_gain"),   "xp_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("xn_offset"),   "xn_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("xn_gain"),   "xn_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("yp_offset"),   "yp_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("yp_gain"),   "yp_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("yn_offset"),   "yn_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("yn_gain"),   "yn_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("ts_offset"),   "ts_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_offset_set_offset"),   "in_offset_set_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_offset_set_gain"),   "in_offset_set_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_offset_get_offset"),   "in_offset_get_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_offset_get_gain"),   "in_offset_get_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_threshold_set_offset"),   "in_threshold_set_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_threshold_set_gain"),   "in_threshold_set_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_threshold_get_offset"),   "in_threshold_get_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("in_thresholdga_get_gain"),   "in_thresholdga_get_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("adc_ref_set_offset"),   "adc_ref_set_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("adc_ref_set_gain"),   "adc_ref_set_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("adc_ref_get_offset"),   "adc_ref_get_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("adc_ref_get_gain"),   "adc_ref_get_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("pm_bias_set_offset"),   "pm_bias_set_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("pm_bias_set_gain"),   "pm_bias_set_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("pm_bias_get_offset"),   "pm_bias_get_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("pm_bias_get_gain"),   "pm_bias_get_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("readout_temp_get_offset"),   "readout_temp_get_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("readout_temp_get_gain"),   "readout_temp_get_gain",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("ifmodule_temp_get_offset"),   "ifmodule_temp_get_offset",  QVariant::Int);
    add_property(variantManager, &l_pet_brg_ch_calibration, tr("ifmodule_temp_get_gain"),   "ifmodule_temp_get_gain",  QVariant::Int);
    foreach (StructProperty_t prop, l_pet_brg_ch_calibration)
    {
        i_pet_brg_ch_calibration->addSubProperty(prop.item);
    }
    //---
    //    QtProperty *i_pet_brg_calibration = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("pet_brg_calibration"));
    //    l_pet_brg_calibration.clear();
    //    add_property(variantManager, &l_pet_brg_calibration, tr(""),   "",  QVariant::Int);
    //    foreach (StructProperty_t prop, l_pet_brg_calibration)
    //    {
    //        i_pet_brg_calibration->addSubProperty(prop.item);
    //    }
    //---

    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    QtTreePropertyBrowser *variantEditor = new QtTreePropertyBrowser(this);
    variantEditor->setFactoryForManager(variantManager, variantFactory);

    variantEditor->addProperty(i_pet_discovery_info);
    variantEditor->addProperty(i_pet_device_info);
    variantEditor->addProperty(i_pet_update_data);
    variantEditor->addProperty(i_pet_wave_point);
    variantEditor->addProperty(i_pet_event);
    variantEditor->addProperty(i_pet_bos_ctrl);
    variantEditor->addProperty(i_pet_bos_state);
    variantEditor->addProperty(i_pet_bos_ch_ctrl);
    variantEditor->addProperty(i_pet_bos_ch_state);
    variantEditor->addProperty(i_pet_brg_ctrl);
    variantEditor->addProperty(i_pet_brg_state);
    variantEditor->addProperty(i_pet_brg_ch_ctrl);
    variantEditor->addProperty(i_pet_brg_ch_state);
    variantEditor->addProperty(i_pet_brg_ch_calibration);
    //variantEditor->addProperty(i_pet_brg_calibration);

    //variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(true);
    variantEditor->setMinimumSize(variantEditor->sizeHint());
    variantEditor->setHeaderVisible(false);

    ui->options_layout->addWidget(variantEditor);
    //---
}
//--------------------------------------------------------------------------------
void Database::add_property(QtVariantPropertyManager *variantManager,
                            QList<StructProperty_t>  *l_property,
                            QString name,
                            QString object_name,
                            int propertyType)
{
    StructProperty_t d_property;
    //---
    d_property.name = name;
    d_property.object_name = object_name;
    d_property.propertyType = propertyType;
    d_property.item = variantManager->addProperty(d_property.propertyType, d_property.name);
    //---
    (*l_property).append(d_property);
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_discovery_info data)
{
    foreach (StructProperty_t prop, l_pet_discovery_info)
    {
        if(prop.name == "pid")  prop.item->setValue(data.pid);
        if(prop.name == "vid")  prop.item->setValue(data.vid);
        if(prop.name == "uid")  prop.item->setValue(QString("%1%2%3%4%5%6")
                                                    .arg(data.uid[0])
                .arg(data.uid[1])
                .arg(data.uid[2])
                .arg(data.uid[3])
                .arg(data.uid[4])
                .arg(data.uid[5]));

        //        QByteArray ba;
        //        ba.clear();
        //        prop.item->setValue(ba);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_device_info data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "pid")  prop.item->setValue(data.pid);
        if(prop.name == "vid")  prop.item->setValue(data.vid);
        //if(prop.name == "uid")  prop.item->setValue(data.uid);
        if(prop.name == "hw_version")  prop.item->setValue(data.pid);
        if(prop.name == "sw_version")  prop.item->setValue(data.pid);
        if(prop.name == "prot_version")  prop.item->setValue(data.pid);
        if(prop.name == "hw_timestamp")  prop.item->setValue(data.pid);
        if(prop.name == "sw_timestamp")  prop.item->setValue(data.pid);
        if(prop.name == "work_time")  prop.item->setValue(data.pid);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_update_data data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "ctrl")  prop.item->setValue(data.ctrl);
        if(prop.name == "offset")  prop.item->setValue(data.offset);
        if(prop.name == "length")  prop.item->setValue(data.length);
        if(prop.name == "crc")  prop.item->setValue(data.crc);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_wave_point data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "xn")  prop.item->setValue(data.xn);
        if(prop.name == "xp")  prop.item->setValue(data.xp);
        if(prop.name == "yn")  prop.item->setValue(data.yn);
        if(prop.name == "yp")  prop.item->setValue(data.yp);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_event data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "src")  prop.item->setValue(data.src);
        if(prop.name == "flags")  prop.item->setValue(data.flags);
        if(prop.name == "ts_fract")  prop.item->setValue(data.ts_fract);
        if(prop.name == "ts")  prop.item->setValue(data.ts);
        if(prop.name == "xp")  prop.item->setValue(data.xp);
        if(prop.name == "xn")  prop.item->setValue(data.xn);
        if(prop.name == "yp")  prop.item->setValue(data.yp);
        if(prop.name == "yn")  prop.item->setValue(data.yn);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_bos_ctrl data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "trn_ctrl")  prop.item->setValue(data.trn_ctrl);
        if(prop.name == "timeout")  prop.item->setValue(data.timeout);
        if(prop.name == "net_ctrl")  prop.item->setValue(data.net_ctrl);
        //if(prop.name == "ip_address")  prop.item->setValue(data.ip_address);
        //if(prop.name == "netmask")  prop.item->setValue(data.netmask);
        //if(prop.name == "gateway")  prop.item->setValue(data.gateway);
        //if(prop.name == "mac_addr")  prop.item->setValue(data.mac_addr);
        if(prop.name == "cmd_port")  prop.item->setValue(data.cmd_port);
        if(prop.name == "event_port")  prop.item->setValue(data.event_port);
        //if(prop.name == "event_addr")  prop.item->setValue(data.event_addr);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_bos_state data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "state")  prop.item->setValue(data.state);
        if(prop.name == "flags")  prop.item->setValue(data.flags);
        if(prop.name == "sync_cycle")  prop.item->setValue(data.sync_cycle);
        if(prop.name == "utilisation")  prop.item->setValue(data.utilisation);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_bos_ch_ctrl data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "channel")  prop.item->setValue(data.channel);
        if(prop.name == "ctrl")  prop.item->setValue(data.ctrl);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_bos_ch_state data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "channel")  prop.item->setValue(data.channel);
        if(prop.name == "state")  prop.item->setValue(data.state);
        if(prop.name == "flags")  prop.item->setValue(data.flags);
        if(prop.name == "sync_cycle")  prop.item->setValue(data.sync_cycle);
        if(prop.name == "utilisation")  prop.item->setValue(data.utilisation);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_brg_ctrl data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "brg_mode")  prop.item->setValue(data.brg_mode);
        if(prop.name == "sync_mode")  prop.item->setValue(data.sync_mode);
        if(prop.name == "sync_period")  prop.item->setValue(data.sync_period);
        if(prop.name == "log_severity")  prop.item->setValue(data.log_severity);
        if(prop.name == "tp1_sel")  prop.item->setValue(data.tp1_sel);
        if(prop.name == "tp2_sel")  prop.item->setValue(data.tp2_sel);
        if(prop.name == "rf1_sel")  prop.item->setValue(data.rf1_sel);
        if(prop.name == "rf2_sel")  prop.item->setValue(data.rf2_sel);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_brg_state data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "state")  prop.item->setValue(data.state);
        if(prop.name == "flags")  prop.item->setValue(data.flags);
        if(prop.name == "sync_cycle")  prop.item->setValue(data.sync_cycle);
        if(prop.name == "utilisation")  prop.item->setValue(data.utilisation);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_brg_ch_ctrl data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "channel")  prop.item->setValue(data.channel);
        if(prop.name == "mode")  prop.item->setValue(data.mode);
        if(prop.name == "gate_width")  prop.item->setValue(data.gate_width);
        if(prop.name == "event_src")  prop.item->setValue(data.event_src);
        if(prop.name == "pm_bias")  prop.item->setValue(data.pm_bias);
        if(prop.name == "pm_bias_deviation")  prop.item->setValue(data.pm_bias_deviation);
        if(prop.name == "readout_temp_max")  prop.item->setValue(data.readout_temp_max);
        if(prop.name == "adc_reference")  prop.item->setValue(data.adc_reference);
        if(prop.name == "in_offset")  prop.item->setValue(data.in_offset);
        if(prop.name == "in_polarity")  prop.item->setValue(data.in_polarity);
        if(prop.name == "in_scale")  prop.item->setValue(data.in_scale);
        if(prop.name == "ifmodule_temp_max")  prop.item->setValue(data.ifmodule_temp_max);
        if(prop.name == "cfd_mode")  prop.item->setValue(data.cfd_mode);
        if(prop.name == "cfd_lpf_mode")  prop.item->setValue(data.cfd_lpf_mode);
        if(prop.name == "cfd_l_threshold")  prop.item->setValue(data.cfd_l_threshold);
        if(prop.name == "cfd_d_threshold")  prop.item->setValue(data.cfd_d_threshold);
        if(prop.name == "cfd_fract")  prop.item->setValue(data.cfd_fract);
        if(prop.name == "blr_mode")  prop.item->setValue(data.blr_mode);
        if(prop.name == "blr_fc")  prop.item->setValue(data.blr_fc);
        if(prop.name == "blr_gate_width")  prop.item->setValue(data.blr_gate_width);
        if(prop.name == "blr_offset")  prop.item->setValue(data.blr_offset);
        if(prop.name == "blr_threshold")  prop.item->setValue(data.blr_threshold);
        if(prop.name == "fir_mode")  prop.item->setValue(data.fir_mode);
        if(prop.name == "fir_fc")  prop.item->setValue(data.fir_fc);
        if(prop.name == "qdc_mode")  prop.item->setValue(data.qdc_mode);
        if(prop.name == "qdc_predelay")  prop.item->setValue(data.qdc_predelay);
        if(prop.name == "qdc_scale")  prop.item->setValue(data.qdc_scale);
        if(prop.name == "tdc_mode")  prop.item->setValue(data.tdc_mode);
        if(prop.name == "tdc_adj")  prop.item->setValue(data.tdc_adj);
        if(prop.name == "dso_time_step")  prop.item->setValue(data.dso_time_step);
        if(prop.name == "dso_value_step")  prop.item->setValue(data.dso_value_step);
        if(prop.name == "dso_prefetch")  prop.item->setValue(data.dso_prefetch);
        if(prop.name == "in_gain")  prop.item->setValue(data.in_gain);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_brg_ch_state data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "channel")  prop.item->setValue(data.channel);
        if(prop.name == "state")  prop.item->setValue(data.state);
        if(prop.name == "flags")  prop.item->setValue(data.flags);
        if(prop.name == "in_offset")  prop.item->setValue(data.in_offset);
        if(prop.name == "in_threshold")  prop.item->setValue(data.in_threshold);
        if(prop.name == "adc_reference")  prop.item->setValue(data.adc_reference);
        if(prop.name == "pm_bias")  prop.item->setValue(data.pm_bias);
        if(prop.name == "readout_temp")  prop.item->setValue(data.readout_temp);
        if(prop.name == "ifmodule_temp")  prop.item->setValue(data.ifmodule_temp);
        if(prop.name == "sync_cycle")  prop.item->setValue(data.sync_cycle);
        if(prop.name == "utilisation")  prop.item->setValue(data.utilisation);
        if(prop.name == "adc_underflow")  prop.item->setValue(data.adc_underflow);
        if(prop.name == "adc_overflow")  prop.item->setValue(data.adc_overflow);
        if(prop.name == "qdc_underflow")  prop.item->setValue(data.qdc_underflow);
        if(prop.name == "qdc_overflow")  prop.item->setValue(data.qdc_overflow);
        if(prop.name == "gate_overlay")  prop.item->setValue(data.gate_overlay);
        if(prop.name == "ts_invalid")  prop.item->setValue(data.ts_invalid);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::set(pet_brg_ch_calibration data)
{
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "channel")  prop.item->setValue(data.channel);
        if(prop.name == "flags")  prop.item->setValue(data.flags);
        if(prop.name == "timestamp")  prop.item->setValue(data.timestamp);
        if(prop.name == "xp_offset")  prop.item->setValue(data.xp_offset);
        if(prop.name == "xp_gain")  prop.item->setValue(data.xp_gain);
        if(prop.name == "xn_offset")  prop.item->setValue(data.xn_offset);
        if(prop.name == "xn_gain")  prop.item->setValue(data.xn_gain);
        if(prop.name == "yp_offset")  prop.item->setValue(data.yp_offset);
        if(prop.name == "yp_gain")  prop.item->setValue(data.yp_gain);
        if(prop.name == "yn_offset")  prop.item->setValue(data.yn_offset);
        if(prop.name == "yn_gain")  prop.item->setValue(data.yn_gain);
        if(prop.name == "ts_offset")  prop.item->setValue(data.ts_offset);
        if(prop.name == "in_offset_set_offset")  prop.item->setValue(data.in_offset_set_offset);
        if(prop.name == "in_offset_set_gain")  prop.item->setValue(data.in_offset_set_gain);
        if(prop.name == "in_offset_get_offset")  prop.item->setValue(data.in_offset_get_offset);
        if(prop.name == "in_offset_get_gain")  prop.item->setValue(data.in_offset_get_gain);
        if(prop.name == "in_threshold_set_offset")  prop.item->setValue(data.in_threshold_set_offset);
        if(prop.name == "in_threshold_set_gain")  prop.item->setValue(data.in_threshold_set_gain);
        if(prop.name == "in_threshold_get_offset")  prop.item->setValue(data.in_threshold_get_offset);
        if(prop.name == "in_thresholdga_get_gain")  prop.item->setValue(data.in_thresholdga_get_gain);
        if(prop.name == "adc_ref_set_offset")  prop.item->setValue(data.adc_ref_set_offset);
        if(prop.name == "adc_ref_set_gain")  prop.item->setValue(data.adc_ref_set_gain);
        if(prop.name == "adc_ref_get_offset")  prop.item->setValue(data.adc_ref_get_offset);
        if(prop.name == "adc_ref_get_gain")  prop.item->setValue(data.adc_ref_get_gain);
        if(prop.name == "pm_bias_set_offset")  prop.item->setValue(data.pm_bias_set_offset);
        if(prop.name == "pm_bias_set_gain")  prop.item->setValue(data.pm_bias_set_gain);
        if(prop.name == "pm_bias_get_offset")  prop.item->setValue(data.pm_bias_get_offset);
        if(prop.name == "pm_bias_get_gain")  prop.item->setValue(data.pm_bias_get_gain);
        if(prop.name == "readout_temp_get_offset")  prop.item->setValue(data.readout_temp_get_offset);
        if(prop.name == "readout_temp_get_gain")  prop.item->setValue(data.readout_temp_get_gain);
        if(prop.name == "ifmodule_temp_get_offset")  prop.item->setValue(data.ifmodule_temp_get_offset);
        if(prop.name == "ifmodule_temp_get_gain")  prop.item->setValue(data.ifmodule_temp_get_gain);
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
//PET_RESULT Database::set(pet_brg_calibration data)
//{
//    foreach (StructProperty_t prop, l_pet_brg_calibration)
//    {
//        //if(prop.name == "")  prop.item->setValue(data.ch_calibration);
//
//    }
//
//    return PET_SUCCESS;
//}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_discovery_info *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_discovery_info)
    {
        if(prop.name == "pid")  (*data).pid = prop.item->value().toInt();
        if(prop.name == "vid")  (*data).vid = prop.item->value().toInt();
        //if(prop.name == "uid")  (*data).uid = prop.item->value().toString();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_device_info *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_device_info)
    {
        if(prop.name == "pid")  (*data).pid = prop.item->value().toInt();
        if(prop.name == "vid")  (*data).vid = prop.item->value().toInt();
        //if(prop.name == "uid")  (*data).uid = prop.item->value().toInt();
        if(prop.name == "hw_version")  (*data).hw_version = prop.item->value().toInt();
        if(prop.name == "sw_version")  (*data).sw_version = prop.item->value().toInt();
        if(prop.name == "prot_version")  (*data).prot_version = prop.item->value().toInt();
        if(prop.name == "hw_timestamp")  (*data).hw_timestamp = prop.item->value().toInt();
        if(prop.name == "sw_timestamp")  (*data).sw_timestamp = prop.item->value().toInt();
        if(prop.name == "work_time")  (*data).work_time = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_update_data *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_update_data)
    {
        if(prop.name == "ctrl")  (*data).ctrl = prop.item->value().toInt();
        if(prop.name == "offset")  (*data).offset = prop.item->value().toInt();
        if(prop.name == "length")  (*data).length = prop.item->value().toInt();
        if(prop.name == "crc")  (*data).crc = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_wave_point *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_wave_point)
    {
        if(prop.name == "xp")  (*data).xp = prop.item->value().toInt();
        if(prop.name == "xn")  (*data).xn = prop.item->value().toInt();
        if(prop.name == "yp")  (*data).yp = prop.item->value().toInt();
        if(prop.name == "yn")  (*data).yn = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_event *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_event)
    {
        if(prop.name == "src")  (*data).src = prop.item->value().toInt();
        if(prop.name == "flags")  (*data).flags = prop.item->value().toInt();
        if(prop.name == "ts_fract")  (*data).ts_fract = prop.item->value().toInt();
        if(prop.name == "ts")  (*data).ts = prop.item->value().toInt();
        if(prop.name == "xp")  (*data).xp = prop.item->value().toInt();
        if(prop.name == "xn")  (*data).xn = prop.item->value().toInt();
        if(prop.name == "yp")  (*data).yp = prop.item->value().toInt();
        if(prop.name == "yn")  (*data).yn = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_bos_ctrl *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_bos_ctrl)
    {
        if(prop.name == "trn_ctrl")  (*data).trn_ctrl = prop.item->value().toInt();
        if(prop.name == "timeout")  (*data).timeout = prop.item->value().toInt();
        if(prop.name == "net_ctrl")  (*data).net_ctrl = prop.item->value().toInt();
        //if(prop.name == "ip_address")  (*data).ip_address = prop.item->value().toInt();
        //if(prop.name == "netmask")  (*data).netmask = prop.item->value().toInt();
        //if(prop.name == "gateway")  (*data).gateway = prop.item->value().toInt();
        //if(prop.name == "mac_addr")  (*data).mac_addr = prop.item->value().toInt();
        if(prop.name == "cmd_port")  (*data).cmd_port = prop.item->value().toInt();
        if(prop.name == "event_port")  (*data).event_port = prop.item->value().toInt();
        //if(prop.name == "event_addr")  (*data).event_addr = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_bos_state *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_bos_state)
    {
        if(prop.name == "state")  (*data).state = prop.item->value().toInt();
        if(prop.name == "flags")  (*data).flags = prop.item->value().toInt();
        if(prop.name == "sync_cycle")  (*data).sync_cycle = prop.item->value().toInt();
        if(prop.name == "utilisation")  (*data).utilisation = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_bos_ch_ctrl *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_bos_ch_ctrl)
    {
        if(prop.name == "channel")  (*data).channel = prop.item->value().toInt();
        if(prop.name == "ctrl")  (*data).ctrl = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_bos_ch_state *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_bos_ch_state)
    {
        if(prop.name == "channel")  (*data).channel = prop.item->value().toInt();
        if(prop.name == "state")  (*data).state = prop.item->value().toInt();
        if(prop.name == "flags")  (*data).flags = prop.item->value().toInt();
        if(prop.name == "sync_cycle")  (*data).sync_cycle = prop.item->value().toInt();
        if(prop.name == "utilisation")  (*data).utilisation = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_brg_ctrl *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_brg_ctrl)
    {
        if(prop.name == "brg_mode")  (*data).brg_mode = prop.item->value().toInt();
        if(prop.name == "sync_mode")  (*data).sync_mode = prop.item->value().toInt();
        if(prop.name == "sync_period")  (*data).sync_period = prop.item->value().toInt();
        if(prop.name == "log_severity")  (*data).log_severity = prop.item->value().toInt();
        if(prop.name == "tp1_sel")  (*data).tp1_sel = prop.item->value().toInt();
        if(prop.name == "tp2_sel")  (*data).tp2_sel = prop.item->value().toInt();
        if(prop.name == "rf1_sel")  (*data).rf1_sel = prop.item->value().toInt();
        if(prop.name == "rf2_sel")  (*data).rf2_sel = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_brg_state *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_brg_state)
    {
        if(prop.name == "state")  (*data).state = prop.item->value().toInt();
        if(prop.name == "flags")  (*data).flags = prop.item->value().toInt();
        if(prop.name == "sync_cycle")  (*data).sync_cycle = prop.item->value().toInt();
        if(prop.name == "utilisation")  (*data).utilisation = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_brg_ch_ctrl *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_brg_ch_ctrl)
    {
        if(prop.name == "channel")  (*data).channel = prop.item->value().toInt();
        if(prop.name == "mode")  (*data).mode = prop.item->value().toInt();
        if(prop.name == "gate_width")  (*data).gate_width = prop.item->value().toInt();
        if(prop.name == "event_src")  (*data).event_src = prop.item->value().toInt();
        if(prop.name == "pm_bias")  (*data).pm_bias = prop.item->value().toInt();
        if(prop.name == "pm_bias_deviation")  (*data).pm_bias_deviation = prop.item->value().toInt();
        if(prop.name == "readout_temp_max")  (*data).readout_temp_max = prop.item->value().toInt();
        if(prop.name == "adc_reference")  (*data).adc_reference = prop.item->value().toInt();
        if(prop.name == "in_offset")  (*data).in_offset = prop.item->value().toInt();
        if(prop.name == "in_polarity")  (*data).in_polarity = prop.item->value().toInt();
        if(prop.name == "in_scale")  (*data).in_scale = prop.item->value().toInt();
        if(prop.name == "ifmodule_temp_max")  (*data).ifmodule_temp_max = prop.item->value().toInt();
        if(prop.name == "cfd_mode")  (*data).cfd_mode = prop.item->value().toInt();
        if(prop.name == "cfd_lpf_mode")  (*data).cfd_lpf_mode = prop.item->value().toInt();
        if(prop.name == "cfd_l_threshold")  (*data).cfd_l_threshold = prop.item->value().toInt();
        if(prop.name == "cfd_d_threshold")  (*data).cfd_d_threshold = prop.item->value().toInt();
        if(prop.name == "cfd_fract")  (*data).cfd_fract = prop.item->value().toInt();
        if(prop.name == "blr_mode")  (*data).blr_mode = prop.item->value().toInt();
        if(prop.name == "blr_fc")  (*data).blr_fc = prop.item->value().toInt();
        if(prop.name == "blr_gate_width")  (*data).blr_gate_width = prop.item->value().toInt();
        if(prop.name == "blr_offset")  (*data).blr_offset = prop.item->value().toInt();
        if(prop.name == "blr_threshold")  (*data).blr_threshold = prop.item->value().toInt();
        if(prop.name == "fir_fc")  (*data).fir_fc = prop.item->value().toInt();
        if(prop.name == "qdc_mode")  (*data).qdc_mode = prop.item->value().toInt();
        if(prop.name == "qdc_predelay")  (*data).qdc_predelay = prop.item->value().toInt();
        if(prop.name == "qdc_scale")  (*data).qdc_scale = prop.item->value().toInt();
        if(prop.name == "tdc_mode")  (*data).tdc_mode = prop.item->value().toInt();
        if(prop.name == "tdc_adj")  (*data).tdc_adj = prop.item->value().toInt();
        if(prop.name == "dso_time_step")  (*data).dso_time_step = prop.item->value().toInt();
        if(prop.name == "dso_value_step")  (*data).dso_value_step = prop.item->value().toInt();
        if(prop.name == "dso_prefetch")  (*data).dso_prefetch = prop.item->value().toInt();
        if(prop.name == "in_gain")  (*data).in_gain = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_brg_ch_state *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_brg_ch_state)
    {
        if(prop.name == "channel")  (*data).channel = prop.item->value().toInt();
        if(prop.name == "state")  (*data).state = prop.item->value().toInt();
        if(prop.name == "flags")  (*data).flags = prop.item->value().toInt();
        if(prop.name == "in_offset")  (*data).in_offset = prop.item->value().toInt();
        if(prop.name == "in_threshold")  (*data).in_threshold = prop.item->value().toInt();
        if(prop.name == "adc_reference")  (*data).adc_reference = prop.item->value().toInt();
        if(prop.name == "pm_bias")  (*data).pm_bias = prop.item->value().toInt();
        if(prop.name == "readout_temp")  (*data).readout_temp = prop.item->value().toInt();
        if(prop.name == "ifmodule_temp")  (*data).ifmodule_temp = prop.item->value().toInt();
        if(prop.name == "sync_cycle")  (*data).sync_cycle = prop.item->value().toInt();
        if(prop.name == "utilisation")  (*data).utilisation = prop.item->value().toInt();
        if(prop.name == "adc_underflow")  (*data).adc_underflow = prop.item->value().toInt();
        if(prop.name == "adc_overflow")  (*data).adc_overflow = prop.item->value().toInt();
        if(prop.name == "qdc_underflow")  (*data).qdc_underflow = prop.item->value().toInt();
        if(prop.name == "qdc_overflow")  (*data).qdc_overflow = prop.item->value().toInt();
        if(prop.name == "gate_overlay")  (*data).gate_overlay = prop.item->value().toInt();
        if(prop.name == "ts_invalid")  (*data).ts_invalid = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Database::get(pet_brg_ch_calibration *data)
{
    Q_CHECK_PTR(data);
    foreach (StructProperty_t prop, l_pet_brg_ch_calibration)
    {
        if(prop.name == "channel")  (*data).channel = prop.item->value().toInt();
        if(prop.name == "flags")  (*data).flags = prop.item->value().toInt();
        if(prop.name == "timestamp")  (*data).timestamp = prop.item->value().toInt();
        if(prop.name == "xp_offset")  (*data).xp_offset = prop.item->value().toInt();
        if(prop.name == "xp_gain")  (*data).xp_gain = prop.item->value().toInt();
        if(prop.name == "xn_offset")  (*data).xn_offset = prop.item->value().toInt();
        if(prop.name == "xn_gain")  (*data).xn_gain = prop.item->value().toInt();
        if(prop.name == "yp_offset")  (*data).yp_offset = prop.item->value().toInt();
        if(prop.name == "yp_gain")  (*data).yp_gain = prop.item->value().toInt();
        if(prop.name == "yn_offset")  (*data).yn_offset = prop.item->value().toInt();
        if(prop.name == "yn_gain")  (*data).yn_gain = prop.item->value().toInt();
        if(prop.name == "ts_offset")  (*data).ts_offset = prop.item->value().toInt();
        if(prop.name == "in_offset_set_offset")  (*data).in_offset_set_offset = prop.item->value().toInt();
        if(prop.name == "in_offset_set_gain")  (*data).in_offset_set_gain = prop.item->value().toInt();
        if(prop.name == "in_offset_get_offset")  (*data).in_offset_get_offset = prop.item->value().toInt();
        if(prop.name == "in_offset_get_gain")  (*data).in_offset_get_gain = prop.item->value().toInt();
        if(prop.name == "in_threshold_set_offset")  (*data).in_threshold_set_offset = prop.item->value().toInt();
        if(prop.name == "in_threshold_set_gain")  (*data).in_threshold_set_gain = prop.item->value().toInt();
        if(prop.name == "in_threshold_get_offset")  (*data).in_threshold_get_offset = prop.item->value().toInt();
        if(prop.name == "in_thresholdga_get_gain")  (*data).in_thresholdga_get_gain = prop.item->value().toInt();
        if(prop.name == "adc_ref_set_offset")  (*data).adc_ref_set_offset = prop.item->value().toInt();
        if(prop.name == "adc_ref_set_gain")  (*data).adc_ref_set_gain = prop.item->value().toInt();
        if(prop.name == "adc_ref_get_offset")  (*data).adc_ref_get_offset = prop.item->value().toInt();
        if(prop.name == "adc_ref_get_gain")  (*data).adc_ref_get_gain = prop.item->value().toInt();
        if(prop.name == "pm_bias_set_offset")  (*data).pm_bias_set_offset = prop.item->value().toInt();
        if(prop.name == "pm_bias_set_gain")  (*data).pm_bias_set_gain = prop.item->value().toInt();
        if(prop.name == "pm_bias_get_offset")  (*data).pm_bias_get_offset = prop.item->value().toInt();
        if(prop.name == "pm_bias_get_gain")  (*data).pm_bias_get_gain = prop.item->value().toInt();
        if(prop.name == "readout_temp_get_offset")  (*data).readout_temp_get_offset = prop.item->value().toInt();
        if(prop.name == "readout_temp_get_gain")  (*data).readout_temp_get_gain = prop.item->value().toInt();
        if(prop.name == "ifmodule_temp_get_offset")  (*data).ifmodule_temp_get_offset = prop.item->value().toInt();
        if(prop.name == "ifmodule_temp_get_gain")  (*data).ifmodule_temp_get_gain = prop.item->value().toInt();
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
//PET_RESULT Database::get(pet_brg_calibration *data)
//{
//    Q_CHECK_PTR(data);
//    foreach (StructProperty_t prop, l_pet_brg_calibration)
//    {
//
//    }
//
//    return PET_SUCCESS;
//}
//--------------------------------------------------------------------------------
