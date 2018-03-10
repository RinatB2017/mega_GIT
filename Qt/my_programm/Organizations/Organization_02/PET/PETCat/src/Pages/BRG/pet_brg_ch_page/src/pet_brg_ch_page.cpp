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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "pet_brg_ch_page.hpp"
#include "ui_pet_brg_ch_page.h"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
PET_brg_ch_page::PET_brg_ch_page(uint16_t channel, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::PET_brg_ch_page),
    channel(channel)
{
    init();
}
//--------------------------------------------------------------------------------
PET_brg_ch_page::~PET_brg_ch_page()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PET_brg_ch_page::init(void)
{
    ui->setupUi(this);

    //TODO mark
    //---
    mark_spinbox("setting_name",        QString("brg_ch_page_%1").arg(channel));
    mark_doublespinbox("setting_name",  QString("brg_ch_page_%1").arg(channel));
    mark_combobox("setting_name",       QString("brg_ch_page_%1").arg(channel));
    //---

    ui->sb_gate_width->setRange(0, UINT16_MAX);
    ui->sb_event_src->setRange(0, UINT8_MAX);
    ui->dsb_pm_bias->setRange(INT16_MIN, INT16_MAX);
    ui->sb_pm_bias_deviation->setRange(0, UINT16_MAX);
    ui->dsb_readout_temp_max->setRange(INT16_MIN, INT16_MAX);
    ui->dsb_adc_reference->setRange(INT16_MIN, INT16_MAX);
    ui->dsb_in_offset->setRange(INT16_MIN, INT16_MAX);

    //ui->dsb_in_underflow_thr->setRange(INT16_MIN, INT16_MAX);
    //ui->dsb_in_overflow_thr->setRange(INT16_MIN, INT16_MAX);

    ui->dsb_ifmodule_temp_max->setRange(INT16_MIN, INT16_MAX);

    ui->dsb_cfd_l_threshold->setRange(INT16_MIN, INT16_MAX);
    ui->dsb_cfd_d_threshold->setRange(INT16_MIN, INT16_MAX);

    ui->sb_blr_gate_width->setRange(0, UINT16_MAX);
    ui->dsb_blr_offset->setRange(INT16_MIN, INT16_MAX);
    ui->dsb_blr_threshold->setRange(INT16_MIN, INT16_MAX);
    ui->sb_qdc_predelay->setRange(0, UINT16_MAX);
    ui->dsb_qdc_scale->setRange(INT16_MIN, INT16_MAX);
    ui->sb_tdc_adj->setRange(INT16_MIN, INT16_MAX);
    ui->sb_dso_time_step->setRange(0, UINT16_MAX);
    ui->dsb_dso_value_step->setRange(0, UINT16_MAX);
    ui->sb_dso_prefetch->setRange(0, UINT16_MAX);
    ui->dsb_in_gain->setRange(0, UINT16_MAX);

    ui->cb_mode->clear();
    ui->cb_mode->addItem("Отключен",            PET_BRG_CH_MODE_DISABLE);
    ui->cb_mode->addItem("Ожидание",            PET_BRG_CH_MODE_IDLE);
    ui->cb_mode->addItem("Регистрация событий", PET_BRG_CH_MODE_SCAN);

    ui->cb_in_polarity->clear();
    ui->cb_in_polarity->addItem("Прямая",       PET_BRG_CH_IN_POLARITY_NORMAL);
    ui->cb_in_polarity->addItem("Инверсная",    PET_BRG_CH_IN_POLARITY_INVERSE);

    ui->cb_in_scale->clear();
    ui->cb_in_scale->addItem("x1",  PET_BRG_CH_IN_SCALE_x1);
    ui->cb_in_scale->addItem("x2",  PET_BRG_CH_IN_SCALE_x2);
    ui->cb_in_scale->addItem("x4",  PET_BRG_CH_IN_SCALE_x4);
    ui->cb_in_scale->addItem("x8",  PET_BRG_CH_IN_SCALE_x8);
    ui->cb_in_scale->addItem("x16", PET_BRG_CH_IN_SCALE_x16);
    ui->cb_in_scale->addItem("x32", PET_BRG_CH_IN_SCALE_x32);

    ui->cb_fir_mode->clear();
    ui->cb_fir_mode->addItem("Отключено", PET_FIR_MODE_DISABLE);

    ui->cb_fir_fc->clear();
    ui->cb_fir_fc->addItem("PET_FIR_FC_none",   PET_FIR_FC_none);

    ui->cb_qdc_mode->clear();
    ui->cb_qdc_mode->addItem("Отключено",       PET_BRG_QDC_MODE_DISABLE);
    ui->cb_qdc_mode->addItem("По максимумам",   PET_BRG_QDC_MODE_PEAK);
    ui->cb_qdc_mode->addItem("Суммирование",    PET_BRG_QDC_MODE_SUM);

    ui->cb_cfd_mode->clear();
    ui->cb_cfd_mode->addItem("Отключено",       PET_CFD_MODE_DISABLE);
    ui->cb_cfd_mode->addItem("Аналоговый CFD",  PET_CFD_MODE_EXTERN);
    ui->cb_cfd_mode->addItem("Цифровой CFD",    PET_CFD_MODE_DIGITAL);

    ui->cb_cfd_lpf_mode->clear();
    ui->cb_cfd_lpf_mode->addItem("Отключено",   PET_CFD_LPF_MODE_DISABLE);
    ui->cb_cfd_lpf_mode->addItem("Включено",    PET_CFD_LPF_MODE_ENABLE);

    ui->cb_tdc_mode->clear();
    ui->cb_tdc_mode->addItem("Отключено",       PET_BRG_TDC_MODE_DISABLE);
    ui->cb_tdc_mode->addItem("По данным CFD",   PET_BRG_TDC_MODE_CFD);

    ui->cb_cfd_fract->clear();
    ui->cb_cfd_fract->addItem("1/1",   PET_BRG_CFD_FRACT_DIV_1);
    ui->cb_cfd_fract->addItem("1/2",   PET_BRG_CFD_FRACT_DIV_2);
    ui->cb_cfd_fract->addItem("1/4",   PET_BRG_CFD_FRACT_DIV_4);
    ui->cb_cfd_fract->addItem("1/8",   PET_BRG_CFD_FRACT_DIV_8);

    ui->cb_blr_mode->clear();
    ui->cb_blr_mode->addItem("Отключено",                   PET_BLR_MODE_DISABLE);
    ui->cb_blr_mode->addItem("Постоянное смещение",         PET_BLR_MODE_CONST);
    ui->cb_blr_mode->addItem("Динамическая компенсация",    PET_BLR_MODE_DYNAMIC);

    ui->cb_blr_fc->clear();
    ui->cb_blr_fc->addItem("20 кГц",    PET_BLR_FC_20kHz);
    ui->cb_blr_fc->addItem("160 кГц",   PET_BLR_FC_160kHz);
    ui->cb_blr_fc->addItem("640 кГц",   PET_BLR_FC_640kHz);
    ui->cb_blr_fc->addItem("6600 кГц",  PET_BLR_FC_6600kHz);
    //---
    ui->dsb_adc_reference->setDecimals(1);
    ui->dsb_in_offset->setDecimals(1);
    //ui->dsb_in_underflow_thr->setDecimals(1);
    //ui->dsb_in_overflow_thr->setDecimals(1);
    ui->dsb_cfd_l_threshold->setDecimals(1);
    ui->dsb_cfd_d_threshold->setDecimals(1);
    ui->dsb_blr_offset->setDecimals(1);
    ui->dsb_blr_threshold->setDecimals(1);
    ui->dsb_dso_value_step->setDecimals(1);
    ui->dsb_pm_bias->setDecimals(3);
    ui->dsb_in_gain->setDecimals(3);
    //---
    ui->dsb_readout_temp_max->setDecimals(2);
    ui->dsb_ifmodule_temp_max->setDecimals(2);
    //--
    ui->le_state->setReadOnly(true);
    ui->le_readout_temp->setReadOnly(true);
    ui->le_ifmodule_temp->setReadOnly(true);
    ui->le_sync_cycle->setReadOnly(true);
    ui->le_utilisation->setReadOnly(true);
    ui->le_adc_underflow->setReadOnly(true);
    ui->le_adc_overflow->setReadOnly(true);
    ui->le_qdc_underflow->setReadOnly(true);
    ui->le_qdc_overflow->setReadOnly(true);
    ui->le_gate_overlay->setReadOnly(true);
    ui->le_ts_invalid->setReadOnly(true);
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_ch_page::set_ctrl(pet_brg_ch_ctrl_t p_data)
{
    int i_mode = ui->cb_mode->findData(p_data.mode);
    int i_in_polarity = ui->cb_in_polarity->findData(p_data.in_polarity);
    int i_in_scale = ui->cb_in_scale->findData(p_data.in_scale);
    int i_fir_mode = ui->cb_fir_mode->findData(p_data.fir_mode);
    int i_fir_fc = ui->cb_fir_fc->findData(p_data.fir_fc);
    int i_cfd_mode = ui->cb_cfd_mode->findData(p_data.cfd_mode);
    int i_cfd_lpf_mode = ui->cb_cfd_lpf_mode->findData(p_data.cfd_lpf_mode);
    int i_cfd_fract = ui->cb_cfd_fract->findData(p_data.cfd_fract);
    int i_tdc_mode = ui->cb_tdc_mode->findData(p_data.tdc_mode);
    int i_blr_mode = ui->cb_blr_mode->findData(p_data.blr_mode);
    int i_blr_fc = ui->cb_blr_fc->findData(p_data.blr_fc);
    int i_qdc_mode = ui->cb_qdc_mode->findData(p_data.qdc_mode);

    if(i_mode != -1)        ui->cb_mode->setCurrentIndex(i_mode);                       else emit error(QString("set_ctrl: i_mode %1").arg(p_data.mode));
    if(i_in_polarity != -1) ui->cb_in_polarity->setCurrentIndex(i_in_polarity);         else emit error(QString("set_ctrl: i_in_polarity %1").arg(p_data.in_polarity));
    if(i_in_scale != -1)    ui->cb_in_scale->setCurrentIndex(i_in_scale);               else emit error(QString("set_ctrl: i_in_scale %1").arg(p_data.in_scale));
    if(i_fir_mode != -1)    ui->cb_fir_mode->setCurrentIndex(i_fir_mode);               else emit error(QString("set_ctrl: i_fir_mode %1").arg(p_data.fir_mode));
    if(i_fir_fc != -1)      ui->cb_fir_fc->setCurrentIndex(i_fir_fc);                   else emit error(QString("set_ctrl: i_fir_fc %1").arg(p_data.fir_fc));
    if(i_cfd_mode != -1)    ui->cb_cfd_mode->setCurrentIndex(i_cfd_mode);               else emit error(QString("set_ctrl: i_cfd_mode %1").arg(p_data.cfd_mode));
    if(i_cfd_lpf_mode != -1)    ui->cb_cfd_lpf_mode->setCurrentIndex(i_cfd_lpf_mode);   else emit error(QString("set_ctrl: i_cfd_lpf_mode %1").arg(p_data.cfd_lpf_mode));
    if(i_cfd_fract != -1)   ui->cb_cfd_fract->setCurrentIndex(i_cfd_fract);             else emit error(QString("set_ctrl: i_cfd_fract %1").arg(p_data.cfd_fract));
    if(i_tdc_mode != -1)    ui->cb_tdc_mode->setCurrentIndex(i_tdc_mode);               else emit error(QString("set_ctrl: i_tdc_mode %1").arg(p_data.tdc_mode));
    if(i_blr_mode != -1)    ui->cb_blr_mode->setCurrentIndex(i_blr_mode);               else emit error(QString("set_ctrl: i_blr_mode %1").arg(p_data.blr_mode));
    if(i_blr_fc != -1)      ui->cb_blr_fc->setCurrentIndex(i_blr_fc);                   else emit error(QString("set_ctrl: i_blr_fc %1").arg(p_data.blr_fc));
    if(i_qdc_mode != -1)    ui->cb_qdc_mode->setCurrentIndex(i_qdc_mode);               else emit error(QString("set_ctrl: i_qdc_mode %1").arg(p_data.qdc_mode));

    ui->sb_gate_width->setValue(p_data.gate_width);
    ui->sb_event_src->setValue(p_data.event_src);
    ui->dsb_pm_bias->setValue(p_data.pm_bias * 0.001);
    ui->sb_pm_bias_deviation->setValue(p_data.pm_bias_deviation);
    ui->dsb_readout_temp_max->setValue(p_data.readout_temp_max * 0.01);
    ui->dsb_adc_reference->setValue(p_data.adc_reference * 0.1);
    ui->dsb_in_offset->setValue(p_data.in_offset * 0.1);
    ui->dsb_ifmodule_temp_max->setValue(p_data.ifmodule_temp_max * 0.01);

    ui->dsb_cfd_l_threshold->setValue(p_data.cfd_l_threshold * 0.1);
    ui->dsb_cfd_d_threshold->setValue(p_data.cfd_d_threshold * 0.1);

    ui->sb_blr_gate_width->setValue(p_data.blr_gate_width);
    ui->dsb_blr_offset->setValue(p_data.blr_offset * 0.1);
    ui->dsb_blr_threshold->setValue(p_data.blr_threshold * 0.1);
    ui->sb_qdc_predelay->setValue(p_data.qdc_predelay);
    ui->dsb_qdc_scale->setValue(p_data.qdc_scale * qPow(2, -8));
    ui->sb_tdc_adj->setValue(p_data.tdc_adj);
    ui->sb_dso_time_step->setValue(p_data.dso_time_step);
    ui->dsb_dso_value_step->setValue(p_data.dso_value_step * 0.1);
    ui->sb_dso_prefetch->setValue(p_data.dso_prefetch);

    ui->dsb_in_gain->setValue(p_data.in_gain * 0.001);

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_ch_page::set_state(pet_brg_ch_state_t p_data)
{
    switch(p_data.state)
    {
    case PET_BRG_CH_STATE_DISABLE:      ui->le_state->setText("Отключен");  break;
    case PET_BRG_CH_STATE_IDLE:         ui->le_state->setText("Ожидание");  break;
    case PET_BRG_CH_STATE_SCAN:         ui->le_state->setText("Регистрация событий");  break;
    case PET_BRG_CH_STATE_NOT_IMPLIMENT:      ui->le_state->setText("Канал обработки не реализован");  break;
    case PET_BRG_CH_STATE_IFMODULE_MISSING:   ui->le_state->setText("Модуль сопряжения не установлен");  break;
    case PET_BRG_CH_STATE_FAULT:              ui->le_state->setText("Неисправность");  break;
    default:
        ui->le_state->setText(QString("0x%1").arg(p_data.state, 2, 16, QChar('0')));
        break;
    }
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_READUOT_TEMP_MSK)
        ui->le_readout_temp->setText(QString("%1").arg(p_data.readout_temp * 0.01, 0, 'f', 2));
    else
        ui->le_readout_temp->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_IFMODULE_TEMP_MSK)
        ui->le_ifmodule_temp->setText(QString("%1").arg(p_data.ifmodule_temp * 0.01, 0, 'f', 2));
    else
        ui->le_ifmodule_temp->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_SYNC_CYCLE_MSK)
        ui->le_sync_cycle->setText(QString("%1").arg(p_data.sync_cycle));
    else
        ui->le_sync_cycle->setText("н/д");

    //---
#if 0
    ххх (yyy.yy%)

    le_utilisation

    le_adc_underflow
    le_adc_overflow
    le_qdc_underflow
    le_qdc_overflow
    le_gate_overlay
    le_ts_invalid
#endif
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_UTILISATION_MSK)
        ui->le_utilisation->setText(QString("%1").arg(p_data.utilisation));
    else
        ui->le_utilisation->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_ADC_UNDERFLOW_MSK)
    {
        QString text;
        float val1 = (float)p_data.adc_underflow;
        float val2 = (float)p_data.utilisation;
        float percent = 0;
        if(val2 != 0) percent = val1 / val2 * (float)100;
        if(percent < 0.01) text = QString("%1 (<0.01 %)").arg(val1);
        else text = QString("%1 (%2%)")
                .arg(val1)
                .arg(percent, 0, 'f', 2);
        ui->le_adc_underflow->setText(text);
    }
    else
        ui->le_adc_underflow->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_ADC_OVERFLOW_MSK)
    {
        QString text;
        float val1 = (float)p_data.adc_overflow;
        float val2 = (float)p_data.utilisation;
        float percent = 0;
        if(val2 != 0) percent = val1 / val2 * (float)100;
        if(percent < 0.01) text = QString("%1 (<0.01%)").arg(val1);
        else text = QString("%1 (%2%)")
                .arg(val1)
                .arg(percent, 0, 'f', 2);
        ui->le_adc_overflow->setText(text);
    }
    else
        ui->le_adc_overflow->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_QDC_UNDERFLOW_MSK)
    {
        QString text;
        float val1 = (float)p_data.qdc_underflow;
        float val2 = (float)p_data.utilisation;
        float percent = 0;
        if(val2 != 0) percent = val1 / val2 * (float)100;
        if(percent < 0.01) text = QString("%1 (<0.01%)").arg(val1);
        else text = QString("%1 (%2%)")
                .arg(val1)
                .arg(percent, 0, 'f', 2);
        ui->le_qdc_underflow->setText(text);
    }
    else
        ui->le_qdc_underflow->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_QDC_OVERFLOW_MSK)
    {
        QString text;
        float val1 = (float)p_data.qdc_overflow;
        float val2 = (float)p_data.utilisation;
        float percent = 0;
        if(val2 != 0) percent = val1 / val2 * (float)100;
        if(percent < 0.01) text = QString("%1 (<0.01%)").arg(val1);
        else text = QString("%1 (%2%)")
                .arg(val1)
                .arg(percent, 0, 'f', 2);
        ui->le_qdc_overflow->setText(text);
    }
    else
        ui->le_qdc_overflow->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_GATE_OVERLAY_MSK)
    {
        QString text;
        float val1 = (float)p_data.gate_overlay;
        float val2 = (float)p_data.utilisation;
        float percent = 0;
        if(val2 != 0) percent = val1 / val2 * (float)100;
        if(percent < 0.01) text = QString("%1 (<0.01%)").arg(val1);
        else text = QString("%1 (%2%)")
                .arg(val1)
                .arg(percent, 0, 'f', 2);
        ui->le_gate_overlay->setText(text);
    }
    else
        ui->le_gate_overlay->setText("н/д");
    //---
    if(p_data.flags & PET_BRG_CH_STATE_FLAG_TS_INVALID_MSK)
    {
        QString text;
        float val1 = (float)p_data.ts_invalid;
        float val2 = (float)p_data.utilisation;
        float percent = 0;
        if(val2 != 0) percent = val1 / val2 * (float)100;
        if(percent < 0.01) text = QString("%1 (<0.01%)").arg(val1);
        else text = QString("%1 (%2%)")
                .arg(val1)
                .arg(percent, 0, 'f', 2);
        ui->le_ts_invalid->setText(text);
    }
    else
        ui->le_ts_invalid->setText("н/д");
    //---

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_ch_page::get_ctrl(pet_brg_ch_ctrl_t *p_data)
{
    Q_CHECK_PTR(p_data);
    //---
    bool ok = false;

    (*p_data).channel = channel;
    (*p_data).mode = ui->cb_mode->itemData(ui->cb_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).mode");
    (*p_data).gate_width = ui->sb_gate_width->value();
    (*p_data).event_src = ui->sb_event_src->value();
    (*p_data).pm_bias = ui->dsb_pm_bias->value() / 0.001 + 0.5;
    (*p_data).pm_bias_deviation = ui->sb_pm_bias_deviation->value();
    (*p_data).readout_temp_max = ui->dsb_readout_temp_max->value() / 0.01 + 0.5;
    (*p_data).adc_reference = ui->dsb_adc_reference->value() / 0.1 + 0.5;
    (*p_data).in_offset = ui->dsb_in_offset->value() / 0.1 + 0.5;
    (*p_data).in_polarity = ui->cb_in_polarity->itemData(ui->cb_in_polarity->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).in_polarity");
    (*p_data).in_scale = ui->cb_in_scale->itemData(ui->cb_in_scale->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).in_scale");
    (*p_data).ifmodule_temp_max = ui->dsb_ifmodule_temp_max->value() / 0.01 + 0.5;
    (*p_data).cfd_mode = ui->cb_cfd_mode->itemData(ui->cb_cfd_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).cfd_mode");

    (*p_data).cfd_fract = ui->cb_cfd_fract->itemData(ui->cb_cfd_fract->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).cb_cfd_fract");

    (*p_data).cfd_lpf_mode = ui->cb_cfd_lpf_mode->itemData(ui->cb_cfd_lpf_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).cfd_lpf_mode");

    (*p_data).qdc_mode = ui->cb_qdc_mode->itemData(ui->cb_qdc_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).qdc_mode");

    (*p_data).cfd_l_threshold = ui->dsb_cfd_l_threshold->value() / 0.1 + 0.5;
    (*p_data).cfd_d_threshold = ui->dsb_cfd_d_threshold->value() / 0.1 + 0.5;

    (*p_data).blr_mode = ui->cb_blr_mode->itemData(ui->cb_blr_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).blr_mode");
    (*p_data).blr_fc = ui->cb_blr_fc->itemData(ui->cb_blr_fc->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).blr_fc");
    (*p_data).blr_gate_width = ui->sb_blr_gate_width->value();
    (*p_data).blr_offset = ui->dsb_blr_offset->value() / 0.1 + 0.5;
    (*p_data).blr_threshold = ui->dsb_blr_threshold->value() / 0.1 + 0.5;
    (*p_data).fir_mode = ui->cb_fir_mode->itemData(ui->cb_fir_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).fir_mode");
    (*p_data).fir_fc = ui->cb_fir_fc->itemData(ui->cb_fir_fc->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).fir_fc");
    (*p_data).qdc_predelay = ui->sb_qdc_predelay->value();
    (*p_data).qdc_scale = ui->dsb_qdc_scale->value() / qPow(2, -8);
    (*p_data).tdc_mode = ui->cb_tdc_mode->itemData(ui->cb_tdc_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).tdc_mode");
    (*p_data).tdc_adj = ui->sb_tdc_adj->value();
    (*p_data).dso_time_step = ui->sb_dso_time_step->value();
    (*p_data).dso_value_step = ui->dsb_dso_value_step->value() / 0.1 + 0.5;
    (*p_data).dso_prefetch = ui->sb_dso_prefetch->value();

    (*p_data).in_gain = ui->dsb_in_gain->value() / 0.001 + 0.5;

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_brg_ch_page::block_ctrl_interface(bool state)
{
    ui->sb_gate_width->setDisabled(state);
    ui->sb_event_src->setDisabled(state);
    ui->dsb_pm_bias->setDisabled(state);
    ui->sb_pm_bias_deviation->setDisabled(state);
    ui->dsb_readout_temp_max->setDisabled(state);
    ui->dsb_adc_reference->setDisabled(state);
    ui->dsb_in_offset->setDisabled(state);
    //ui->dsb_in_underflow_thr->setDisabled(state);
    //ui->dsb_in_overflow_thr->setDisabled(state);
    ui->dsb_ifmodule_temp_max->setDisabled(state);
    ui->dsb_cfd_l_threshold->setDisabled(state);
    ui->dsb_cfd_d_threshold->setDisabled(state);
    ui->sb_blr_gate_width->setDisabled(state);
    ui->dsb_blr_offset->setDisabled(state);
    ui->dsb_blr_threshold->setDisabled(state);
    ui->sb_qdc_predelay->setDisabled(state);
    ui->dsb_qdc_scale->setDisabled(state);
    ui->sb_tdc_adj->setDisabled(state);
    ui->sb_dso_time_step->setDisabled(state);
    ui->dsb_dso_value_step->setDisabled(state);
    ui->sb_dso_prefetch->setDisabled(state);
    ui->cb_mode->setDisabled(state);
    ui->cb_in_polarity->setDisabled(state);
    ui->cb_in_scale->setDisabled(state);
    ui->cb_fir_mode->setDisabled(state);
    ui->cb_fir_fc->setDisabled(state);
    ui->cb_qdc_mode->setDisabled(state);
    ui->cb_cfd_mode->setDisabled(state);
    ui->cb_cfd_lpf_mode->setDisabled(state);
    ui->cb_tdc_mode->setDisabled(state);
    ui->cb_blr_mode->setDisabled(state);
    ui->cb_blr_fc->setDisabled(state);
}
//--------------------------------------------------------------------------------
void PET_brg_ch_page::block_state_interface(bool state)
{
    ui->le_state->setDisabled(state);
    ui->le_readout_temp->setDisabled(state);
    ui->le_ifmodule_temp->setDisabled(state);
    ui->le_sync_cycle->setDisabled(state);
    ui->le_utilisation->setDisabled(state);
    ui->le_adc_underflow->setDisabled(state);
    ui->le_adc_overflow->setDisabled(state);
    ui->le_qdc_underflow->setDisabled(state);
    ui->le_qdc_overflow->setDisabled(state);
    ui->le_gate_overlay->setDisabled(state);
    ui->le_ts_invalid->setDisabled(state);
}
//--------------------------------------------------------------------------------
