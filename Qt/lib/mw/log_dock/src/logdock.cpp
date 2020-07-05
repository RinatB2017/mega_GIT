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
#include <QMetaMethod>
#include <QTextEdit>
#include <QThread>
#include <QAction>
#include <QTimer>
#include <QMenu>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "dock_position.hpp"
#include "logdock.hpp"
//--------------------------------------------------------------------------------
#include "logbox.hpp"
//--------------------------------------------------------------------------------
LogDock::LogDock(const QString &title,
                 QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    le = new LogBox("LogBox", this);
    Q_CHECK_PTR(le);

    connect(this,   &LogDock::signal_is_shows_info,     le, &LogBox::set_flag_is_shows_info);
    connect(this,   &LogDock::signal_is_shows_debug,    le, &LogBox::set_flag_is_shows_debug);
    connect(this,   &LogDock::signal_is_shows_error,    le, &LogBox::set_flag_is_shows_error);
    connect(this,   &LogDock::signal_is_shows_trace,    le, &LogBox::set_flag_is_shows_trace);

#if 1
    QWidget *w = new QWidget(this);

    Dock_position *dp = new Dock_position(objectName(), this);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->setSpacing(0);
    hbox->addWidget(le);
    hbox->addWidget(dp);
    w->setLayout(hbox);
    setWidget(w);
#else
    setWidget(le);
#endif

    timer = new QTimer(this);
    connect(timer,  &QTimer::timeout,   this,   &LogDock::update);
    timer->start(10);   //TODO не надо ставить меньше
}
//--------------------------------------------------------------------------------
LogDock::~LogDock()
{
    if(le)
    {
        le->disconnect();
        le->deleteLater();
    }

    if(timer)
    {
        timer->stop();
        timer->disconnect();
        timer->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void LogDock::update(void)
{
    if(is_busy) return;
    is_busy = true;

    if(queue_log.isEmpty() == false)
    {
        LOG log = queue_log.dequeue();
        switch(log.level)
        {
        case L_INFO:    le->infoLog(log.text);    break;
        case L_DEBUG:   le->debugLog(log.text);   break;
        case L_ERROR:   le->errorLog(log.text);   break;
        case L_TRACE:   le->traceLog(log.text);   break;
        case L_COLOR:   le->colorLog(log.text,
                                     log.text_color,
                                     log.background_color);   break;
        }
    }

    is_busy = false;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LogDock::infoLog(QString text)
{
    LOG log;
    log.level = L_INFO;
    log.text = text;
    log.text_color = QColor(Qt::blue);
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::debugLog(QString text)
{
    LOG log;
    log.level = L_DEBUG;
    log.text = text;
    log.text_color = QColor(Qt::darkGreen);
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::errorLog(QString text)
{
    LOG log;
    log.level = L_ERROR;
    log.text = text;
    log.text_color = QColor(Qt::red);
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::traceLog(QString text)
{
    LOG log;
    log.level = L_TRACE;
    log.text = text;
    log.text_color = QColor(Qt::gray);
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::clearLog(void)
{
    queue_log.clear();
    le->clear();
}
//--------------------------------------------------------------------------------
void LogDock::colorLog(const QString &text,
                       const QColor text_color,
                       const QColor background_color)
{
    LOG log;
    log.level = L_COLOR;
    log.text = text;
    log.text_color = text_color;
    log.background_color = background_color;
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void  LogDock::set_font(QFont font)
{
#ifndef NO_LOG
    le->set_font(font);
#else
    Q_UNUSED(font)
#endif
}
//--------------------------------------------------------------------------------
QFont LogDock::get_font(void)
{
#ifndef NO_LOG
    return le->get_font();
#else
    return QFont();
#endif
}
//--------------------------------------------------------------------------------
