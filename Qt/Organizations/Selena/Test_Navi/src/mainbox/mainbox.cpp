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
#include "serialbox.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#include "proto_NMEA_0183.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox(0),
    test_byte(0),
    cnt(0)
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

    init_protocol();

    createTestBar();

    serialBox = new SerialBox(this, "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::init_protocol(void)
{
    emit info("init protocol NMEA-0183");
    proto = new Proto_NMEA_0183();

    connect(proto,  SIGNAL(output_latitude_string(QString)),    this,   SIGNAL(output_latitude_string(QString)));
    connect(proto,  SIGNAL(output_longitude_string(QString)),   this,   SIGNAL(output_longitude_string(QString)));
    connect(proto,  SIGNAL(output_observation(QString)),        this,   SIGNAL(output_observation(QString)));

    connect(this,   SIGNAL(output_latitude_string(QString)),    this,   SIGNAL(message(QString)));
    connect(this,   SIGNAL(output_longitude_string(QString)),   this,   SIGNAL(message(QString)));
    connect(this,   SIGNAL(output_observation(QString)),        this,   SIGNAL(message(QString)));
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolBar *toolBar = new QToolBar(tr("testbar"));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info(tr("reset"));
    cnt=0;
    test_byte=0;
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        if(is_ready)
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug(ba.data());
    data_rs232.append(ba);
    is_ready = true;

    //emit trace(ba);
    int err = proto->check_message(ba.data());
    proto->print_error(err);
#if 0
    proto->print_variable();
    proto->test_cheksum();
#endif
#if 0
    emit info(QString("latitude %1").arg(proto->get_latitude_string()));
#endif
#if 0
    emit info(QString("longitude %1").arg(proto->get_longitude_string()));
#endif
#if 1
    int hour = proto->get_observation_hour();
    int min  = proto->get_observation_min();
    int sec  = proto->get_observation_sec();
    if((hour !=0) || (min != 0) || (sec != 0))
    {
        emit info(QString("%1:%2:%3")
                  .arg(hour)
                  .arg(min)
                  .arg(sec));
    }
#endif
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
