/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_sendbox5.h"
//--------------------------------------------------------------------------------
#include "sendbox5.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
SendBox5::SendBox5(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendBox5)
{
    ui->setupUi(this);

    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));
    }

    connect(ui->btn_text_commands,  &QPushButton::clicked,  this,   &SendBox5::send_text_command);
    connect(ui->btn_bin_commands,   &QPushButton::clicked,  this,   &SendBox5::send_bin_command);
}
//--------------------------------------------------------------------------------
SendBox5::~SendBox5()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void SendBox5::send_text_command(void)
{
    Text_widget *tw = new Text_widget();
    connect(tw, &Text_widget::info,     this,   &SendBox5::info);
    connect(tw, &Text_widget::debug,    this,   &SendBox5::debug);
    connect(tw, &Text_widget::error,    this,   &SendBox5::error);
    connect(tw, &Text_widget::trace,    this,   &SendBox5::trace);
    connect(tw, &Text_widget::send_command, this,   &SendBox5::sendData);
    tw->exec();
}
//--------------------------------------------------------------------------------
void SendBox5::send_bin_command(void)
{
    Bin_widget *hw = new Bin_widget();
    connect(hw, &Bin_widget::info,     this,   &SendBox5::info);
    connect(hw, &Bin_widget::debug,    this,   &SendBox5::debug);
    connect(hw, &Bin_widget::error,    this,   &SendBox5::error);
    connect(hw, &Bin_widget::trace,    this,   &SendBox5::trace);
    connect(hw, &Bin_widget::send_command, this,   &SendBox5::sendData);
    hw->exec();
}
//--------------------------------------------------------------------------------
void SendBox5::block_interface(bool state)
{
    ui->btn_text_commands->setDisabled(state);
    ui->btn_bin_commands->setDisabled(state);
}
//--------------------------------------------------------------------------------
void SendBox5::updateText(void)
{

}
//--------------------------------------------------------------------------------
