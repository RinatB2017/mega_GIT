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
#include "timer_options.hpp"
#include "ui_timer_options.h"
//--------------------------------------------------------------------------------
Timer_options::Timer_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Timer_options)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
Timer_options::~Timer_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Timer_options::set_time(const QTime &time)
{
    ui->timeEdit->setTime(time);
}
//--------------------------------------------------------------------------------
QTime Timer_options::get_time(void)
{
    return ui->timeEdit->time();
}
//--------------------------------------------------------------------------------
void Timer_options::set_message(const QString &message)
{
    ui->le_message->setText(message);
}
//--------------------------------------------------------------------------------
QString Timer_options::get_message(void)
{
    return ui->le_message->text();
}
//--------------------------------------------------------------------------------
