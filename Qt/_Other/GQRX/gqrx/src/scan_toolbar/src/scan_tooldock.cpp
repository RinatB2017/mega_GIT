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
#include <QStyleFactory>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QSpinBox>
#include <QLabel>
#include <QStyle>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "applications/gqrx/mainwindow.h"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
#include "scan_tooldock.hpp"
//--------------------------------------------------------------------------------
Scan_ToolDock::Scan_ToolDock(QWidget *parent) :
    QDockWidget(parent)
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
    btn_scan_test->setText("Test");
    btn_scan_test->setToolTip("Test");

    btn_scan_prev = new QPushButton(this);
    btn_scan_prev->setObjectName("btn_scan_prev");
    btn_scan_prev->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipBackward));
    btn_scan_prev->setToolTip("prev");

    btn_scan_next = new QPushButton(this);
    btn_scan_next->setObjectName("btn_scan_next");
    btn_scan_next->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipForward));
    btn_scan_next->setToolTip("next");

    log = new LogBox("log", this);
    connect(this,   SIGNAL(info(QString)),  log,    SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)), log,    SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)), log,    SLOT(errorLog(QString)));
    connect(this,   SIGNAL(trace(QString)), log,    SLOT(traceLog(QString)));

    QVBoxLayout *vbox = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(btn_scan_start);
    hbox->addWidget(btn_scan_stop);
    hbox->addStretch(1);
    hbox->addWidget(btn_scan_prev);
    hbox->addWidget(btn_scan_next);

    vbox->addWidget(lbl_begin_freq);
    vbox->addWidget(sb_begin_freq);
    vbox->addWidget(lbl_end_freq);
    vbox->addWidget(sb_end_freq);
    vbox->addWidget(lbl_step_freq);
    vbox->addWidget(sb_step_freq);
    vbox->addWidget(lbl_interval_timer);
    vbox->addWidget(sb_interval_timer);
    vbox->addLayout(hbox);
    vbox->addWidget(log);
    vbox->addWidget(btn_scan_test);

    //vbox->addStretch(1);

    QFrame *frame = new QFrame;
    frame->setLayout(vbox);

    setWidget(frame);

    connect(btn_scan_start, SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(btn_scan_stop,  SIGNAL(clicked(bool)),  this,   SLOT(stop()));
    connect(btn_scan_test,  SIGNAL(clicked(bool)),  this,   SLOT(test()));

    connect(btn_scan_prev,  SIGNAL(clicked(bool)),  this,   SLOT(prev()));
    connect(btn_scan_next,  SIGNAL(clicked(bool)),  this,   SLOT(next()));

    btn_scan_start->setEnabled(true);
    btn_scan_stop->setEnabled(false);
}
//--------------------------------------------------------------------------------
Scan_ToolDock::~Scan_ToolDock()
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
int Scan_ToolDock::messagebox_question(const QString title,
                                       const QString text,
                                       unsigned int width)
{
    //QApplication::setStyle(QStyleFactory::create("Breeze"));

    QMessageBox msgBox;

    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
void Scan_ToolDock::test(void)
{
    int btn = messagebox_question("Установить частоту?",
                                  QString("%1").arg(sb_begin_freq->value() * 1000),
                                  300);
    if(btn != QMessageBox::Yes)
    {
        return;
    }
    if(mw)
    {
        emit info(QString("setNewFrequency = %1").arg(mw->setNewFrequency(sb_begin_freq->value() * 1000)));
    }
}
//--------------------------------------------------------------------------------
void Scan_ToolDock::start(void)
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
void Scan_ToolDock::stop(void)
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
void Scan_ToolDock::update(void)
{
    qDebug() << "update";
    if(mw)
    {
        if(curr_freq < (sb_end_freq->value() * 1000))
        {
            mw->setNewFrequency(curr_freq);
        }
        if(curr_freq <= (sb_end_freq->value() * 1000))
        {
            curr_freq += (sb_step_freq->value() * 1000);
            sb_begin_freq->setValue(curr_freq / 1000);
        }
    }
}
//--------------------------------------------------------------------------------
void Scan_ToolDock::prev(void)
{
    qDebug() << "prev";
    if(mw)
    {
        if(curr_freq >= (sb_begin_freq->value() * 1000))
        {
            curr_freq -= (sb_step_freq->value() * 1000);
            sb_begin_freq->setValue(curr_freq / 1000);
            mw->setNewFrequency(curr_freq);
        }
    }
}
//--------------------------------------------------------------------------------
void Scan_ToolDock::next(void)
{
    qDebug() << "next";
    if(mw)
    {
        if(curr_freq <= (sb_end_freq->value() * 1000))
        {
            curr_freq += (sb_step_freq->value() * 1000);
            sb_begin_freq->setValue(curr_freq / 1000);
            mw->setNewFrequency(curr_freq);
        }
    }
}
//--------------------------------------------------------------------------------
