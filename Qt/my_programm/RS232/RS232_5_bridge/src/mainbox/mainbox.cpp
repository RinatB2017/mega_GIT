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
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
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

    serialBox5_1 = new SerialBox5(this, "RS232 (1)", "RS232_1");
    serialBox5_1->add_menu(2);

    serialBox5_2 = new SerialBox5(this, "RS232 (2)", "RS232_2");
    serialBox5_2->add_menu(4);

    ui->serial_layout->addWidget(serialBox5_1);
    ui->serial_layout->addWidget(serialBox5_2);
    ui->serial_layout->addStretch();

#if 1
    connect(serialBox5_1,   SIGNAL(output(QByteArray)), serialBox5_2,   SLOT(input(QByteArray)));
    connect(serialBox5_2,   SIGNAL(output(QByteArray)), serialBox5_1,   SLOT(input(QByteArray)));
#else
    connect(serialBox5_1,   SIGNAL(output(QByteArray)), this,   SLOT(send_data_1(QByteArray)));
    connect(serialBox5_2,   SIGNAL(output(QByteArray)), this,   SLOT(send_data_2(QByteArray)));
#endif
}
//--------------------------------------------------------------------------------
void MainBox::send_data_1(QByteArray ba)
{
    emit info(QString("send_data_1: %1").arg(ba.toHex().data()));
    //serialBox5_2->input(ba);
}
//--------------------------------------------------------------------------------
void MainBox::send_data_2(QByteArray ba)
{
    emit info(QString("send_data_2: %1").arg(ba.toHex().data()));
    //serialBox5_1->input(ba);
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
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
