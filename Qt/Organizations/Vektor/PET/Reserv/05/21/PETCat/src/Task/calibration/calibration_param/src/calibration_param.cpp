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
#include <QSpinBox>
//--------------------------------------------------------------------------------
#include "calibration_param.hpp"
#include "ui_calibration_param.h"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_brg.h"
#include "pet.h"
//--------------------------------------------------------------------------------
// pet_brg_ch_calibration
//--------------------------------------------------------------------------------
Calibration_param::Calibration_param(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Calibration_param)
{
    init();
}
//--------------------------------------------------------------------------------
Calibration_param::~Calibration_param()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Calibration_param::init(void)
{
    ui->setupUi(this);

    ui->sb_N->setRange(1, PET_WAVE_POINT_MAX);
    ui->sb_N->setValue(PET_WAVE_POINT_MAX);

    ui->sb_max_cnt_read->setRange(1, 100);
    ui->sb_max_cnt_calib->setRange(1, 100);
    ui->sb_timeout->setRange(100, 100 * 1000);

    ui->dsb_A_min->setRange(0.0, 2.5);
    ui->dsb_A_max->setRange(0.0, 2.5);
    ui->dsb_F->setRange(1.0, 20000.0);
    ui->dsb_K->setRange(0.01, 100.0);
    ui->dsb_B->setRange(0.01, 100.0);

    ui->dsb_A_min->setValue(0.1);
    ui->dsb_A_max->setValue(2.4);

    ui->dsb_F->setValue(10000.0);
}
//--------------------------------------------------------------------------------
double Calibration_param::get_A_min(void)
{
    return ui->dsb_A_min->value();
}
//--------------------------------------------------------------------------------
double Calibration_param::get_A_max(void)
{
    return ui->dsb_A_max->value();
}
//--------------------------------------------------------------------------------
double Calibration_param::get_B(void)
{
    return ui->dsb_B->value();
}
//--------------------------------------------------------------------------------
double Calibration_param::get_F(void)
{
    return ui->dsb_F->value();
}
//--------------------------------------------------------------------------------
double Calibration_param::get_K(void)
{
    return ui->dsb_K->value();
}
//--------------------------------------------------------------------------------
int Calibration_param::get_N(void)
{
    return ui->sb_N->value();
}
//--------------------------------------------------------------------------------
int Calibration_param::get_max_cnt_read(void)
{
    return ui->sb_max_cnt_read->value();
}
//--------------------------------------------------------------------------------
int Calibration_param::get_max_cnt_calib(void)
{
    return ui->sb_max_cnt_calib->value();
}
//--------------------------------------------------------------------------------
int Calibration_param::get_timeout(void)
{
    return ui->sb_timeout->value();
}
//--------------------------------------------------------------------------------
void Calibration_param::set_A_min(double value)
{
    ui->dsb_A_min->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_A_max(double value)
{
    ui->dsb_A_max->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_B(double value)
{
    ui->dsb_B->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_F(double value)
{
    ui->dsb_F->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_K(double value)
{
    ui->dsb_K->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_N(int value)
{
    ui->sb_N->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_max_cnt_read(int value)
{
    ui->sb_max_cnt_read->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_max_cnt_calib(int value)
{
    ui->sb_max_cnt_calib->setValue(value);
}
//--------------------------------------------------------------------------------
void Calibration_param::set_timeout(int value)
{
    ui->sb_timeout->setValue(value);
}
//--------------------------------------------------------------------------------
