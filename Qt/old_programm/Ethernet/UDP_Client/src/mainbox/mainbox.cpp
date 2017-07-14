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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "client.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    timer(0)
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
    connect(client, SIGNAL(message(QString)),   this, SIGNAL(message(QString)));

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
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    QToolBar *toolBar = new QToolBar(tr("toolbar"));
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    startButton = add_button(toolBar,
                             new QToolButton(this),
                             qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                             "startBroadcasting",
                             "startBroadcasting");
    QToolButton *start_fast_client = add_button(toolBar,
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
    emit debug("updateText()");
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
