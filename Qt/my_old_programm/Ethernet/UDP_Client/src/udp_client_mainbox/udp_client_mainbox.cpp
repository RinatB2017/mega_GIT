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
#include <QToolButton>
#include <QCheckBox>
#include <QtNetwork>
#include <QToolBar>
#include <QThread>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "ui_udp_client_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "udp_client_mainbox.hpp"
#include "client.hpp"
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
    ui->setupUi(this);

    createTestBar();
#if 0
    create_slow_client();
#else
    create_fast_client();
#endif
}
//--------------------------------------------------------------------------------
void MainBox::create_slow_client(void)
{
    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    messageNo = 1;

    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
}
//--------------------------------------------------------------------------------
void MainBox::create_fast_client(void)
{
    QThread *thread_client = new QThread;

    Client *client = new Client;
    connect(client, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(client, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(client, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(client, SIGNAL(trace(QString)),     this, SIGNAL(trace(QString)));

    connect(client, SIGNAL(finished()), thread_client, SLOT(quit()));
    connect(thread_client, SIGNAL(finished()), thread_client, SLOT(deleteLater()));

    connect(thread_client, SIGNAL(started()),  client, SLOT(process()));
    connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));

    connect(thread_client, SIGNAL(finished()), this, SLOT(client_thread_is_finished()));

    client->moveToThread(thread_client);

    thread_client->start();
}
//--------------------------------------------------------------------------------
void MainBox::client_thread_is_finished(void)
{
    emit info("client_thread_is_finished");
}
//--------------------------------------------------------------------------------
void MainBox::startBroadcasting(void)
{
    startButton->setEnabled(false);
    if(!timer)
    {
        emit error("no init timer");
        return;
    }
    timer->start(1000);
}
//--------------------------------------------------------------------------------
void MainBox::broadcastDatagram(void)
{
    emit info(QString("Now broadcasting datagram %1").arg(messageNo));
    QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, PORT);
    ++messageNo;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    startButton = add_button(testbar,
                             new QToolButton(this),
                             qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                             "startBroadcasting",
                             "startBroadcasting");
    QToolButton *start_fast_client = add_button(testbar,
                                                new QToolButton(this),
                                                qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                                "start fast client",
                                                "start fast client");

    connect(startButton, SIGNAL(clicked()), this, SLOT(startBroadcasting()));
    connect(start_fast_client, SIGNAL(clicked()), this, SLOT(create_fast_client()));
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
