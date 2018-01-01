/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include "safe.hpp"
#include "ui_safe.h"
//--------------------------------------------------------------------------------
Safe::Safe(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Safe)
{
    init();
}
//--------------------------------------------------------------------------------
Safe::~Safe()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Safe::init(void)
{
    ui->setupUi(this);

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

    connect(ui->btn_0,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_1,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_2,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_3,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_4,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_5,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_6,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_7,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_8,  SIGNAL(clicked(bool)),  this,   SLOT(click()));
    connect(ui->btn_9,  SIGNAL(clicked(bool)),  this,   SLOT(click()));

    connect(ui->btn_back,   SIGNAL(clicked(bool)),  this,   SLOT(s_back()));
    connect(ui->btn_clear,  SIGNAL(clicked(bool)),  this,   SLOT(s_clear()));

    ui->lcdNumber->display(0);
}
//--------------------------------------------------------------------------------
void Safe::click(void)
{
    QToolButton *btn = dynamic_cast<QToolButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    bool ok = false;
    int x = btn->property("value").toInt(&ok);
    if(ok)
    {
        value *= 10;
        value += x;
        if(value > 99999)
        {
            value = 99999;
        }
        ui->lcdNumber->display(value);
    }
}
//--------------------------------------------------------------------------------
void Safe::s_back(void)
{

}
//--------------------------------------------------------------------------------
void Safe::s_clear(void)
{
    value = 0;
    ui->lcdNumber->display(value);
}
//--------------------------------------------------------------------------------
int Safe::get_value(void)
{
    return value;
}
//--------------------------------------------------------------------------------
void Safe::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
