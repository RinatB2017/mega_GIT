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
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "wifi_frame.hpp"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
#include "sleeper.h"
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

    wf = new WIFI_frame("WiFi", false, this);
    wf->setFixedSize(wf->sizeHint());
    wf->show();

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(wf);
    setLayout(vbox);

    createTestBar();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test,   SIGNAL(clicked()),  this,   SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
    bool ok = false;

#if 0
    QByteArray ba;
    ba.clear();

    ba.append("at+netmode=1\r");
    ba.append("at+dhcpc=1\r");
    ba.append("at+remoteip=192.168.0.10\r");
    ba.append("at+remoteport=8080\r");
    ba.append("at+remotepro=tcp\r");
    ba.append("at+timeout=0\r");
    ba.append("at+mode=server\r");
    ba.append("at+uart=9600,8,n,1\r");
    ba.append("at+uartpacklen=64\r");
    ba.append("at+uartpacktimeout=10\r");
    ba.append("at+net_commit=1\r");
    ba.append("at+reconn=1\r");
#endif

    ok = wf->send_at_command("at+ver=?\r");
    if(ok)  emit info("OK");

#if 1
    ok = wf->send_at_command("at+netmode=1\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+dhcpc=1\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+remoteip=192.168.0.10\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+remoteport=8080\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+remotepro=tcp\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+timeout=0\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+mode=server\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+uart=9600,8,n,1\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+uartpacklen=64\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+uartpacktimeout=10\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+net_commit=1\r");
    if(ok)  emit info("OK");
    ok = wf->send_at_command("at+reconn=1\r");
    if(ok)  emit info("OK");
#endif
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    emit info("test2");

#if 0
    QByteArray ba;
    ba.clear();
    //ba.append("at+mode=?\r");
    //ba.append("at+net_ip=?\r");
    //ba.append("at+ver=?\r");
    ba.append("at+wifi_conf=?\r");

    data_rs232.clear();
    is_ready = false;
    emit send(ba);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
