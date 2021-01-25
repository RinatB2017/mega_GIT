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
#include "ui_test_libusb_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_libusb_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <sys/time.h>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    Usb(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    connect(ui->btn_list,   &QToolButton::clicked,  this,   &MainBox::s_list);
    connect(ui->btn_open,   &QToolButton::clicked,  this,   &MainBox::s_open);
    connect(ui->btn_info,   &QToolButton::clicked,  this,   &MainBox::s_info);
    connect(ui->btn_read,   &QToolButton::clicked,  this,   &MainBox::s_read);
    connect(ui->btn_write,  &QToolButton::clicked,  this,   &MainBox::s_write);
    connect(ui->btn_close,  &QToolButton::clicked,  this,   &MainBox::s_close);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());    //TODO странно
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::s_list(void)
{
    lock_this_button();
    f_list();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_open(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    f_open(get_VID(), get_PID());
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_info(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    print_info();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_read(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    f_read();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_write(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    f_write();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_close(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    f_close();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
uint16_t MainBox::get_VID(void)
{
    return static_cast<uint16_t>(ui->sb_VID->value());
}
//--------------------------------------------------------------------------------
uint16_t MainBox::get_PID(void)
{
    return static_cast<uint16_t>(ui->sb_PID->value());
}
//--------------------------------------------------------------------------------
void MainBox::set_VID(uint16_t value)
{
    ui->sb_VID->setValue(value);
}
//--------------------------------------------------------------------------------
void MainBox::set_PID(uint16_t value)
{
    ui->sb_PID->setValue(value);
}
//--------------------------------------------------------------------------------
