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
//--------------------------------------------------------------------------------
#include "ui_navi_receiver_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "navi_receiver_mainbox.hpp"
//--------------------------------------------------------------------------------
#include "proto_NMEA_0183.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    init_protocol();
    read_fake_data();
    createTestBar();

    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::init_protocol(void)
{
    emit info("init protocol NMEA-0183");
    proto = new Proto_NMEA_0183();
    Q_CHECK_PTR(proto);

    connect(proto, SIGNAL(info(QString)),   this,   SIGNAL(info(QString)));
    connect(proto, SIGNAL(debug(QString)),  this,   SIGNAL(debug(QString)));
    connect(proto, SIGNAL(error(QString)),  this,   SIGNAL(error(QString)));
    connect(proto, SIGNAL(trace(QString)),  this,   SIGNAL(trace(QString)));

    connect(proto,  SIGNAL(output_latitude_string(QString)),    this,   SIGNAL(output_latitude_string(QString)));
    connect(proto,  SIGNAL(output_longitude_string(QString)),   this,   SIGNAL(output_longitude_string(QString)));
    connect(proto,  SIGNAL(output_observation(QString)),        this,   SIGNAL(output_observation(QString)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_skip_backward = add_button(testbar,
                                                new QToolButton(this),
                                                qApp->style()->standardIcon(QStyle::SP_MediaSkipBackward),
                                                "<<",
                                                "<<");
    QToolButton *btn_seek_backward = add_button(testbar,
                                                new QToolButton(this),
                                                qApp->style()->standardIcon(QStyle::SP_MediaSeekBackward),
                                                "<",
                                                "<");

    le_index = new QLineEdit(this);
    le_index->setReadOnly(true);
    le_index->setText("0");
    le_index->setFixedWidth(100);
    testbar->addWidget(le_index);

    QToolButton *btn_seek_forward = add_button(testbar,
                                               new QToolButton(this),
                                               qApp->style()->standardIcon(QStyle::SP_MediaSeekForward),
                                               ">",
                                               ">");
    QToolButton *btn_skip_forward = add_button(testbar,
                                               new QToolButton(this),
                                               qApp->style()->standardIcon(QStyle::SP_MediaSkipForward),
                                               "<<",
                                               "<<");

    connect(btn_skip_backward,  SIGNAL(clicked(bool)),  this,   SLOT(skip_backward()));
    connect(btn_seek_backward,  SIGNAL(clicked(bool)),  this,   SLOT(seek_backward()));
    connect(btn_seek_forward,   SIGNAL(clicked(bool)),  this,   SLOT(seek_forward()));
    connect(btn_skip_forward,   SIGNAL(clicked(bool)),  this,   SLOT(skip_forward()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info(tr("test"));

    read_fake_data();
}
//--------------------------------------------------------------------------------
void MainBox::skip_backward(void)
{
    if(fake_data.isEmpty())
    {
        emit error("no data");
        return;
    }

    index_fake_data = 0;
    le_index->setText(QString("%1").arg(index_fake_data));
    read_data(fake_data.at(index_fake_data));
}
//--------------------------------------------------------------------------------
void MainBox::seek_backward(void)
{
    if(fake_data.isEmpty())
    {
        emit error("no data");
        return;
    }

    index_fake_data--;
    if(index_fake_data < 0)
        index_fake_data = 0;
    le_index->setText(QString("%1").arg(index_fake_data));
    read_data(fake_data.at(index_fake_data));
}
//--------------------------------------------------------------------------------
void MainBox::seek_forward(void)
{
    if(fake_data.isEmpty())
    {
        emit error("no data");
        return;
    }

    index_fake_data++;
    if(index_fake_data > (fake_data.count() - 1))
        index_fake_data = fake_data.count() - 1;
    le_index->setText(QString("%1").arg(index_fake_data));
    read_data(fake_data.at(index_fake_data));
}
//--------------------------------------------------------------------------------
void MainBox::skip_forward(void)
{
    if(fake_data.isEmpty())
    {
        emit error("no data");
        return;
    }

    index_fake_data = fake_data.count() - 1;
    le_index->setText(QString("%1").arg(index_fake_data));
    read_data(fake_data.at(index_fake_data));
}
//--------------------------------------------------------------------------------
void MainBox::read_fake_data(void)
{
    QFile file(":/test_data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    fake_data.clear();
    index_fake_data = 0;

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        fake_data.append(line);
    }
    max_index_fake_data = fake_data.count();
    emit info(QString("Found %1 record").arg(max_index_fake_data));
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug(ba.data());

    for (int n=0; n<ba.length(); n++)
    {
        char s = ba.at(n);
        switch (s)
        {
        case '$':
            data_rs232.clear();
            break;

        case '\r':
        case '\n':
            analize();
            break;

        default:
            data_rs232.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::analize(void)
{
    int err = proto->check_message(data_rs232.data());
    if(err == Proto_NMEA_0183::E_ERROR_UNKNOWN_MESSAGE)
    {
        proto->print_error(data_rs232, err);
        return;
    }
    if(err != Proto_NMEA_0183::E_NO_ERROR)
    {
        proto->print_error(data_rs232, err);
        return;
    }
#if 0
    proto->print_variable();
    proto->test_cheksum();
#endif

    int hour = proto->get_observation_hour();
    int min  = proto->get_observation_min();
    int sec  = proto->get_observation_sec();

    QString str_latitude = proto->get_latitude_string();
    QString str_longitude = proto->get_longitude_string();
    QString str_time = QString("%1:%2:%3")
            .arg(hour,  2,  10, QChar('0'))
            .arg(min,   2,  10, QChar('0'))
            .arg(sec,   2,  10, QChar('0'));

    emit info(QString("latitude %1").arg(str_latitude));
    emit info(QString("longitude %1").arg(str_longitude));
    emit info(QString("time %1").arg(str_time));

    ui->le_latitude->setText(str_latitude);
    ui->le_longitude->setText(str_longitude);
    ui->le_time->setText(str_time);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
