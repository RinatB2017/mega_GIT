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
#include "pet_brg_debug_page.hpp"
#include "ui_pet_brg_debug_page.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
PET_brg_debug_page::PET_brg_debug_page(QWidget *parent) :
    PET_device_brg(parent),
    ui(new Ui::PET_brg_debug_page)
{
    init();
}
//--------------------------------------------------------------------------------
PET_brg_debug_page::~PET_brg_debug_page()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PET_brg_debug_page::init(void)
{
    ui->setupUi(this);

    //TODO mark
    //---
    mark_spinbox("setting_name",        "brg_debug_page");
    mark_doublespinbox("setting_name",  "brg_debug_page");
    mark_combobox("setting_name",       "brg_debug_page");
    //---

    ui->sb_sync_period->setRange(0, UINT16_MAX);

    ui->cb_sync_mode->clear();
    ui->cb_sync_mode->addItem("Отключено",   PET_BRG_SYNC_MODE_DISABLE);
    ui->cb_sync_mode->addItem("Внутренняя синхронизация",   PET_BRG_SYNC_MODE_INT);
    ui->cb_sync_mode->addItem("Внешняя синхронизация",      PET_BRG_SYNC_MODE_EXT);
    ui->cb_sync_mode->addItem("Внешняя синхронизация (с контролем)",   PET_BRG_SYNC_MODE_EXT_CHECK);

    ui->cb_log_severity->clear();

    ui->cb_tp1_sel->clear();
    ui->cb_tp1_sel->addItem("КТ отключена (не используется)", PET_BRG_TP_OFF);
    ui->cb_tp1_sel->addItem("Тактовая частота опорного генератора", PET_BRG_TP_REF_CLOCK);
    ui->cb_tp1_sel->addItem("Тактовая частота синхронизации", PET_BRG_TP_SYNC_CLOCK);
    ui->cb_tp1_sel->addItem("Тактовая частота модуля DPP", PET_BRG_TP_DPP_CLOCK);
    ui->cb_tp1_sel->addItem("Тактовая частота модуля TCom", PET_BRG_TP_TCOM_CLOCK);
    ui->cb_tp1_sel->addItem("Тактовая частота процессора", PET_BRG_TP_CPU_CLOCK);
    ui->cb_tp1_sel->addItem("Сигнал \"триггера\" канала 0", PET_BRG_TP_CH0_TRG);
    ui->cb_tp1_sel->addItem("Сигнал \"триггера\" канала 1", PET_BRG_TP_CH1_TRG);
    ui->cb_tp1_sel->addItem("Сигнал \"триггера\" канала 2", PET_BRG_TP_CH2_TRG);
    ui->cb_tp1_sel->addItem("Сигнал \"триггера\" канала 3", PET_BRG_TP_CH3_TRG);

    ui->cb_tp2_sel->clear();
    ui->cb_tp2_sel->addItem("КТ отключена (не используется)", PET_BRG_TP_OFF);
    ui->cb_tp2_sel->addItem("Тактовая частота опорного генератора", PET_BRG_TP_REF_CLOCK);
    ui->cb_tp2_sel->addItem("Тактовая частота синхронизации", PET_BRG_TP_SYNC_CLOCK);
    ui->cb_tp2_sel->addItem("Тактовая частота модуля DPP", PET_BRG_TP_DPP_CLOCK);
    ui->cb_tp2_sel->addItem("Тактовая частота модуля TCom", PET_BRG_TP_TCOM_CLOCK);
    ui->cb_tp2_sel->addItem("Тактовая частота процессора", PET_BRG_TP_CPU_CLOCK);
    ui->cb_tp2_sel->addItem("Сигнал \"триггера\" канала 0", PET_BRG_TP_CH0_TRG);
    ui->cb_tp2_sel->addItem("Сигнал \"триггера\" канала 1", PET_BRG_TP_CH1_TRG);
    ui->cb_tp2_sel->addItem("Сигнал \"триггера\" канала 2", PET_BRG_TP_CH2_TRG);
    ui->cb_tp2_sel->addItem("Сигнал \"триггера\" канала 3", PET_BRG_TP_CH3_TRG);

    ui->cb_rf1_sel->clear();
    ui->cb_rf1_sel->addItem("КТ отключена (не используется)", PET_BRG_TP_OFF);
    ui->cb_rf1_sel->addItem("Тактовая частота опорного генератора", PET_BRG_TP_REF_CLOCK);
    ui->cb_rf1_sel->addItem("Тактовая частота синхронизации", PET_BRG_TP_SYNC_CLOCK);
    ui->cb_rf1_sel->addItem("Тактовая частота модуля DPP", PET_BRG_TP_DPP_CLOCK);
    ui->cb_rf1_sel->addItem("Тактовая частота модуля TCom", PET_BRG_TP_TCOM_CLOCK);
    ui->cb_rf1_sel->addItem("Тактовая частота процессора", PET_BRG_TP_CPU_CLOCK);
    ui->cb_rf1_sel->addItem("Сигнал \"триггера\" канала 0", PET_BRG_TP_CH0_TRG);
    ui->cb_rf1_sel->addItem("Сигнал \"триггера\" канала 1", PET_BRG_TP_CH1_TRG);
    ui->cb_rf1_sel->addItem("Сигнал \"триггера\" канала 2", PET_BRG_TP_CH2_TRG);
    ui->cb_rf1_sel->addItem("Сигнал \"триггера\" канала 3", PET_BRG_TP_CH3_TRG);

    ui->cb_rf2_sel->clear();
    ui->cb_rf2_sel->addItem("КТ отключена (не используется)", PET_BRG_TP_OFF);
    ui->cb_rf2_sel->addItem("Тактовая частота опорного генератора", PET_BRG_TP_REF_CLOCK);
    ui->cb_rf2_sel->addItem("Тактовая частота синхронизации", PET_BRG_TP_SYNC_CLOCK);
    ui->cb_rf2_sel->addItem("Тактовая частота модуля DPP", PET_BRG_TP_DPP_CLOCK);
    ui->cb_rf2_sel->addItem("Тактовая частота модуля TCom", PET_BRG_TP_TCOM_CLOCK);
    ui->cb_rf2_sel->addItem("Тактовая частота процессора", PET_BRG_TP_CPU_CLOCK);
    ui->cb_rf2_sel->addItem("Сигнал \"триггера\" канала 0", PET_BRG_TP_CH0_TRG);
    ui->cb_rf2_sel->addItem("Сигнал \"триггера\" канала 1", PET_BRG_TP_CH1_TRG);
    ui->cb_rf2_sel->addItem("Сигнал \"триггера\" канала 2", PET_BRG_TP_CH2_TRG);
    ui->cb_rf2_sel->addItem("Сигнал \"триггера\" канала 3", PET_BRG_TP_CH3_TRG);

    connect(ui->btn_save_to_EEPROM, SIGNAL(clicked(bool)),  this,   SLOT(save_to_EEPROM()));
    connect(ui->btn_set_default,    SIGNAL(clicked(bool)),  this,   SLOT(set_default()));
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_debug_page::set(pet_brg_ctrl_t p_data)
{
    int i_sync_mode = ui->cb_sync_mode->findData(p_data.sync_mode);
    int i_log_severity = ui->cb_log_severity->findData(p_data.log_severity);

    int i_tp1_sel = ui->cb_tp1_sel->findData(p_data.tp1_sel);
    int i_tp2_sel = ui->cb_tp2_sel->findData(p_data.tp2_sel);
    int i_rf1_sel = ui->cb_rf1_sel->findData(p_data.rf1_sel);
    int i_rf2_sel = ui->cb_rf2_sel->findData(p_data.rf2_sel);

    if(i_sync_mode != -1)       ui->cb_sync_mode->setCurrentIndex(i_sync_mode);
    if(i_log_severity != -1)    ui->cb_log_severity->setCurrentIndex(i_log_severity);
    if(i_tp1_sel != -1)         ui->cb_tp1_sel->setCurrentIndex(i_tp1_sel);
    if(i_tp2_sel != -1)         ui->cb_tp2_sel->setCurrentIndex(i_tp2_sel);
    if(i_rf1_sel != -1)         ui->cb_rf1_sel->setCurrentIndex(i_rf1_sel);
    if(i_rf2_sel != -1)         ui->cb_rf2_sel->setCurrentIndex(i_rf2_sel);

    ui->sb_sync_period->setValue(p_data.sync_period);

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_brg_debug_page::get(pet_brg_ctrl_t *p_data)
{
    Q_CHECK_PTR(p_data);
    //---
    bool ok = false;
    (*p_data).sync_mode = ui->cb_sync_mode->itemData(ui->cb_sync_mode->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).sync_mode");
    (*p_data).sync_period = ui->sb_sync_period->value();
    (*p_data).log_severity = ui->cb_log_severity->itemData(ui->cb_log_severity->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).log_severity");
    (*p_data).tp1_sel = ui->cb_tp1_sel->itemData(ui->cb_tp1_sel->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).tp1_sel");
    (*p_data).tp2_sel = ui->cb_tp2_sel->itemData(ui->cb_tp1_sel->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).tp2_sel");
    (*p_data).rf1_sel = ui->cb_rf1_sel->itemData(ui->cb_rf1_sel->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).rf1_sel");
    (*p_data).rf2_sel = ui->cb_rf1_sel->itemData(ui->cb_rf2_sel->currentIndex()).toInt(&ok);
    if(!ok) emit error("(*p_data).rf2_sel");

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_brg_debug_page::block_ctrl_interface(bool state)
{
    ui->sb_sync_period->setDisabled(state);
    ui->cb_sync_mode->setDisabled(state);
    ui->cb_log_severity->setDisabled(state);
    ui->cb_tp1_sel->setDisabled(state);
    ui->cb_tp2_sel->setDisabled(state);
    ui->cb_rf1_sel->setDisabled(state);
    ui->cb_rf2_sel->setDisabled(state);
}
//--------------------------------------------------------------------------------
void PET_brg_debug_page::block_state_interface(bool state)
{
    Q_UNUSED(state)
}
//--------------------------------------------------------------------------------
void PET_brg_debug_page::save_to_EEPROM(void)
{
    PET_device_brg::save_to_EEPROM();
}
//--------------------------------------------------------------------------------
void PET_brg_debug_page::set_default(void)
{
    PET_device_brg::set_default();
}
//--------------------------------------------------------------------------------
