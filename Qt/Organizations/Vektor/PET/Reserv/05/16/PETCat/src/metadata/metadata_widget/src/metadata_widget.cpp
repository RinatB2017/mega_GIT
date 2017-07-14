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
#include "metadata_widget.hpp"
#include "ui_metadata_widget.h"
//--------------------------------------------------------------------------------
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
MetaData_widget::MetaData_widget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MetaData_widget),
    parent(parent)
{
    ui->setupUi(this);

    ui->btn_add_metadata->setIcon(qApp->style()->standardIcon(QStyle::SP_DirOpenIcon));
    connect(ui->btn_add_metadata,   SIGNAL(clicked(bool)),  parent,   SIGNAL(signal_add_metadata_file()));

    ui->btn_add_metadata->setProperty(FLAG_NO_BLOCK, true);

    refresh_metadata();
}
//--------------------------------------------------------------------------------
MetaData_widget::~MetaData_widget()
{
    disconnect();
    delete ui;
}
//--------------------------------------------------------------------------------
void MetaData_widget::refresh_metadata(void)
{
    MetaData *md = new MetaData();
    connect(md, SIGNAL(info(QString)),      parent,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     parent,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)),     parent,   SIGNAL(error(QString)));
    connect(md, SIGNAL(message(QString)),   parent,   SIGNAL(message(QString)));
    md->load_data(ui->cb_metadata);
    md->deleteLater();
}
//--------------------------------------------------------------------------------
int MetaData_widget::count(void)
{
    return ui->cb_metadata->count();
}
//--------------------------------------------------------------------------------
void MetaData_widget::setCurrentText(QString filename)
{
    ui->cb_metadata->setCurrentText(filename);
}
//--------------------------------------------------------------------------------
QString MetaData_widget::MetaData_widget::currentText(void)
{
    return ui->cb_metadata->currentText();
}
//--------------------------------------------------------------------------------
