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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include </usr/include/syslog.h>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
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

    table = new QTableView(this);
    Q_CHECK_PTR(table);

    model = new QStandardItemModel(0, 3, this);
    model->setHeaderData(0, Qt::Horizontal, tr("syslog"));
    model->setHorizontalHeaderLabels(QStringList() << "datetime"
                                     << "level"
                                     << "src"
                                     << "message");
    //model->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->setModel(model);

    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //---
    QWidget *w = new QWidget;

    btn_first = new QToolButton;
    btn_prev  = new QToolButton;
    btn_next  = new QToolButton;
    btn_last  = new QToolButton;

    btn_test = new QPushButton;
    btn_test->setText("test");

    for(int n=0; n<8; n++)
    {
        QToolButton *btn = new QToolButton;
        btn->setObjectName(QString("btn_%1").arg(n));
        btn->setText(QString("%1").arg(n));
        btn->setProperty("value", n);
        connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(click()));
        buttons.append(btn);
    }

    btn_first->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipBackward));
    btn_prev->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSeekBackward));
    btn_next->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSeekForward));
    btn_last->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(btn_first,  SIGNAL(clicked(bool)),  this,   SLOT(seek_first()));
    connect(btn_prev,   SIGNAL(clicked(bool)),  this,   SLOT(seek_prev()));
    connect(btn_next,   SIGNAL(clicked(bool)),  this,   SLOT(seek_next()));
    connect(btn_last,   SIGNAL(clicked(bool)),  this,   SLOT(seek_last()));

    connect(btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));

    if(topLevelWidget())
    {
        connect(topLevelWidget(),   SIGNAL(info(QString)),  this,   SLOT(syslog_info(QString)));
        connect(topLevelWidget(),   SIGNAL(debug(QString)), this,   SLOT(syslog_debug(QString)));
        connect(topLevelWidget(),   SIGNAL(error(QString)), this,   SLOT(syslog_error(QString)));
        connect(topLevelWidget(),   SIGNAL(trace(QString)), this,   SLOT(syslog_trace(QString)));
    }

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setSpacing(0);
    hbox->setMargin(0);

    hbox->addWidget(btn_test);
    hbox->addStretch(1);
    foreach (QToolButton *btn, buttons)
    {
        hbox->addWidget(btn);
#ifndef QT_DEBUG
        btn->setEnabled(false);
#endif
    }
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
#ifndef QT_DEBUG
    btn_first->setEnabled(false);
    btn_prev->setEnabled(false);
    btn_next->setEnabled(false);
    btn_last->setEnabled(false);
    btn_test->setEnabled(false);
#endif
    //---
    setWidget(w);
}
//--------------------------------------------------------------------------------
void SysLog_dock::syslog_info(const QString &text)
{
    syslog(QDateTime::currentDateTime(), LOG_INFO, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog_dock::syslog_debug(const QString &text)
{
    syslog(QDateTime::currentDateTime(), LOG_DEBUG, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog_dock::syslog_error(const QString &text)
{
    syslog(QDateTime::currentDateTime(), LOG_ERR, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog_dock::syslog_trace(const QString &text)
{
    syslog(QDateTime::currentDateTime(), LOG_NOTICE, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog_dock::syslog(int level,
                         int src,
                         QString message)
{
    syslog_t log;
    log.dtime = QDateTime::currentDateTime();
    log.level = level;
    log.src = src;
    log.message = message;

    l_syslog.append(log);

    int index = model->rowCount();
    model->insertRow(index);
    model->setData(model->index(index, 0, QModelIndex()), QDateTime::currentDateTime().toString("dd-MM-yy hh:mm:ss"));
    model->setData(model->index(index, 1, QModelIndex()), syslog_to_str(level));
    model->setData(model->index(index, 2, QModelIndex()), src);
    model->setData(model->index(index, 3, QModelIndex()), message);

    table->scrollToBottom();
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

    int index = model->rowCount();
    model->insertRow(index);
    model->setData(model->index(index, 0, QModelIndex()), dtime.toString("dd-MM-yy hh:mm:ss"));
    model->setData(model->index(index, 1, QModelIndex()), syslog_to_str(level));
    model->setData(model->index(index, 2, QModelIndex()), src);
    model->setData(model->index(index, 3, QModelIndex()), message);

    table->scrollToBottom();
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_first(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_prev(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_next(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void SysLog_dock::seek_last(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
QString SysLog_dock::syslog_to_str(int level)
{
    QString temp;
    switch(level)
    {
    case LOG_EMERG:     temp = "LOG_EMERG  ";  break;
    case LOG_ALERT:     temp = "LOG_ALERT  ";  break;
    case LOG_CRIT:      temp = "LOG_CRIT   ";  break;
    case LOG_ERR:       temp = "LOG_ERR    ";  break;
    case LOG_WARNING:   temp = "LOG_WARNING";  break;
    case LOG_NOTICE:    temp = "LOG_NOTICE ";  break;
    case LOG_INFO:      temp = "LOG_INFO   ";  break;
    case LOG_DEBUG:     temp = "LOG_DEBUG  ";  break;
    default:
        temp = QString("%1").arg(level);
        break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
void SysLog_dock::add_test_data(int level)
{
    emit trace(Q_FUNC_INFO);
    QDateTime dt;

    int index = model->rowCount();
    model->insertRow(index);
    model->setData(model->index(index, 0, QModelIndex()), dt.currentDateTime().toString("dd-MM-yy hh:mm:ss"));
    model->setData(model->index(index, 1, QModelIndex()), syslog_to_str(level));
    model->setData(model->index(index, 2, QModelIndex()), 0);
    model->setData(model->index(index, 3, QModelIndex()), "no message");
}
//--------------------------------------------------------------------------------
void SysLog_dock::click(void)
{
    emit trace(Q_FUNC_INFO);

    QToolButton *btn = dynamic_cast<QToolButton *>(sender());
    Q_CHECK_PTR(btn);

    emit info(QString("%1").arg(btn->property("value").toInt()));
}
//--------------------------------------------------------------------------------
void SysLog_dock::test(void)
{
    emit trace(Q_FUNC_INFO);

    for(int n=0; n<8; n++)
    {
        add_test_data(n);
        add_test_data(n);
        add_test_data(n);
    }
}
//--------------------------------------------------------------------------------
