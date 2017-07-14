/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QApplication>
#include <QPushButton>
#include <QSpinBox>
#include <QStyle>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "applications/gqrx/mainwindow.h"
//--------------------------------------------------------------------------------
#include "scan_toolbar.hpp"
//--------------------------------------------------------------------------------
Scan_ToolBar::Scan_ToolBar(QWidget *parent) :
    QToolBar(parent)
{
    //---
    mw = dynamic_cast<MainWindow *>(parent);
    if(mw == nullptr)
    {
        qDebug() << "mw is null";
        return;
    }
    //---

    //---
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //---

    QLabel *lbl_begin_freq = new QLabel(this);
    lbl_begin_freq->setText("Начальная частота, kHz:");

    QLabel *lbl_end_freq = new QLabel(this);
    lbl_end_freq->setText("Конечная частота, kHz:");

    QLabel *lbl_step_freq = new QLabel(this);
    lbl_step_freq->setText("Шаг частоты, kHz:");

    QLabel *lbl_interval_timer = new QLabel(this);
    lbl_interval_timer->setText("Интервал таймера, msec:");

    sb_begin_freq = new QSpinBox(this);
    sb_begin_freq->setRange(25 * 1000, 1600 * 1000);
    sb_begin_freq->setValue(25 * 1000);

    sb_end_freq = new QSpinBox(this);
    sb_end_freq->setRange(25 * 1000, 1600 * 1000);
    sb_end_freq->setValue(1600 * 1000);

    sb_step_freq = new QSpinBox(this);
    sb_step_freq->setRange(1, 1000);

    sb_interval_timer = new QSpinBox(this);
    sb_interval_timer->setRange(1000, 100 * 1000);

    btn_scan_start = new QPushButton(this);
    btn_scan_start->setObjectName("btn_scan_start");
    btn_scan_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    btn_scan_start->setToolTip("Start");

    btn_scan_stop = new QPushButton(this);
    btn_scan_stop->setObjectName("btn_scan_stop");
    btn_scan_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    btn_scan_stop->setToolTip("Stop");

    btn_scan_test = new QPushButton(this);
    btn_scan_test->setObjectName("btn_scan_stop");
    btn_scan_test->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));
    btn_scan_test->setToolTip("Test");

    addWidget(lbl_begin_freq);
    addWidget(sb_begin_freq);
    addWidget(lbl_end_freq);
    addWidget(sb_end_freq);
    addWidget(lbl_step_freq);
    addWidget(sb_step_freq);
    addWidget(lbl_interval_timer);
    addWidget(sb_interval_timer);
    addWidget(btn_scan_start);
    addWidget(btn_scan_stop);
    addWidget(btn_scan_test);

    connect(btn_scan_start, SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(btn_scan_stop,  SIGNAL(clicked(bool)),  this,   SLOT(stop()));
    connect(btn_scan_test,  SIGNAL(clicked(bool)),  this,   SLOT(test()));

    btn_scan_start->setEnabled(true);
    btn_scan_stop->setEnabled(false);
}
//--------------------------------------------------------------------------------
Scan_ToolBar::~Scan_ToolBar()
{
    if(timer)
    {
        timer->stop();
        timer->deleteLater();
    }

    sb_begin_freq->deleteLater();
    sb_end_freq->deleteLater();
    sb_step_freq->deleteLater();
    sb_interval_timer->deleteLater();

    btn_scan_start->deleteLater();
    btn_scan_stop->deleteLater();
    btn_scan_test->deleteLater();
}
//--------------------------------------------------------------------------------
void Scan_ToolBar::test(void)
{
    if(mw)
    {
        mw->setNewFrequency(sb_begin_freq->value() * 1000);
        //mw->sta
    }
}
//--------------------------------------------------------------------------------
void Scan_ToolBar::start(void)
{
    qDebug() << "start";
    if(timer)
    {
        int interval_msec = sb_interval_timer->value();
        timer->stop();
        timer->start(interval_msec);

        sb_begin_freq->setDisabled(true);
        sb_end_freq->setDisabled(true);
        sb_step_freq->setDisabled(true);
        sb_interval_timer->setDisabled(true);

        btn_scan_start->setEnabled(false);
        btn_scan_stop->setEnabled(true);

        curr_freq = sb_begin_freq->value() * 1000;
    }
}
//--------------------------------------------------------------------------------
void Scan_ToolBar::stop(void)
{
    qDebug() << "stop";
    if(timer)
    {
        timer->stop();

        sb_begin_freq->setEnabled(true);
        sb_end_freq->setEnabled(true);
        sb_step_freq->setEnabled(true);
        sb_interval_timer->setEnabled(true);

        btn_scan_start->setEnabled(true);
        btn_scan_stop->setEnabled(false);
    }
}
//--------------------------------------------------------------------------------
void Scan_ToolBar::update(void)
{
    qDebug() << "update";
    if(mw)
    {
        mw->setNewFrequency(curr_freq);
        if(curr_freq <= (sb_end_freq->value() * 1000))
        {
            curr_freq += (sb_step_freq->value() * 1000);
        }
    }
}
//--------------------------------------------------------------------------------
