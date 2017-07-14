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
//--------------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QToolButton>
#include <QToolBar>
#include <QWidget>
#include <QThread>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "worker.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
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
void MainBox::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void MainBox::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    connect_log();
    createTestBar();

    init_thread();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *toolBar = new QToolBar(this);

    MainWindow *mw = (MainWindow *)parent;
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->addWidget(btnTest);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::init_thread(void)
{
//    QThread* thread = new QThread;
    Worker* worker = new Worker();
//    worker->moveToThread(thread);
    connect(worker, SIGNAL(info(QString)),  this, SIGNAL(info(QString)));
    connect(worker, SIGNAL(debug(QString)), this, SIGNAL(debug(QString)));
    connect(worker, SIGNAL(error(QString)), this, SIGNAL(error(QString)));

    connect(this, SIGNAL(send(QString)), worker, SLOT(input(QString)));

    connect(worker, SIGNAL(output(QString)), this, SIGNAL(debug(QString)));

//    connect(thread, SIGNAL(started()),  worker, SLOT(process()));
//    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
//    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    thread->start();
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit send("00010203040506070809");
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
