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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_SerialBox5_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "test_SerialBox5_mainbox.hpp"
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

    init_serial_widgets();
    load_widgets();

    //---
#ifdef FAKE
    QTimer::singleShot(0, [this]{
        MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
        if(mw)
        {
            QList<QDockWidget *> alldw = mw->findChildren<QDockWidget *>();
            foreach (QDockWidget *dock, alldw)
            {
                QList<Worker_fake *> alldocs = dock->findChildren<Worker_fake *>();
                if(alldocs.count() > 0)
                {
                    //emit info(alldocs.at(0)->objectName());
                    dock->setWindowTitle(QString("LOG: %1").arg(alldocs.at(0)->objectName()));
                }
            }
        }
    });
#endif
    //---
}
//--------------------------------------------------------------------------------
void MainBox::init_serial_widgets(void)
{
    ui->serial_widget->set_caption("RS-232");

    ui->serial_widget_fix_baudrate->set_fix_baudrate(BAUDRATE);
    ui->serial_widget_fix_baudrate_win7->set_fix_baudrate(BAUDRATE);

    connect(ui->serial_widget,                      &SerialBox5::output,                    this,   &MainBox::serial_data);
    connect(ui->serial_widget_lite,                 &SerialBox5_lite::output,               this,   &MainBox::serial_data);
    connect(ui->serial_widget_fix_baudrate,         &SerialBox5_fix_baudrate::output,       this,   &MainBox::serial_data);
    connect(ui->serial_widget_fix_baudrate_win7,    &SerialBox5_fix_baudrate_win7::output,  this,   &MainBox::serial_data);
    connect(ui->serial_widget_wo_form,              &SerialBox5_wo_form::output,            this,   &MainBox::serial_data);

    ui->btn_serial_widget->setDisabled(true);
    ui->btn_serial_widget_lite->setDisabled(true);
    ui->btn_serial_widget_fix_baudrate->setDisabled(true);
    ui->btn_serial_widget_fix_baudrate_win7->setDisabled(true);
    ui->btn_serial_widget_wo_form->setDisabled(true);

    connect(ui->btn_serial_widget,                      &QToolButton::clicked,  this,   &MainBox::test_only);
    connect(ui->btn_serial_widget_lite,                 &QToolButton::clicked,  this,   &MainBox::test_only);
    connect(ui->btn_serial_widget_fix_baudrate,         &QToolButton::clicked,  this,   &MainBox::test_only);
    connect(ui->btn_serial_widget_fix_baudrate_win7,    &QToolButton::clicked,  this,   &MainBox::test_only);

    connect(ui->btn_serial_widget_wo_form,              &QToolButton::clicked,  this,   &MainBox::test_send);

    connect(ui->serial_widget,                      &SerialBox5::port_is_active,                    ui->btn_serial_widget,                      &QToolButton::setEnabled);
    connect(ui->serial_widget_lite,                 &SerialBox5_lite::port_is_active,               ui->btn_serial_widget_lite,                 &QToolButton::setEnabled);
    connect(ui->serial_widget_fix_baudrate,         &SerialBox5_fix_baudrate::port_is_active,       ui->btn_serial_widget_fix_baudrate,         &QToolButton::setEnabled);
    connect(ui->serial_widget_fix_baudrate_win7,    &SerialBox5_fix_baudrate_win7::port_is_active,  ui->btn_serial_widget_fix_baudrate_win7,    &QToolButton::setEnabled);
    connect(ui->serial_widget_wo_form,              &SerialBox5_wo_form::port_is_active,            ui->btn_serial_widget_wo_form,              &QToolButton::setEnabled);

    ui->btn_sb_wo_form->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    connect(ui->btn_sb_wo_form, &QToolButton::toggled,  this,   &MainBox::test_open);

    //TODO
#if 1
    ui->serial_widget_fix_baudrate_win7->setVisible(false);
    ui->btn_serial_widget_fix_baudrate_win7->setVisible(false);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::test_only(void)
{
    messagebox_info("Info", "test_only");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test,   SIGNAL(clicked()),  this,   SLOT(test()));
    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::serial_data(QByteArray data)
{
    if(data.isEmpty())
    {
        return;
    }
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::test_open(bool state)
{
    if(state)
        ui->serial_widget_wo_form->serial_open();
    else
        ui->serial_widget_wo_form->serial_close();

    if(ui->serial_widget_wo_form->isOpen())
        ui->btn_sb_wo_form->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    else
        ui->btn_sb_wo_form->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
}
//--------------------------------------------------------------------------------
void MainBox::test_send(void)
{
    if(ui->serial_widget_wo_form->isOpen())
    {
        emit info("send data to serialbox_wo_form");
        ui->serial_widget_wo_form->input(QString("send_wo_form"));
    }
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
