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
#include "ui_broadcast_udp_client_mainbox.h"
//--------------------------------------------------------------------------------
#include "udp_client.hpp"
#include "broadcast_udp_client_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init()
{
    ui->setupUi(this);

    client = new UDP_Client(this);

    connect(ui->btn_Send,   &QPushButton::clicked,  this,   &MainBox::send);

    setMinimumWidth(800);
}
//--------------------------------------------------------------------------------
void MainBox::updateText()
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit()
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting()
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting()
{

}
//--------------------------------------------------------------------------------
void MainBox::send()
{
    QByteArray ba;

    ba.clear();
    ba.append("xxx");

    client->send_data(ba);
}
//--------------------------------------------------------------------------------
