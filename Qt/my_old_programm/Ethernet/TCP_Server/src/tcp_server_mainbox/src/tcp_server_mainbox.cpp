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
#include <QNetworkInterface>
#include <QTcpServer>
//--------------------------------------------------------------------------------
#include "ui_tcp_server_mainbox.h"
//--------------------------------------------------------------------------------
#include "tcp_server_mainbox.hpp"
#include "tcp_server.hpp"
#include "defines.hpp"
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
    save_widgets();
    if(server)
    {
        delete server;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    ui->sb_port->setRange(0, 0xFFFF);
    ui->sb_port->setValue(554);

    server = new TCP_Server(this);

    connect(server,             &TCP_Server::output,    this,   &MainBox::f_get_data);
    connect(ui->btn_create,     &QPushButton::clicked,  this,   &MainBox::f_connect);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::f_connect(void)
{
    if(server)
    {
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
            {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        if (ipAddress.isEmpty())
        {
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        }

        server->createServerOnPort(QHostAddress(ipAddress), static_cast<quint16>(ui->sb_port->value()));
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_disconnect(void)
{
    emit trace(Q_FUNC_INFO);
    if(server)
    {
        server->closeServer();
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_get_data(const QByteArray &data)
{
    emit trace("get_data");
    emit trace(data.data());
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
