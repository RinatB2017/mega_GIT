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

    ui->le_add->setText("HD");
}
//--------------------------------------------------------------------------------
void PTZ_dialog::set_url(QUrl url)
{
    ui->ip4_widget->set_url(url);
}
//--------------------------------------------------------------------------------
QString PTZ_dialog::get_address(void)
{
    QString address;
    address.append("rtsp://");
    address.append(QString("%1").arg(ui->ip4_widget->get_url().host()));
    address.append("/");
    address.append(ui->le_add->text());

    return address;
}
//--------------------------------------------------------------------------------
