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
#include "modbus_server.hpp"
#include "modbus_client.hpp"
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
    setMinimumHeight(sizeHint().height());

    MODBUS_client *client = new MODBUS_client(this);
    MODBUS_server *server = new MODBUS_server(this);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(client);
    vbox->addWidget(server);
    vbox->addStretch(1);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = (MainWindow *)topLevelWidget();
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    CMD command;
    command.cmd = ID_TEST_0; command.cmd_text = "test 0";   command.func = &MainBox::test_0;  commands.append(command);
    command.cmd = ID_TEST_1; command.cmd_text = "test 1";   command.func = &MainBox::test_1;  commands.append(command);
    command.cmd = ID_TEST_2; command.cmd_text = "test 2";   command.func = &MainBox::test_2;  commands.append(command);
    command.cmd = ID_TEST_3; command.cmd_text = "test 3";   command.func = &MainBox::test_3;  commands.append(command);
    command.cmd = ID_TEST_4; command.cmd_text = "test 4";   command.func = &MainBox::test_4;  commands.append(command);
    command.cmd = ID_TEST_5; command.cmd_text = "test 5";   command.func = &MainBox::test_5;  commands.append(command);
    command.cmd = ID_TEST_6; command.cmd_text = "test 6";   command.func = 0;    commands.append(command);

    QToolBar *toolBar = new QToolBar("testbar");
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
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
void MainBox::test_0(void)
{
    emit info("Test_0()");
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("Test_5()");
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
#ifdef QT_DEBUG
int MainBox::xxx(int x)
{
    return x+5;
}
#endif
//--------------------------------------------------------------------------------
