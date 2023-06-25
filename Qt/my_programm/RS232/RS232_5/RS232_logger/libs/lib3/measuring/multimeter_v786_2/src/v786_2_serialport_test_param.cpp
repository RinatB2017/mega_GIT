/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include "v786_2_serialport_test_param.hpp"
#include "ui_v786_2_serialport_test_param.h"
//--------------------------------------------------------------------------------
SerialPort_V786_2_test_param::SerialPort_V786_2_test_param(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPort_V786_2_test_param)
{
    init();
}
//--------------------------------------------------------------------------------
SerialPort_V786_2_test_param::~SerialPort_V786_2_test_param()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test_param::init(void)
{
    ui->setupUi(this);

    ui->sb_address->setMinimum(0);
    ui->sb_address->setMaximum(255);

    ui->sb_serno->setMinimum(0);
    ui->sb_serno->setMaximum(9999);

    ui->sb_year->setMinimum(2010);
    ui->sb_year->setMaximum(2100);

    ui->sb_channel->setMinimum(1);
    ui->sb_channel->setMaximum(15);

    connect(ui->btn_get,    &QPushButton::clicked,
            this,           &SerialPort_V786_2_test_param::s_get);
    connect(ui->btn_set,    &QPushButton::clicked,
            this,           &SerialPort_V786_2_test_param::s_set);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test_param::set_serno(int value)
{
    ui->sb_serno->setValue(value);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test_param::set_year(int value)
{
    ui->sb_year->setValue(value);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test_param::set_address(int value)
{
    ui->sb_address->setValue(value);
}
//--------------------------------------------------------------------------------
void SerialPort_V786_2_test_param::set_channel(int value)
{
    ui->sb_channel->setValue(value);
}
//--------------------------------------------------------------------------------
int SerialPort_V786_2_test_param::get_serno(void)
{
    return ui->sb_serno->value();
}
//--------------------------------------------------------------------------------
int SerialPort_V786_2_test_param::get_year(void)
{
    return ui->sb_year->value();
}
//--------------------------------------------------------------------------------
int SerialPort_V786_2_test_param::get_address(void)
{
    return ui->sb_address->value();
}
//--------------------------------------------------------------------------------
int SerialPort_V786_2_test_param::get_channel(void)
{
    return ui->sb_channel->value();
}
//--------------------------------------------------------------------------------
