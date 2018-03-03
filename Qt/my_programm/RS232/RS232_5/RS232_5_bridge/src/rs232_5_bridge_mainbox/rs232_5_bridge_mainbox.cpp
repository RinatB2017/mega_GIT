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
#   include <QWidget>
#else
#   include <QVBoxLayout>
#   include <QMessageBox>
#   include <QTime>

#   include <QAction>
#   include <QMenu>

#   include <QToolButton>
#   include <QToolBar>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_rs232_5_bridge_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "rs232_5_bridge_mainbox.hpp"
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

    ui->serial0->set_caption("RS232 (0)");
    ui->serial1->set_caption("RS232 (1)");

    ui->serial0->add_menu(1, "RS232 (0)");
    ui->serial1->add_menu(2, "RS232 (1)");

#if 1
    connect(ui->serial0,    SIGNAL(output(QByteArray)), ui->log0,   SLOT(bappend(QByteArray)));
    connect(ui->serial1,    SIGNAL(output(QByteArray)), ui->log1,   SLOT(bappend(QByteArray)));
#endif

    connect(ui->serial0,    SIGNAL(output(QByteArray)), ui->serial1,    SLOT(input(QByteArray)));
    connect(ui->serial1,    SIGNAL(output(QByteArray)), ui->serial0,    SLOT(input(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::send_data_1(QByteArray ba)
{
    emit debug(QString("send_data_1: %1").arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::send_data_2(QByteArray ba)
{
    emit debug(QString("send_data_2: %1").arg(ba.toHex().data()));
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
