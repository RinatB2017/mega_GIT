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
#include <QHostAddress>
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "tcp_client.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(client)  delete client;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    init_widgets();
    init_client();
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->lne_Data->setText("test");
    ui->lne_Address->setText(QHostAddress(QHostAddress::LocalHost).toString());
    ui->sb_Port->setValue(10000);
}
//--------------------------------------------------------------------------------
void MainBox::init_client(void)
{
    client = new TCP_Client(this);
    client->setAddress(QHostAddress(ui->lne_Address->text()));
    client->setPort(ui->sb_Port->value());

    connect(ui->btn_Send, SIGNAL(clicked()), this, SLOT(send()));
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::send(void)
{
    QByteArray data;
    QByteArray res_data;

    if(ui->lne_Address->text().isEmpty()) return;
    client->setAddress(QHostAddress(ui->lne_Address->text()));
    client->setPort(ui->sb_Port->value());

    data.clear();
    data.append(ui->lne_Data->text());
    emit debug(QString("send(%1)").arg(data.data()));
    res_data = client->input(data);
    emit debug(res_data.toHex());
}
//--------------------------------------------------------------------------------
