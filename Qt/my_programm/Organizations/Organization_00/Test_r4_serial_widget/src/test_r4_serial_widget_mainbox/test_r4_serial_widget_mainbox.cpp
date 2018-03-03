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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "ui_test_r4_serial_widget_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_r4_serial_widget_mainbox.hpp"
#include "worker.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
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
void MainBox::init(void)
{
    createTestBar();

    init_thread();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    Q_CHECK_PTR(parent);
    MainWindow *mw = (MainWindow *)parent;
    Q_CHECK_PTR(mw);

    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *toolBar = new QToolBar(this);
    toolBar->setObjectName("toolBar");

    toolBar->addWidget(btnTest);

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
