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
#include <QToolButton>
#include <QObject>
#include <QToolBar>
#include <QThread>
//--------------------------------------------------------------------------------
#include "ui_test_broadcast_udp_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_broadcast_udp_mainbox.hpp"
//--------------------------------------------------------------------------------
#include "server.hpp"
#include "client.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
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
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = (MainWindow *)parentWidget();
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
bool MainBox::create_server(void)
{
    QThread *thread_server = new QThread;

    Server *server = new Server;
    connect(server, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(server, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(server, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(server, SIGNAL(message(QString)),   this, SIGNAL(message(QString)));

    connect(server, SIGNAL(finished()), thread_server, SLOT(quit()));
    connect(thread_server, SIGNAL(finished()), thread_server, SLOT(deleteLater()));

    connect(thread_server, SIGNAL(started()),  server, SLOT(process()));
    connect(server, SIGNAL(finished()), server, SLOT(deleteLater()));

    connect(thread_server, SIGNAL(finished()), this, SLOT(server_thread_is_finished()));

    server->moveToThread(thread_server);

    thread_server->start();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::create_client(void)
{
    QThread *thread_client = new QThread;

    Client *client = new Client;
    connect(client, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(client, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(client, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(client, SIGNAL(message(QString)),   this, SIGNAL(message(QString)));

    connect(client, SIGNAL(finished()), thread_client, SLOT(quit()));
    connect(thread_client, SIGNAL(finished()), thread_client, SLOT(deleteLater()));

    connect(thread_client, SIGNAL(started()),  client, SLOT(process()));
    connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));

    connect(thread_client, SIGNAL(finished()), this, SLOT(client_thread_is_finished()));

    client->moveToThread(thread_client);

    thread_client->start();
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
#if 0
    QToolButton *btn = (QToolButton *)sender();
    if(btn)
    {
        btn->setDisabled(true);
    }
#endif

    emit info("test started");
    bool ok = false;
#if 0
    ok = create_server();
    if(!ok)
    {
        emit error("error create server");
    }
#endif
#if 1
    ok = create_client();
    if(!ok)
    {
        emit error("error create client");
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::server_thread_is_finished(void)
{
    emit info("Server is finished");
}
//--------------------------------------------------------------------------------
void MainBox::client_thread_is_finished(void)
{
    emit info("Client is finished");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
