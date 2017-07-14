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
#include "gen_sinus_box.hpp"
#include "ui_gen_sinus_box.h"
//--------------------------------------------------------------------------------
Gen_sinus_box::Gen_sinus_box(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Gen_sinus_box)
{
    ui->setupUi(this);

    ui->dsb_offset_begin_y->setRange(INT16_MIN, INT16_MAX);
    ui->dsb_vert_ampl->setRange(INT16_MIN, INT16_MAX);
    ui->dsb_begin_angle->setRange(0, 359);
    ui->dsb_count_cycles->setRange(0, 100);

    connect(ui->btn_generate,   SIGNAL(clicked(bool)),
            this,               SIGNAL(generate()));
}
//--------------------------------------------------------------------------------
Gen_sinus_box::~Gen_sinus_box()
{
    delete ui;
}
//--------------------------------------------------------------------------------
double Gen_sinus_box::get_offset_begin_y(void)
{
    return ui->dsb_offset_begin_y->value();
}
//--------------------------------------------------------------------------------
double Gen_sinus_box::get_vert_ampl(void)
{
    return ui->dsb_vert_ampl->value();
}
//--------------------------------------------------------------------------------
double Gen_sinus_box::get_begin_angle(void)
{
    return ui->dsb_begin_angle->value();
}
//--------------------------------------------------------------------------------
double Gen_sinus_box::get_count_cycles(void)
{
    return ui->dsb_count_cycles->value();
}
//--------------------------------------------------------------------------------
