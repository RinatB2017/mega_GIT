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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_thread_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_thread_mainbox.hpp"
#include "mythread.hpp"
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
    thread_start();

    ui->frame->setFixedSize(430, 125);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_set_clock = add_button(testbar,
                                            new QToolButton(this),
                                            qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                            "set clock",
                                            "set clock");

    QToolButton *btn_exit = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaStop),
                                       "stop",
                                       "stop");

    connect(btn_set_clock,  SIGNAL(clicked()), this, SLOT(set_time()));
    connect(btn_exit,       SIGNAL(clicked()), this, SLOT(stop()));
}
//--------------------------------------------------------------------------------
void MainBox::thread_start(void)
{
    emit info("thread_is_started");

    thread = new QThread;

    worker = new MyThread;
    connect(worker, SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(worker, SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(worker, SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(worker, SIGNAL(trace(QString)),     this, SIGNAL(trace(QString)));

    connect(worker, SIGNAL(set_hour(unsigned char)),    this,   SLOT(set_hour(unsigned char)));
    connect(worker, SIGNAL(set_min(unsigned char)),     this,   SLOT(set_min(unsigned char)));
    connect(worker, SIGNAL(set_sec(unsigned char)),     this,   SLOT(set_sec(unsigned char)));

    connect(thread, SIGNAL(started()),  worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, SIGNAL(finished()), this, SLOT(thread_is_finished()));

    worker->moveToThread(thread);

    thread->start();

    //---
    worker->set_time(QDateTime::currentDateTime());
}
//--------------------------------------------------------------------------------
void MainBox::thread_is_finished(void)
{
    emit info("thread_is_finished");
    qApp->exit();
}
//--------------------------------------------------------------------------------
void MainBox::set_time(void)
{
    QDateTime dt = QDateTime::currentDateTime();
    worker->set_time(dt);
}
//--------------------------------------------------------------------------------
void MainBox::stop(void)
{
    worker->stop();
}
//--------------------------------------------------------------------------------
void MainBox::set_hour(unsigned char hour)
{
    ui->lcd_hour->display(QString("%1").arg(hour, 2, 10, QChar('0')));
}
//--------------------------------------------------------------------------------
void MainBox::set_min(unsigned char min)
{
    ui->lcd_min->display(QString("%1").arg(min, 2, 10, QChar('0')));
}
//--------------------------------------------------------------------------------
void MainBox::set_sec(unsigned char sec)
{
    ui->lcd_sec->display(QString("%1").arg(sec, 2, 10, QChar('0')));
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
