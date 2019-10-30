/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "rs232_widget.hpp"
#include "ui_rs232_widget.h"
//--------------------------------------------------------------------------------
RS232_widget::RS232_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::RS232_widget)
{
    init();
}
//--------------------------------------------------------------------------------
RS232_widget::~RS232_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void RS232_widget::init(void)
{
    ui->setupUi(this);

    ui->rs232_widget->set_caption("RS232");

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    connect(ui->btn_start,  SIGNAL(clicked()),  this,   SLOT(start()));
    connect(ui->btn_stop,   SIGNAL(clicked()),  this,   SLOT(stop()));

    connect(this, SIGNAL(send(QByteArray)),    ui->rs232_widget,   SLOT(input(QByteArray)));
}
//--------------------------------------------------------------------------------
void RS232_widget::start(void)
{
    emit error("start");
}
//--------------------------------------------------------------------------------
void RS232_widget::stop(void)
{
    emit error("stop");
}
//--------------------------------------------------------------------------------
void RS232_widget::updateText()
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool RS232_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void RS232_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void RS232_widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
