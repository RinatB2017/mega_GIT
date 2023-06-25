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
    ui->cb_D1_1->setChecked(data.bites.D1_1);
    ui->cb_D1_2->setChecked(data.bites.D1_2);
    ui->cb_D1_3->setChecked(data.bites.D1_3);
    ui->cb_D1_4->setChecked(data.bites.D1_4);

    ui->cb_D4_1->setChecked(data.bites.D4_1);
    ui->cb_D4_2->setChecked(data.bites.D4_2);
    ui->cb_D4_3->setChecked(data.bites.D4_3);
    ui->cb_D4_4->setChecked(data.bites.D4_4);

    ui->cb_D5_1->setChecked(data.bites.D5_1);
    ui->cb_D5_2->setChecked(data.bites.D5_2);
    ui->cb_D5_3->setChecked(data.bites.D5_3);
    ui->cb_D5_4->setChecked(data.bites.D5_4);

    ui->cb_D6_1->setChecked(data.bites.D6_1);
    ui->cb_D6_2->setChecked(data.bites.D6_2);
    ui->cb_D6_3->setChecked(data.bites.D6_3);
    ui->cb_D6_4->setChecked(data.bites.D6_4);

    ui->cb_D15_1->setChecked(data.bites.D15_1);
    ui->cb_D15_2->setChecked(data.bites.D15_2);
    ui->cb_D15_3->setChecked(data.bites.D15_3);
    ui->cb_D15_4->setChecked(data.bites.D15_4);

    ui->cb_D16_1->setChecked(data.bites.D16_1);
    ui->cb_D16_2->setChecked(data.bites.D16_2);
    ui->cb_D16_3->setChecked(data.bites.D16_3);
    ui->cb_D16_4->setChecked(data.bites.D16_4);

    ui->cb_D27_1->setChecked(data.bites.D27_1);
    ui->cb_D27_2->setChecked(data.bites.D27_2);
    ui->cb_D27_3->setChecked(data.bites.D27_3);
    ui->cb_D27_4->setChecked(data.bites.D27_4);
}
//--------------------------------------------------------------------------------
SWITCH Switches_box::get(void)
{
    SWITCH sw;

    sw.value = 0;
    sw.bites.D1_1 = ui->cb_D1_1->isChecked();
    sw.bites.D1_2 = ui->cb_D1_2->isChecked();
    sw.bites.D1_3 = ui->cb_D1_3->isChecked();
    sw.bites.D1_4 = ui->cb_D1_4->isChecked();

    sw.bites.D4_1 = ui->cb_D4_1->isChecked();
    sw.bites.D4_2 = ui->cb_D4_2->isChecked();
    sw.bites.D4_3 = ui->cb_D4_3->isChecked();
    sw.bites.D4_4 = ui->cb_D4_4->isChecked();

    sw.bites.D5_1 = ui->cb_D5_1->isChecked();
    sw.bites.D5_2 = ui->cb_D5_2->isChecked();
    sw.bites.D5_3 = ui->cb_D5_3->isChecked();
    sw.bites.D5_4 = ui->cb_D5_4->isChecked();

    sw.bites.D6_1 = ui->cb_D6_1->isChecked();
    sw.bites.D6_2 = ui->cb_D6_2->isChecked();
    sw.bites.D6_3 = ui->cb_D6_3->isChecked();
    sw.bites.D6_4 = ui->cb_D6_4->isChecked();

    sw.bites.D15_1 = ui->cb_D15_1->isChecked();
    sw.bites.D15_2 = ui->cb_D15_2->isChecked();
    sw.bites.D15_3 = ui->cb_D15_3->isChecked();
    sw.bites.D15_4 = ui->cb_D15_4->isChecked();

    sw.bites.D16_1 = ui->cb_D16_1->isChecked();
    sw.bites.D16_2 = ui->cb_D16_2->isChecked();
    sw.bites.D16_3 = ui->cb_D16_3->isChecked();
    sw.bites.D16_4 = ui->cb_D16_4->isChecked();

    sw.bites.D27_1 = ui->cb_D27_1->isChecked();
    sw.bites.D27_2 = ui->cb_D27_2->isChecked();
    sw.bites.D27_3 = ui->cb_D27_3->isChecked();
    sw.bites.D27_4 = ui->cb_D27_4->isChecked();

    return sw;
}
//--------------------------------------------------------------------------------
