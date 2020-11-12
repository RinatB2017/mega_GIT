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
#include "rtsp_dialog.hpp"
#include "ui_rtsp_dialog.h"
//--------------------------------------------------------------------------------
RTSP_dialog::RTSP_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RTSP_dialog)
{
    init();
}
//--------------------------------------------------------------------------------
RTSP_dialog::~RTSP_dialog()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void RTSP_dialog::init(void)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
void RTSP_dialog::set_login(const QString &login)
{
    ui->le_login->setText(login);
}
//--------------------------------------------------------------------------------
void RTSP_dialog::set_password(const QString &password)
{
    ui->le_password->setText(password);
}
//--------------------------------------------------------------------------------
void RTSP_dialog::set_param(const QString &text)
{
    ui->le_param->setText(text);
}
//--------------------------------------------------------------------------------
void RTSP_dialog::set_url(QUrl url)
{
    ui->ip4_widget->set_url(url);
}
//--------------------------------------------------------------------------------
QString RTSP_dialog::get_login(void)
{
    return ui->le_login->text();
}
//--------------------------------------------------------------------------------
QString RTSP_dialog::get_password(void)
{
    return ui->le_password->text();
}
//--------------------------------------------------------------------------------
QString RTSP_dialog::get_param(void)
{
    return ui->le_param->text();
}
//--------------------------------------------------------------------------------
QUrl RTSP_dialog::get_url(void)
{
    return ui->ip4_widget->get_url();
}
//--------------------------------------------------------------------------------
QString RTSP_dialog::get_address(void)
{
    QString address;
    address.append("rtsp://");
    if(ui->le_login->text().isEmpty() == false && ui->le_password->text().isEmpty() == false)
    {
        address.append(ui->le_login->text());
        address.append(":");
        address.append(ui->le_password->text());
        address.append("@");
    }
    address.append(QString("%1:%2")
                   .arg(ui->ip4_widget->get_url().host())
                   .arg(ui->ip4_widget->get_url().port()));
    if(ui->le_param->text().isEmpty() == false)
    {
        address.append("/");
        address.append(ui->le_param->text());
    }

    return address;
}
//--------------------------------------------------------------------------------
