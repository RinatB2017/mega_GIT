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
#include "pet_event_flag.hpp"
#include "ui_pet_event_flag.h"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
PET_event_flag::PET_event_flag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PET_event_flag)
{
    ui->setupUi(this);
    setFixedSize(sizeHint());

    ui->cb_PET_EVENT_FLAG_FAULT->setChecked(true);
    ui->cb_PET_EVENT_FLAG_TS_INVALID->setChecked(true);
    ui->cb_PET_EVENT_FLAG_GATE_OVERLAY->setChecked(true);
    ui->cb_PET_EVENT_FLAG_QDC_OVERFLOW->setChecked(true);
    ui->cb_PET_EVENT_FLAG_QDC_UNDERFLOW->setChecked(true);
    ui->cb_PET_EVENT_FLAG_ADC_OVERFLOW->setChecked(true);
    ui->cb_PET_EVENT_FLAG_ADC_UNDERFLOW->setChecked(true);
    ui->cb_PET_EVENT_FLAG_SYNC->setChecked(false);

    setWindowIcon(QIcon(ICON_PROGRAMM));
}
//--------------------------------------------------------------------------------
PET_event_flag::~PET_event_flag()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PET_event_flag::set(uint8_t value)
{
    ui->cb_PET_EVENT_FLAG_FAULT->setChecked(value & PET_EVENT_FLAG_FAULT);
    ui->cb_PET_EVENT_FLAG_TS_INVALID->setChecked(value & PET_EVENT_FLAG_TS_INVALID);
    ui->cb_PET_EVENT_FLAG_GATE_OVERLAY->setChecked(value & PET_EVENT_FLAG_GATE_OVERLAY);
    ui->cb_PET_EVENT_FLAG_QDC_OVERFLOW->setChecked(value & PET_EVENT_FLAG_QDC_OVERFLOW);
    ui->cb_PET_EVENT_FLAG_QDC_UNDERFLOW->setChecked(value & PET_EVENT_FLAG_QDC_UNDERFLOW);
    ui->cb_PET_EVENT_FLAG_ADC_OVERFLOW->setChecked(value & PET_EVENT_FLAG_ADC_OVERFLOW);
    ui->cb_PET_EVENT_FLAG_ADC_UNDERFLOW->setChecked(value & PET_EVENT_FLAG_ADC_UNDERFLOW);
    ui->cb_PET_EVENT_FLAG_SYNC->setChecked(value & PET_EVENT_FLAG_SYNC);
}
//--------------------------------------------------------------------------------
uint8_t PET_event_flag::get(void)
{
    uint8_t value = 0;

    if(ui->cb_PET_EVENT_FLAG_FAULT->isChecked())            value |= PET_EVENT_FLAG_FAULT;
    if(ui->cb_PET_EVENT_FLAG_TS_INVALID->isChecked())       value |= PET_EVENT_FLAG_TS_INVALID;
    if(ui->cb_PET_EVENT_FLAG_GATE_OVERLAY->isChecked())     value |= PET_EVENT_FLAG_GATE_OVERLAY;
    if(ui->cb_PET_EVENT_FLAG_QDC_OVERFLOW->isChecked())     value |= PET_EVENT_FLAG_QDC_OVERFLOW;
    if(ui->cb_PET_EVENT_FLAG_QDC_UNDERFLOW->isChecked())    value |= PET_EVENT_FLAG_QDC_UNDERFLOW;
    if(ui->cb_PET_EVENT_FLAG_ADC_OVERFLOW->isChecked())     value |= PET_EVENT_FLAG_ADC_OVERFLOW;
    if(ui->cb_PET_EVENT_FLAG_ADC_UNDERFLOW->isChecked())    value |= PET_EVENT_FLAG_ADC_UNDERFLOW;
    if(ui->cb_PET_EVENT_FLAG_SYNC->isChecked())             value |= PET_EVENT_FLAG_SYNC;

    return value;
}
//--------------------------------------------------------------------------------
