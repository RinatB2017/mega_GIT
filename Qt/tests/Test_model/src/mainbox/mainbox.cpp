/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QStandardItemModel>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
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

    model = new QStandardItemModel(0, 1, this);
    model->setHeaderData(0, Qt::Horizontal, tr("User"));

    ui->tableView->setModel(model);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    CMD command;
    command.cmd = 0x10; command.cmd_text = "add user_0";   command.func = &MainBox::test_0;  commands.append(command);
    command.cmd = 0x20; command.cmd_text = "add user_1";   command.func = &MainBox::test_1;  commands.append(command);
    command.cmd = 0x30; command.cmd_text = "add user_2";   command.func = &MainBox::test_2;  commands.append(command);
    command.cmd = 0x40; command.cmd_text = "add user_3";   command.func = &MainBox::test_3;  commands.append(command);
    command.cmd = 0x50; command.cmd_text = "add user_4";   command.func = &MainBox::test_4;  commands.append(command);
    command.cmd = 0x60; command.cmd_text = "add user_5";   command.func = &MainBox::test_5;  commands.append(command);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef void (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
QString MainBox::convert_int_to_string(uint32_t value)
{
    if(value >= 1e6)
    {
        return QString("%1 млн.").arg((double)value / (double)1e6, 0, 'f', 3);
    }
    if(value >= 1e3)
    {
        return QString("%1 тыс.").arg((double)value / (double)1e3, 0, 'f', 3);
    }
    return QString("%1").arg(value);
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    model->insertRow(0);
    model->setData(model->index(0, 0, QModelIndex()), "user_0");
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    model->insertRow(0);
    model->setData(model->index(0, 0, QModelIndex()), "user_1");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    model->insertRow(0);
    model->setData(model->index(0, 0, QModelIndex()), "user_2");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    model->insertRow(0);
    model->setData(model->index(0, 0, QModelIndex()), "user_3");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    model->insertRow(0);
    model->setData(model->index(0, 0, QModelIndex()), "user_4");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    model->insertRow(0);
    model->setData(model->index(0, 0, QModelIndex()), "user_5");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
int MainBox::xxx(int x)
{
    return x+5;
}
//--------------------------------------------------------------------------------
