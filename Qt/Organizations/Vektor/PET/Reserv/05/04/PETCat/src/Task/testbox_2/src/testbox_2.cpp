/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QToolButton>
//--------------------------------------------------------------------------------
#include "testbox_2.hpp"
#include "ui_testbox_2.h"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
TestBox_2::TestBox_2(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TestBox_2)
{
    init();
    load_widgets("TestBox_2");    //TODO
}
//--------------------------------------------------------------------------------
TestBox_2::~TestBox_2()
{
    save_widgets("TestBox_2");    //TODO

    delete ui;
}
//--------------------------------------------------------------------------------
void TestBox_2::init(void)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
