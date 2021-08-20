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
#ifdef Q_OS_LINUX
#   include </usr/include/syslog.h>
#endif
//--------------------------------------------------------------------------------
#include "ui_syslog.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "syslog.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
SysLog::SysLog(const QString &title,
               QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysLog)
{
    ui->setupUi(this);

    setWindowTitle(title);
    setObjectName(title);

    model = new QStandardItemModel(0, 3, this);
    model->setHeaderData(0, Qt::Horizontal, tr("syslog"));
    model->setHorizontalHeaderLabels(QStringList()
                                     << "datetime"
                                     << "level"
                                     << "src"
                                     << "message");
    //model->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table->setModel(model);

    ui->table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    ui->table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    if(topLevelWidget())
    {
        connect(topLevelWidget(),   SIGNAL(info(QString)),  this,   SLOT(syslog_info(QString)));
        connect(topLevelWidget(),   SIGNAL(debug(QString)), this,   SLOT(syslog_debug(QString)));
        connect(topLevelWidget(),   SIGNAL(error(QString)), this,   SLOT(syslog_error(QString)));
        connect(topLevelWidget(),   SIGNAL(trace(QString)), this,   SLOT(syslog_trace(QString)));
    }
}
//--------------------------------------------------------------------------------
SysLog::~SysLog()
{

}
//--------------------------------------------------------------------------------
void SysLog::syslog_info(const QString &text)
{
    s_syslog(QDateTime::currentDateTime(), LOG_INFO, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog::syslog_debug(const QString &text)
{
    s_syslog(QDateTime::currentDateTime(), LOG_DEBUG, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog::syslog_error(const QString &text)
{
    s_syslog(QDateTime::currentDateTime(), LOG_ERR, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog::syslog_trace(const QString &text)
{
    s_syslog(QDateTime::currentDateTime(), LOG_NOTICE, 0, text);
}
//--------------------------------------------------------------------------------
void SysLog::s_syslog(int level,
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

    ui->table->scrollToBottom();
}
//--------------------------------------------------------------------------------
void SysLog::s_syslog(QDateTime dtime,
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

    ui->table->scrollToBottom();
}
//--------------------------------------------------------------------------------
void SysLog::seek_first(void)
{
    //    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void SysLog::seek_prev(void)
{
    //    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void SysLog::seek_next(void)
{
    //    emit trace(Q_FUNC_INFO);
    ui->table->setCurrentIndex(QModelIndex());
}
//--------------------------------------------------------------------------------
void SysLog::seek_last(void)
{
    //    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
QString SysLog::syslog_to_str(int level)
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
        temp = QString("level %1").arg(level);
        break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
void SysLog::add_test_data(int level)
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
void SysLog::click(void)
{
    emit trace(Q_FUNC_INFO);

    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    Q_ASSERT(btn);

    emit info(QString("%1").arg(btn->property(P_SYSLOG_VALUE).toInt()));
}
//--------------------------------------------------------------------------------
void SysLog::test(void)
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
