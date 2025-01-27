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
#include "ui_rs232_5_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "rs232_5_mainbox.hpp"
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

#if 0
    serial_widget = new SerialBox5(this, "RS232_5", "RS232_5");
    serial_widget->add_menu(2);
    connect(this,           SIGNAL(send(QByteArray)),   serial_widget,  SLOT(input(QByteArray)));
    connect(serial_widget,  SIGNAL(output(QByteArray)), this,           SLOT(read_data(QByteArray)));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(serial_widget);
    setLayout(vbox);
#else
    ui->serial_widget->set_caption("RS232_5");
    ui->serial_widget->add_menu(2);
    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));
#endif

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    ui->serial_widget->set_caption("RS232_5");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit info(ba.data());
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    if(ui->serial_widget->isOpen())
    {
        messagebox_critical("Ошибка", "Сначала закройте порт!");
        return false;
    }
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
