/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "autoclicker_item.hpp"
#include "ui_autoclicker_item.h"
//--------------------------------------------------------------------------------
AutoClicker_item::AutoClicker_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoClicker_item)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
AutoClicker_item::~AutoClicker_item()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void AutoClicker_item::init(void)
{
    ui->sb_pos_x->setRange(0, 1000);
    ui->sb_pos_y->setRange(0, 1000);

    connect(ui->btn_remove_item,    &QPushButton::clicked,  this,   &AutoClicker_item::remove_item);
}
//--------------------------------------------------------------------------------
void AutoClicker_item::set_name(const QString &name)
{
    this->name = name;
    ui->lbl_name->setText(name);
}
//--------------------------------------------------------------------------------
QString AutoClicker_item::get_name(void)
{
    return name;
}
//--------------------------------------------------------------------------------
void AutoClicker_item::set_id(int new_id)
{
    id = new_id;
}
//--------------------------------------------------------------------------------
int AutoClicker_item::get_id(void)
{
    return id;
}
//--------------------------------------------------------------------------------
QTime AutoClicker_item::get_time(void)
{
    return ui->te_time->time();
}
//--------------------------------------------------------------------------------
void AutoClicker_item::set_x(int x)
{
    ui->sb_pos_x->setValue(x);
}
//--------------------------------------------------------------------------------
int  AutoClicker_item::get_x(void)
{
    return ui->sb_pos_x->value();
}
//--------------------------------------------------------------------------------
void AutoClicker_item::set_y(int y)
{
    ui->sb_pos_y->setValue(y);
}
//--------------------------------------------------------------------------------
int  AutoClicker_item::get_y(void)
{
    return ui->sb_pos_y->value();
}
//--------------------------------------------------------------------------------
