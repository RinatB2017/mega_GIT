/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include "ui_template_mainbox_GUI.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "template_mainbox_GUI.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox_GUI::MainBox_GUI(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox_GUI)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox_GUI::~MainBox_GUI()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox_GUI::init(void)
{
    ui->setupUi(this);

    connect(ui->btn_plus, &QPushButton::clicked, [this]() {
            ui->sb_value->setValue(ui->sb_value->value()+1);
        });
    connect(ui->btn_minus, &QPushButton::clicked, [this]() {
            ui->sb_value->setValue(ui->sb_value->value()-1);
        });

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox_GUI::set_value(int value)
{
    ui->sb_value->setValue(value);
}
//--------------------------------------------------------------------------------
int  MainBox_GUI::get_value(void)
{
    return ui->sb_value->value();
}
//--------------------------------------------------------------------------------
void MainBox_GUI::inc_value(void)
{
    ui->sb_value->setValue(ui->sb_value->value()+1);
}
//--------------------------------------------------------------------------------
void MainBox_GUI::dec_value(void)
{
    ui->sb_value->setValue(ui->sb_value->value()-1);
}
//--------------------------------------------------------------------------------
void MainBox_GUI::set_range(int min, int max)
{
    ui->sb_value->setRange(min, max);
}
//--------------------------------------------------------------------------------
void MainBox_GUI::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
