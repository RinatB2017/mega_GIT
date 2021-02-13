/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "check_in_widget.hpp"
#include "ui_check_in_widget.h"
//--------------------------------------------------------------------------------
Check_in_widget::Check_in_widget(QWidget *parent) :
    Default_widget(parent),
    ui(new Ui::Check_in_widget)
{
    init();
}
//--------------------------------------------------------------------------------
Check_in_widget::~Check_in_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Check_in_widget::init(void)
{
    ui->setupUi(this);

    connect(ui->sb_1,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &Check_in_widget::check_in);
    connect(ui->sb_2,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &Check_in_widget::check_in);
    connect(ui->sb_res, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this,   &Check_in_widget::check_in);
    connect(ui->btn_ok, &QPushButton::clicked,  this,   &Check_in_widget::victory);

    check_in();
}
//--------------------------------------------------------------------------------
void Check_in_widget::check_in(void)
{
    int a = ui->sb_1->value();
    int b = ui->sb_2->value();
    int c = ui->sb_res->value();

    bool res = ((a + b) == c);
    ui->btn_ok->setEnabled(res);
}
//--------------------------------------------------------------------------------
void Check_in_widget::victory(void)
{
    messagebox_info("Info", "Victory!");
}
//--------------------------------------------------------------------------------
