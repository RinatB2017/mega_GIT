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
//--------------------------------------------------------------------------------
#include "ui_udp_client_mainbox.h"
//--------------------------------------------------------------------------------
#include "udp_client.hpp"
#include "udp_client_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    ui->setupUi(this);

    init();;
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    if(client)
    {
        delete client;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    init_widgets();
    init_client();

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->le_data->setText("test");
    ui->ipv4_widget->set_url(QUrl(QHostAddress(QHostAddress::LocalHost).toString()));
}
//--------------------------------------------------------------------------------
void MainBox::init_client(void)
{
    client = new UDP_Client();
    client->setAddress(QHostAddress(ui->ipv4_widget->get_url().host()));
    client->setPort(static_cast<quint16>(ui->ipv4_widget->get_url().port()));

    connect(ui->btn_Send, &QPushButton::clicked,    this,   &MainBox::send);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::send(void)
{
    QByteArray data;
    QByteArray res_data;

    client->setAddress(QHostAddress(ui->ipv4_widget->get_url().host()));
    client->setPort(static_cast<quint16>(ui->ipv4_widget->get_url().port()));

    data.clear();
    data.append(ui->le_data->text().toLatin1());
    emit debug(QString("send(%1)").arg(data.data()));
    res_data = client->input(data);
    emit debug(res_data.toHex());
    emit info(res_data);
}
//--------------------------------------------------------------------------------
