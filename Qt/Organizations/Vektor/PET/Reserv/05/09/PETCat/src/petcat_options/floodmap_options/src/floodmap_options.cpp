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
#include <QMouseEvent>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "floodmap_options.hpp"
#include "ui_floodmap_options.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
Floodmap_options::Floodmap_options(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Floodmap_options)
{
    init();
}
//--------------------------------------------------------------------------------
Floodmap_options::~Floodmap_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Floodmap_options::init(void)
{
    ui->setupUi(this);

    //---
    block_wheel();
    //---
    QVariant sb_min_energy = 0;
    QVariant sb_max_energy = 0;
    QVariant f_value = 0xFE;
    QVariant flag_min_energy = 0;
    QVariant flag_max_energy = 0;
    //---
    PETCat_options *o = new PETCat_options();
    o->get_floodmap_option(PARAM_floodmap_sb_min_energy,    &sb_min_energy);
    o->get_floodmap_option(PARAM_floodmap_sb_max_energy,    &sb_max_energy);
    o->get_floodmap_option(PARAM_floodmap_events_flags,     &f_value);
    o->get_floodmap_option(PARAM_floodmap_flag_min_energy,  &flag_min_energy);
    o->get_floodmap_option(PARAM_floodmap_flag_max_energy,  &flag_max_energy);
    o->deleteLater();
    //---
    ui->sb_min_energy->setRange(0, 4 * 0xFFFF);
    ui->sb_min_energy->setValue(sb_min_energy.toInt());

    ui->sb_max_energy->setRange(0, 4 * 0xFFFF);
    ui->sb_max_energy->setValue(sb_max_energy.toInt());

    ui->cb_enable_filter_min_energy->setChecked(flag_min_energy.toBool());
    ui->cb_enable_filter_max_energy->setChecked(flag_max_energy.toBool());
    //---
    uint8_t value = f_value.toInt();

    ui->cb_PET_EVENT_FLAG_FAULT->setChecked(value & PET_EVENT_FLAG_FAULT);
    ui->cb_PET_EVENT_FLAG_TS_INVALID->setChecked(value & PET_EVENT_FLAG_TS_INVALID);
    ui->cb_PET_EVENT_FLAG_GATE_OVERLAY->setChecked(value & PET_EVENT_FLAG_GATE_OVERLAY);
    ui->cb_PET_EVENT_FLAG_QDC_OVERFLOW->setChecked(value & PET_EVENT_FLAG_QDC_OVERFLOW);
    ui->cb_PET_EVENT_FLAG_QDC_UNDERFLOW->setChecked(value & PET_EVENT_FLAG_QDC_UNDERFLOW);
    ui->cb_PET_EVENT_FLAG_ADC_OVERFLOW->setChecked(value & PET_EVENT_FLAG_ADC_OVERFLOW);
    ui->cb_PET_EVENT_FLAG_ADC_UNDERFLOW->setChecked(value & PET_EVENT_FLAG_ADC_UNDERFLOW);
    ui->cb_PET_EVENT_FLAG_SYNC->setChecked(value & PET_EVENT_FLAG_SYNC);
    //---

    connect(ui->btn_accept, SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));
}
//--------------------------------------------------------------------------------
void Floodmap_options::t_accept(void)
{
    //---
    uint8_t value = 0;

    if(ui->cb_PET_EVENT_FLAG_FAULT->isChecked())            value |= PET_EVENT_FLAG_FAULT;
    if(ui->cb_PET_EVENT_FLAG_TS_INVALID->isChecked())       value |= PET_EVENT_FLAG_TS_INVALID;
    if(ui->cb_PET_EVENT_FLAG_GATE_OVERLAY->isChecked())     value |= PET_EVENT_FLAG_GATE_OVERLAY;
    if(ui->cb_PET_EVENT_FLAG_QDC_OVERFLOW->isChecked())     value |= PET_EVENT_FLAG_QDC_OVERFLOW;
    if(ui->cb_PET_EVENT_FLAG_QDC_UNDERFLOW->isChecked())    value |= PET_EVENT_FLAG_QDC_UNDERFLOW;
    if(ui->cb_PET_EVENT_FLAG_ADC_OVERFLOW->isChecked())     value |= PET_EVENT_FLAG_ADC_OVERFLOW;
    if(ui->cb_PET_EVENT_FLAG_ADC_UNDERFLOW->isChecked())    value |= PET_EVENT_FLAG_ADC_UNDERFLOW;
    if(ui->cb_PET_EVENT_FLAG_SYNC->isChecked())             value |= PET_EVENT_FLAG_SYNC;
    //---

    PETCat_options *o = new PETCat_options();
    o->set_floodmap_option(PARAM_floodmap_sb_min_energy,    ui->sb_min_energy->value());
    o->set_floodmap_option(PARAM_floodmap_sb_max_energy,    ui->sb_max_energy->value());
    o->set_floodmap_option(PARAM_floodmap_events_flags,     value);
    o->set_floodmap_option(PARAM_floodmap_flag_min_energy,  ui->cb_enable_filter_min_energy->isChecked());
    o->set_floodmap_option(PARAM_floodmap_flag_max_energy,  ui->cb_enable_filter_max_energy->isChecked());
    o->deleteLater();

    messagebox_info("Сохранение", "OK");
}
//--------------------------------------------------------------------------------
void Floodmap_options::block_wheel(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(!obj->objectName().isEmpty())
        {
            obj->installEventFilter(this);
        }
    }
}
//--------------------------------------------------------------------------------
bool Floodmap_options::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
