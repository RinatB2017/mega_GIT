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

    widgets.append(ui->btn_0);
    widgets.append(ui->btn_1);
    widgets.append(ui->btn_2);
    widgets.append(ui->btn_3);
    widgets.append(ui->btn_4);
    widgets.append(ui->btn_5);
    widgets.append(ui->btn_6);
    widgets.append(ui->btn_7);
    widgets.append(ui->btn_8);
    widgets.append(ui->btn_9);

    int index = 0;
    foreach (QAbstractButton *btn, widgets)
    {
        btn->setProperty("value", index++);
        connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(click()));
    }

    connect(ui->btn_back,   SIGNAL(clicked(bool)),  this,   SLOT(s_back()));
    connect(ui->btn_clear,  SIGNAL(clicked(bool)),  this,   SLOT(s_clear()));

    s_clear();
}
//--------------------------------------------------------------------------------
void Safe::click(void)
{
    QToolButton *btn = dynamic_cast<QToolButton *>(sender());
    Q_CHECK_PTR(btn);

    bool ok = false;
    int x = btn->property("value").toInt(&ok);
    if(ok)
    {
        int new_value = value;
        new_value *= 10;
        new_value += x;
        if(new_value > 99999)
        {
            return;
        }
        value = new_value;
        ui->lcdNumber->display(value);
    }
}
//--------------------------------------------------------------------------------
void Safe::s_back(void)
{
    value /= 10;
    ui->lcdNumber->display(value);
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
