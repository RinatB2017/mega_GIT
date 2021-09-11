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
    init();;
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

#ifdef QT_DEBUG
    createTestBar();
#endif

    setVisible(false);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = (MainWindow *)parentWidget();
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    
    connect(btn_test,   &QToolButton::clicked,  this,   &MainBox::test);
}
//--------------------------------------------------------------------------------
bool MainBox::create_server(void)
{
    QThread *thread_server = new QThread();

    server = new Server();
    connect(server, &Server::info,  this,   &MainBox::info);
    connect(server, &Server::debug, this,   &MainBox::debug);
    connect(server, &Server::error, this,   &MainBox::error);
    connect(server, &Server::trace, this,   &MainBox::trace);

    connect(server,         &Server::finished,  thread_server, &QThread::quit);
    connect(thread_server,  &QThread::finished, thread_server, &QThread::deleteLater);

    connect(thread_server,  &QThread::started,  server, &Server::f_process);
    connect(server,         &Server::finished,  server, &Server::deleteLater);

    connect(thread_server,  &QThread::finished, this,   &MainBox::server_thread_is_finished);

    server->moveToThread(thread_server);

    thread_server->start();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::create_client(void)
{
    QThread *thread_client = new QThread();

    client = new Client();
    connect(client, &Client::info,  this,   &MainBox::info);
    connect(client, &Client::debug, this,   &MainBox::debug);
    connect(client, &Client::error, this,   &MainBox::error);
    connect(client, &Client::trace, this,   &MainBox::trace);

    connect(client,         &Client::finished,  thread_client,  &QThread::quit);
    connect(thread_client,  &QThread::finished, thread_client,  &QThread::deleteLater);

    connect(thread_client,  &QThread::started,  client,     &Client::f_process);
    connect(client,         &Client::finished,  client,     &Client::deleteLater);

    connect(thread_client,  &QThread::finished, this,       &MainBox::client_thread_is_finished);

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
