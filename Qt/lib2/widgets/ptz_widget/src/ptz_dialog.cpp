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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ptz_dialog.hpp"
#include "ui_ptz_dialog.h"
//--------------------------------------------------------------------------------
PTZ_dialog::PTZ_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PTZ_dialog)
{
    init();
}
//--------------------------------------------------------------------------------
PTZ_dialog::~PTZ_dialog()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PTZ_dialog::init(void)
{
    ui->setupUi(this);

    ui->sb_port->setRange(0, 0xFFFF);

    ui->le_proto->setText("rtsp");
    ui->le_proto->setReadOnly(true);

    ui->le_login->setText("admin");
    ui->le_password->setText("admin");
    ui->ip_widget->set_url(QUrl("ip://192.168.1.1"));
    ui->sb_port->setValue(81);

    connect(ui->buttonBox,  SIGNAL(accepted()), this,   SLOT(accept()));
    connect(ui->buttonBox,  SIGNAL(rejected()), this,   SLOT(reject()));
}
//--------------------------------------------------------------------------------
void PTZ_dialog::set_login(const QString &login)
{
    ui->le_login->setText(login);
}
//--------------------------------------------------------------------------------
void PTZ_dialog::set_password(const QString &password)
{
    ui->le_password->setText(password);
}
//--------------------------------------------------------------------------------
void PTZ_dialog::set_IP(QUrl url)
{
    ui->ip_widget->set_url(url);
}
//--------------------------------------------------------------------------------
void PTZ_dialog::set_port(int port)
{
    ui->sb_port->setValue(port);
}
//--------------------------------------------------------------------------------
void PTZ_dialog::set_param(const QString &param)
{
    ui->le_param->setText(param);
}
//--------------------------------------------------------------------------------
QString PTZ_dialog::get_login(void)
{
    return ui->le_login->text();
}
//--------------------------------------------------------------------------------
QString PTZ_dialog::get_password(void)
{
    return ui->le_password->text();
}
//--------------------------------------------------------------------------------
QUrl PTZ_dialog::get_IP(void)
{
    return ui->ip_widget->get_url();
}
//--------------------------------------------------------------------------------
int PTZ_dialog::get_port(void)
{
    return ui->sb_port->value();
}
//--------------------------------------------------------------------------------
QString PTZ_dialog::get_param(void)
{
    return ui->le_param->text();
}
//--------------------------------------------------------------------------------
