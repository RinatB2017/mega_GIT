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
#include <QComboBox>
#include <QToolBar>
#include <QThread>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
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
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_refresh_ports = new QToolButton;
    btn_refresh_ports->setObjectName("btn_refresh_ports");
    btn_refresh_ports->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    connect(btn_refresh_ports,  SIGNAL(clicked(bool)),  this,   SLOT(refresh_ports()));

    cb_port = new QComboBox(this);
    cb_port->setMinimumWidth(200);

    toolBar->addWidget(btn_refresh_ports);
    toolBar->addWidget(cb_port);

    QToolButton *btn_start = add_button(toolBar,
                                        new QToolButton(this),
                                        qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                        "start",
                                        "start");

    QToolButton *btn_stop = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaStop),
                                       "stop",
                                       "stop");

    connect(btn_start,  SIGNAL(clicked()), this, SLOT(thread_start()));
    connect(btn_stop,   SIGNAL(clicked()), this, SLOT(thread_stop()));

    toolBar->setFixedWidth(toolBar->sizeHint().width());
    refresh_ports();
}
//--------------------------------------------------------------------------------
void MainBox::refresh_ports(void)
{
    cb_port->clear();
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
#ifdef Q_OS_LINUX
        cb_port->addItem(port.systemLocation());
#endif
#ifdef Q_OS_WIN
        cb_port->addItem(port.portName());
#endif
    }
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

    connect(thread, SIGNAL(started()),  worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, SIGNAL(finished()), this, SLOT(thread_is_finished()));

    worker->set_portname(cb_port->currentText());

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
}
//--------------------------------------------------------------------------------
void MainBox::thread_stop(void)
{
    if(worker)
    {
        worker->stop();
    }
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
