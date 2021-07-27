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
#include "testbox.hpp"
#include "ui_testbox.h"
//--------------------------------------------------------------------------------
TestBox::TestBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TestBox)
{
    ui->setupUi(this);

    QList<QAbstractButton *> lists;

    ui->btn_0->setProperty("value", 0);
    ui->btn_1->setProperty("value", 1);
    ui->btn_2->setProperty("value", 2);
    ui->btn_3->setProperty("value", 3);
    ui->btn_4->setProperty("value", 4);
    ui->btn_5->setProperty("value", 5);
    ui->btn_6->setProperty("value", 6);
    ui->btn_7->setProperty("value", 7);
    ui->btn_8->setProperty("value", 8);
    ui->btn_9->setProperty("value", 9);

    lists.append(ui->btn_0);
    lists.append(ui->btn_1);
    lists.append(ui->btn_2);
    lists.append(ui->btn_3);
    lists.append(ui->btn_4);
    lists.append(ui->btn_5);
    lists.append(ui->btn_6);
    lists.append(ui->btn_7);
    lists.append(ui->btn_8);

    foreach (QAbstractButton *btn, lists)
    {
        connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(click()));
    }
}
//--------------------------------------------------------------------------------
TestBox::~TestBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void TestBox::click(void)
{
    QAbstractButton *btn = dynamic_cast<QAbstractButton *>(sender());
    if(btn)
    {
        emit info(btn->property("value").toString());
    }
}
//--------------------------------------------------------------------------------
