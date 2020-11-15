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
#include "ui_test_at_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5_lite.hpp"
#include "test_at_mainbox.hpp"
#include "defines.hpp"
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

#if 1
    connect(this,           SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,           SLOT(read_data(QByteArray)));
#else
    connect(this,               static_cast<void (MainBox::*)(const QByteArray&)>(&MainBox::send),
            ui->serial_widget,  static_cast<int (SerialBox5_lite::*)(const QByteArray&)>(&SerialBox5_lite::input));
    connect(ui->serial_widget,  &SerialBox5_lite::output,   this,                           &MainBox::read_data);
#endif

    ui->btn_send_SIM800->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_send_SIM900->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_send_SL6087->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    connect(ui->btn_send_SIM800,    &QToolButton::clicked,  this,   &MainBox::send_command_SIM800);
    connect(ui->btn_send_SIM900,    &QToolButton::clicked,  this,   &MainBox::send_command_SIM900);
    connect(ui->btn_send_SL6087,    &QToolButton::clicked,  this,   &MainBox::send_command_SL6087);

    connect(ui->serial_widget,  &SerialBox5_lite::port_is_active,   this,   &MainBox::lock_widgets);

    init_widgets();

    lock_widgets(false);
    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::lock_widgets(bool state)
{
    ui->cb_commands_SIM800->setEnabled(state);
    ui->cb_commands_SIM900->setEnabled(state);
    ui->cb_commands_SL6087->setEnabled(state);

    ui->btn_send_SIM800->setEnabled(state);
    ui->btn_send_SIM900->setEnabled(state);
    ui->btn_send_SL6087->setEnabled(state);
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    sl_commands_SIM800.append( { 1, "", "test"});
    sl_commands_SIM800.append( { 2, "+AT2", "SIM800 cmd2"});
    sl_commands_SIM800.append( { 3, "+AT3", "SIM800 cmd3"});

    sl_commands_SIM900.append( { 1, "", "test"});
    sl_commands_SIM900.append( { 2, "+GSN=?", "get serial number"});

    sl_commands_SL6087.append( { 1, "", "test"});

    foreach (CMD cmd, sl_commands_SIM800)
    {
        ui->cb_commands_SIM800->addItem(cmd.comment, cmd.index);
    }
    foreach (CMD cmd, sl_commands_SIM900)
    {
        ui->cb_commands_SIM900->addItem(cmd.comment, cmd.index);
    }
    foreach (CMD cmd, sl_commands_SL6087)
    {
        ui->cb_commands_SL6087->addItem(cmd.comment, cmd.index);
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw != nullptr);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit info(ba.data());
}
//--------------------------------------------------------------------------------
void MainBox::send_command_SIM800(void)
{
    template_command(ui->cb_commands_SIM800, sl_commands_SIM800);
}
//--------------------------------------------------------------------------------
void MainBox::send_command_SIM900(void)
{
    template_command(ui->cb_commands_SIM900, sl_commands_SIM900);
}
//--------------------------------------------------------------------------------
void MainBox::send_command_SL6087(void)
{
    template_command(ui->cb_commands_SL6087, sl_commands_SL6087);
}
//--------------------------------------------------------------------------------
template<typename T, typename T2>
void MainBox::template_command(T w, T2 sl)
{
    if(ui->serial_widget->isOpen() ==  false)
    {
        emit error("port not open");
        return;
    }
    bool ok = false;
    int index = w->itemData(w->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        emit error("test_template: bad index");
        return;
    }

    QByteArray ba;
    ba.append("AT");
    foreach(CMD cmd, sl)
    {
        if(cmd.index == index)
        {
            ba.append(cmd.cmd.toLatin1());
        }
    }
    ba.append(static_cast<char>(0x0D));

    emit error(QString("send [%1]").arg(ba.data()));
    emit send(ba);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
