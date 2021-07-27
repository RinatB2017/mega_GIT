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
#include "inventory_widget.hpp"
#include "ui_inventory_widget.h"
//--------------------------------------------------------------------------------
Inventory_widget::Inventory_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inventory_widget)
{
    init();
}
//--------------------------------------------------------------------------------
Inventory_widget::~Inventory_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Inventory_widget::init(void)
{
    ui->setupUi(this);

    ui->label->set_pixmap(":/images/apple.png"); //TODO костыль
    ui->label->set_flag_need_clear(false);

    connect(ui->btn_clear,  &QPushButton::clicked,  ui->tableWidget,   &Table_widget::clear_items);
}
//--------------------------------------------------------------------------------
