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
#include "testdialog.hpp"
#include "ui_testdialog.h"
//--------------------------------------------------------------------------------
TestDialog::TestDialog(int max_index,
                       int max_offset,
                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog),
    max_index(max_index),
    max_offset(max_offset)
{
    ui->setupUi(this);

    init();;
}
//--------------------------------------------------------------------------------
TestDialog::~TestDialog()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void TestDialog::init(void)
{
    ui->sb_index->setRange(0,  max_index);
    ui->sb_offset->setRange(0, max_offset);

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
int TestDialog::get_index(void)
{
    return ui->sb_index->value();
}
//--------------------------------------------------------------------------------
int TestDialog::get_offset(void)
{
    return ui->sb_offset->value();
}
//--------------------------------------------------------------------------------
