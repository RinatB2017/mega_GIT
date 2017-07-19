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
#include <QToolBar>
#include <QThread>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "mythread.hpp"
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
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return nullptr;
    if(!tool_button) return nullptr;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_set_clock = add_button(toolBar,
                                            new QToolButton(this),
                                            qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                            "set clock",
                                            "set clock");

    QToolButton *btn_exit = add_button(toolBar,
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
    connect(worker, SIGNAL(message(QString)),   this, SIGNAL(message(QString)));

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
    is_close = false;
}
//--------------------------------------------------------------------------------
void MainBox::thread_is_finished(void)
{
    emit info("thread_is_finished");

    worker = 0;
    thread = 0;

    is_close = true;

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
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
