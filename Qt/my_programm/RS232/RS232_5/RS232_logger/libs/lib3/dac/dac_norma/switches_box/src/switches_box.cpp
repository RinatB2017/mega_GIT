/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "switches_box.hpp"
#include "ui_switches_box.h"
//--------------------------------------------------------------------------------
Switches_box::Switches_box(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Switches_box)
{
    init();
}
//--------------------------------------------------------------------------------
Switches_box::~Switches_box()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Switches_box::init(void)
{
    ui->setupUi(this);

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void Switches_box::set(SWITCH data)
{
    ui->cb_D1_1->setChecked(data.data_0.bites.D1_1);
    ui->cb_D1_2->setChecked(data.data_0.bites.D1_2);
    ui->cb_D1_3->setChecked(data.data_0.bites.D1_3);
    ui->cb_D1_4->setChecked(data.data_0.bites.D1_4);

    ui->cb_D2_1->setChecked(data.data_0.bites.D2_1);
    ui->cb_D2_2->setChecked(data.data_0.bites.D2_2);
    ui->cb_D2_3->setChecked(data.data_0.bites.D2_3);
    ui->cb_D2_4->setChecked(data.data_0.bites.D2_4);

    ui->cb_D3_1->setChecked(data.data_0.bites.D3_1);
    ui->cb_D3_2->setChecked(data.data_0.bites.D3_2);
    ui->cb_D3_3->setChecked(data.data_0.bites.D3_3);
    ui->cb_D3_4->setChecked(data.data_0.bites.D3_4);

    ui->cb_D4_1->setChecked(data.data_0.bites.D4_1);
    ui->cb_D4_2->setChecked(data.data_0.bites.D4_2);
    ui->cb_D4_3->setChecked(data.data_0.bites.D4_3);
    ui->cb_D4_4->setChecked(data.data_0.bites.D4_4);

    ui->cb_D5_1->setChecked(data.data_0.bites.D5_1);
    ui->cb_D5_2->setChecked(data.data_0.bites.D5_2);
    ui->cb_D5_3->setChecked(data.data_0.bites.D5_3);
    ui->cb_D5_4->setChecked(data.data_0.bites.D5_4);

    ui->cb_D6_1->setChecked(data.data_0.bites.D6_1);
    ui->cb_D6_2->setChecked(data.data_0.bites.D6_2);
    ui->cb_D6_3->setChecked(data.data_0.bites.D6_3);
    ui->cb_D6_4->setChecked(data.data_0.bites.D6_4);

    ui->cb_D7_1->setChecked(data.data_0.bites.D7_1);
    ui->cb_D7_2->setChecked(data.data_0.bites.D7_2);
    ui->cb_D7_3->setChecked(data.data_0.bites.D7_3);
    ui->cb_D7_4->setChecked(data.data_0.bites.D7_4);

    ui->cb_D8_1->setChecked(data.data_0.bites.D8_1);
    ui->cb_D8_2->setChecked(data.data_0.bites.D8_2);
    ui->cb_D8_3->setChecked(data.data_0.bites.D8_3);
    ui->cb_D8_4->setChecked(data.data_0.bites.D8_4);

    ui->cb_D9_1->setChecked(data.data_1.bites.D9_1);
    ui->cb_D9_2->setChecked(data.data_1.bites.D9_2);
    ui->cb_D9_3->setChecked(data.data_1.bites.D9_3);
    ui->cb_D9_4->setChecked(data.data_1.bites.D9_4);
}
//--------------------------------------------------------------------------------
SWITCH Switches_box::get(void)
{
    SWITCH sw;

    sw.data_0.bites.D1_1 = ui->cb_D1_1->isChecked();
    sw.data_0.bites.D1_2 = ui->cb_D1_2->isChecked();
    sw.data_0.bites.D1_3 = ui->cb_D1_3->isChecked();
    sw.data_0.bites.D1_4 = ui->cb_D1_4->isChecked();

    sw.data_0.bites.D2_1 = ui->cb_D2_1->isChecked();
    sw.data_0.bites.D2_2 = ui->cb_D2_2->isChecked();
    sw.data_0.bites.D2_3 = ui->cb_D2_3->isChecked();
    sw.data_0.bites.D2_4 = ui->cb_D2_4->isChecked();

    sw.data_0.bites.D3_1 = ui->cb_D3_1->isChecked();
    sw.data_0.bites.D3_2 = ui->cb_D3_2->isChecked();
    sw.data_0.bites.D3_3 = ui->cb_D3_3->isChecked();
    sw.data_0.bites.D3_4 = ui->cb_D3_4->isChecked();

    sw.data_0.bites.D4_1 = ui->cb_D4_1->isChecked();
    sw.data_0.bites.D4_2 = ui->cb_D4_2->isChecked();
    sw.data_0.bites.D4_3 = ui->cb_D4_3->isChecked();
    sw.data_0.bites.D4_4 = ui->cb_D4_4->isChecked();

    sw.data_0.bites.D5_1 = ui->cb_D5_1->isChecked();
    sw.data_0.bites.D5_2 = ui->cb_D5_2->isChecked();
    sw.data_0.bites.D5_3 = ui->cb_D5_3->isChecked();
    sw.data_0.bites.D5_4 = ui->cb_D5_4->isChecked();

    sw.data_0.bites.D6_1 = ui->cb_D6_1->isChecked();
    sw.data_0.bites.D6_2 = ui->cb_D6_2->isChecked();
    sw.data_0.bites.D6_3 = ui->cb_D6_3->isChecked();
    sw.data_0.bites.D6_4 = ui->cb_D6_4->isChecked();

    sw.data_0.bites.D7_1 = ui->cb_D7_1->isChecked();
    sw.data_0.bites.D7_2 = ui->cb_D7_2->isChecked();
    sw.data_0.bites.D7_3 = ui->cb_D7_3->isChecked();
    sw.data_0.bites.D7_4 = ui->cb_D7_4->isChecked();

    sw.data_0.bites.D8_1 = ui->cb_D8_1->isChecked();
    sw.data_0.bites.D8_2 = ui->cb_D8_2->isChecked();
    sw.data_0.bites.D8_3 = ui->cb_D8_3->isChecked();
    sw.data_0.bites.D8_4 = ui->cb_D8_4->isChecked();

    sw.data_1.bites.D9_1 = ui->cb_D9_1->isChecked();
    sw.data_1.bites.D9_2 = ui->cb_D9_2->isChecked();
    sw.data_1.bites.D9_3 = ui->cb_D9_3->isChecked();
    sw.data_1.bites.D9_4 = ui->cb_D9_4->isChecked();

    return sw;
}
//--------------------------------------------------------------------------------
