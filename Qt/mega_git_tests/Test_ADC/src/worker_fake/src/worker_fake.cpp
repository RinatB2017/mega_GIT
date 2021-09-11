/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
#include "mainwindow.hpp"
#include "worker_fake.hpp"
#include "ui_worker_fake.h"
//--------------------------------------------------------------------------------
Worker_fake::Worker_fake(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Worker_fake)
{
    init();;
}
//--------------------------------------------------------------------------------
Worker_fake::~Worker_fake()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker_fake::init(void)
{
    ui->setupUi(this);

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_test->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));

    timer = new QTimer(this);
    connect(timer,  &QTimer::timeout,   this,   &Worker_fake::update);

    connect(ui->btn_start,  &QToolButton::clicked,  this,   &Worker_fake::start);
    connect(ui->btn_stop,   &QToolButton::clicked,  this,   &Worker_fake::stop);
    connect(ui->btn_test,   &QToolButton::clicked,  this,   &Worker_fake::test);

    ui->sb_interval->setRange(100, 10000);

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("FAKE log", "fake_log_dock",  Qt::BottomDockWidgetArea, this);
    }
#endif
}
//--------------------------------------------------------------------------------
void Worker_fake::input(QByteArray data)
{
    Q_UNUSED(data)
}
//--------------------------------------------------------------------------------
void Worker_fake::update(void)
{
    int a = 100;
    int b = 50;
    QString temp = QString(":data|%1|%2|%3|%4|%5|%6")
            .arg(static_cast<qreal>(rand() % a - b))
            .arg(static_cast<qreal>(rand() % a - b))
            .arg(static_cast<qreal>(rand() % a - b))
            .arg(static_cast<qreal>(rand() % a - b))
            .arg(static_cast<qreal>(rand() % a - b))
            .arg(static_cast<qreal>(rand() % a - b));

    QByteArray data;
    data.append(temp.toLatin1());
    data.append(static_cast<char>(0x0A));

    emit output(data);
    //ui->log_widget->errorLog(data);
}
//--------------------------------------------------------------------------------
void Worker_fake::port_open(void)
{
#ifdef Q_DEBUG
    qDebug() << "port_open";
#endif
    if(timer)
    {
        timer->start(ui->sb_interval->value());
    }
}
//--------------------------------------------------------------------------------
void Worker_fake::port_close(void)
{
#ifdef Q_DEBUG
    qDebug() << "port_close";
#endif
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Worker_fake::readyRead(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Worker_fake::start(void)
{
    emit trace(Q_FUNC_INFO);
    if(timer)
    {
        timer->start(ui->sb_interval->value());
    }
}
//--------------------------------------------------------------------------------
void Worker_fake::stop(void)
{
    emit trace(Q_FUNC_INFO);
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Worker_fake::test(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Worker_fake::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Worker_fake::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Worker_fake::load_setting(void)
{
    int value = 0;
    bool ok = load_int("fake_intervel", &value);
    if(ok)
    {
        ui->sb_interval->setValue(value);
    }
}
//--------------------------------------------------------------------------------
void Worker_fake::save_setting(void)
{
    save_int("fake_intervel", ui->sb_interval->value());
}
//--------------------------------------------------------------------------------
