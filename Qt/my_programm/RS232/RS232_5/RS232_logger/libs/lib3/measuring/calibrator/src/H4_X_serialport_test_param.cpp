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
#include "H4_X_serialport_test_param.hpp"
#include "ui_H4_X_serialport_test_param.h"
//--------------------------------------------------------------------------------
SerialPort_H4_X_test_param::SerialPort_H4_X_test_param(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPort_H4_X_test_param)
{
    init();
}
//--------------------------------------------------------------------------------
SerialPort_H4_X_test_param::~SerialPort_H4_X_test_param()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test_param::init(void)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test_param::f_info(const QString  &message)
{
    QByteArray ba = message.toLocal8Bit();
    QByteArray ba_temp = ba.replace("### H4-6: ", "");
    ui->log_widget->infoLog(ba_temp);
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test_param::f_debug(const QString &message)
{
    QByteArray ba = message.toLocal8Bit();
    QByteArray ba_temp = ba.replace("### H4-6: ", "");
    ui->log_widget->infoLog(ba_temp);
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test_param::f_error(const QString &message)
{
    QByteArray ba = message.toLocal8Bit();
    QByteArray ba_temp = ba.replace("### H4-6: ", "");
    ui->log_widget->infoLog(ba_temp);
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test_param::f_trace(const QString &message)
{
    QByteArray ba = message.toLocal8Bit();
    QByteArray ba_temp = ba.replace("### H4-6: ", "");
    ui->log_widget->infoLog(ba_temp);
}
//--------------------------------------------------------------------------------
