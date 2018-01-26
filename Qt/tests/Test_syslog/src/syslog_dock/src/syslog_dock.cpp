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
#include <QToolButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTime>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "syslog_dock.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
SysLog_dock::SysLog_dock(const QString &title,
                         QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    table = new QTableWidget(this);
    if(!table) return;

    table->setColumnCount(4);
    table->setRowCount(10);
    table->setHorizontalHeaderLabels(QStringList() << "datetime"
                                     << "level"
                                     << "src"
                                     << "message");

    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    //---
    QWidget *w = new QWidget;

    btn_first = new QToolButton;
    btn_prev  = new QToolButton;
    btn_next  = new QToolButton;
    btn_last  = new QToolButton;

    btn_test = new QPushButton;
    btn_test->setText("test");

    btn_first->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipBackward));
    btn_prev->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSeekBackward));
    btn_next->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSeekForward));
    btn_last->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(btn_first,  SIGNAL(clicked(bool)),  this,   SLOT(seek_first()));
    connect(btn_prev,   SIGNAL(clicked(bool)),  this,   SLOT(seek_prev()));
    connect(btn_next,   SIGNAL(clicked(bool)),  this,   SLOT(seek_next()));
    connect(btn_last,   SIGNAL(clicked(bool)),  this,   SLOT(seek_last()));

    connect(btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setSpacing(0);
    hbox->setMargin(0);

    hbox->addWidget(btn_test);
    hbox->addStretch(1);
    hbox->addWidget(btn_first);
    hbox->addWidget(btn_prev);
    hbox->addWidget(btn_next);
    hbox->addWidget(btn_last);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(table);
    vbox->addLayout(hbox);

    w->setLayout(vbox);
    //---

    setWidget(w);
}
//--------------------------------------------------------------------------------
void SysLog_dock::syslog(QDateTime dtime,
                         int level,
                         int src,
                         QString message)
{
    syslog_t log;
    log.dtime = dtime;
    log.level = level;
    log.src = src;
    log.message = message;

    l_syslog.append(log);

    //qDebug() << "SysLog_dock::syslog";
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_first(void)
{
#ifdef QT_DEBUG
    qDebug() << "first";
#endif
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_prev(void)
{
#ifdef QT_DEBUG
    qDebug() << "prev";
#endif
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_next(void)
{
#ifdef QT_DEBUG
    qDebug() << "next";
#endif
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_last(void)
{
#ifdef QT_DEBUG
    qDebug() << "last";
#endif
}
//--------------------------------------------------------------------------------
void SysLog_dock::test(void)
{
#ifdef QT_DEBUG
    qDebug() << "l_syslog.count" << l_syslog.count();
#endif
}
//--------------------------------------------------------------------------------
