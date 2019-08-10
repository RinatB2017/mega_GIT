/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include "ui_udp_server_mainbox.h"
//--------------------------------------------------------------------------------
#include "udp_server_mainbox.hpp"
#include "defines.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    packets_good = 0;
    packets_bad  = 0;

    udpSocket = new QUdpSocket(this);
    bool ok = udpSocket->bind(PORT, QUdpSocket::ShareAddress);
    if(!ok)
    {
        emit error("bind error!");
        return;
    }

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    emit info(QString("UDP server started on port %1").arg(PORT));
}
//--------------------------------------------------------------------------------
void MainBox::processPendingDatagrams()
{
    datagram.clear();
    while (udpSocket->hasPendingDatagrams())
    {
        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        emit info(QString("Received datagram: %1 bytes").arg(datagram.size()));
    }
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
void MainBox::get_data(const QByteArray &data)
{
    emit debug(data.data());
}
//--------------------------------------------------------------------------------
