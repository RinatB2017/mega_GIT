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

    connect(ui->serial_widget_0,    &SerialBox5::output,    ui->serial_widget_1,    static_cast<int (SerialBox5::*)(const QByteArray &)>(&SerialBox5::input));
    connect(ui->serial_widget_1,    &SerialBox5::output,    ui->serial_widget_0,    static_cast<int (SerialBox5::*)(const QByteArray &)>(&SerialBox5::input));

    connect(ui->serial_widget_0,    &SerialBox5::output,    this,   &MainBox::serial_log);
    connect(ui->serial_widget_1,    &SerialBox5::output,    this,   &MainBox::serial_log);

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
                    dock->setWindowTitle(QString("LOG: %1").arg(alldocs.at(0)->objectName()));
                }
            }
        }
    });
#endif
    //---

    connect(this,   &MainBox::info,     ui->logbox_widget,  &LogBox::infoLog);
    connect(this,   &MainBox::debug,    ui->logbox_widget,  &LogBox::debugLog);
    connect(this,   &MainBox::error,    ui->logbox_widget,  &LogBox::errorLog);
    connect(this,   &MainBox::trace,    ui->logbox_widget,  &LogBox::traceLog);

    load_widgets();
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
void MainBox::serial_log(const QByteArray &data)
{
    SerialBox5 *serial = static_cast<SerialBox5 *>(sender());
    QString text = QString();
    if(serial == ui->serial_widget_0)
    {
        if(ui->rb_hex->isChecked())
        {
            text = QString("Serial0: %1").arg(data.toHex().data());
        }
        else
        {
            text = QString("Serial0: %1").arg(data.data());
        }
    }
    if(serial == ui->serial_widget_1)
    {
        if(ui->rb_hex->isChecked())
        {
            text = QString("Serial1: %1").arg(data.toHex().data());
        }
        else
        {
            text = QString("Serial1: %1").arg(data.data());
        }
    }

    if(text.isEmpty() == false)
    {
        emit info(text);
    }
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
