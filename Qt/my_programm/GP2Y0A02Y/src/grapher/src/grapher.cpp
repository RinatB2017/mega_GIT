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
#include "grapher.hpp"
#include "defines.hpp"
#include "ui_grapher.h"
//--------------------------------------------------------------------------------
Grapher::Grapher(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Grapher)
{
    init();
}
//--------------------------------------------------------------------------------
Grapher::~Grapher()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Grapher::init(void)
{
    ui->setupUi(this);

    ui->grapher->set_silense(true);
    ui->grapher->set_visible_btn_Options(false);
    ui->grapher->set_visible_btn_all_ON(true);
    ui->grapher->set_visible_btn_all_OFF(true);
    ui->grapher->set_visible_btn_Clear(true);
    ui->grapher->set_visible_btn_Load(false);
    ui->grapher->set_visible_btn_Save(false);
    ui->grapher->set_visible_btn_Statistic(false);

    ui->grapher->set_title("Измерение расстояний");
    ui->grapher->set_title_axis_X("Измерение");
    ui->grapher->set_title_axis_Y("Дистанция");

    for(int n=0; n<8; n++)
    {
        curves[n] = ui->grapher->add_curve(QString("%1").arg(n));
    }

    emit info("Grapher started");
}
//--------------------------------------------------------------------------------
void Grapher::clean_all(void)
{
    ui->grapher->clear();
}
//--------------------------------------------------------------------------------
bool Grapher::set_value(int channel, int value)
{
    emit debug("Grapher: set_value");

    if(channel < 0)
    {
        emit error("Grapher: channel < 0");
        return false;
    }
    if(channel > MAX_CHANNEL)
    {
        emit error(QString("Grapher: channel > MAX_CHANNEL (%1)").arg(channel));
        return false;
    }

    ui->grapher->add_curve_data(curves[channel], value);
    return true;
}
//--------------------------------------------------------------------------------
void Grapher::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
