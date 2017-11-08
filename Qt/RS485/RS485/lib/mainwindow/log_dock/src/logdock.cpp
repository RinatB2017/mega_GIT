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
#include <QDebug>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "logdock.hpp"
//#include "logdock_options.hpp"
//--------------------------------------------------------------------------------
#include "logbox.hpp"
//--------------------------------------------------------------------------------
LogDock::LogDock(const QString &title,
                 QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    le = new LogBox("RS232", this);
    if(!le) return;

    connect(this,   SIGNAL(signal_is_shows_info(bool)),     le, SLOT(set_flag_is_shows_info(bool)));
    connect(this,   SIGNAL(signal_is_shows_debug(bool)),    le, SLOT(set_flag_is_shows_debug(bool)));
    connect(this,   SIGNAL(signal_is_shows_error(bool)),    le, SLOT(set_flag_is_shows_error(bool)));
    connect(this,   SIGNAL(signal_is_shows_trace(bool)),    le, SLOT(set_flag_is_shows_trace(bool)));

    setWidget(le);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);   //TODO не надо ставить меньше
}
//--------------------------------------------------------------------------------
LogDock::~LogDock()
{

}
//--------------------------------------------------------------------------------
void LogDock::update(void)
{
    if(is_busy) return;
    is_busy = true;

    if(queue_log.isEmpty() ==  false)
    {
        LOG log = queue_log.dequeue();
        switch(log.level)
        {
        case L_INFO:    le->infoLog(log.text);    break;
        case L_DEBUG:   le->debugLog(log.text);   break;
        case L_ERROR:   le->errorLog(log.text);   break;
        case L_TRACE:   le->traceLog(log.text);   break;
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
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::debugLog(QString text)
{
    LOG log;
    log.level = L_DEBUG;
    log.text = text;
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::errorLog(QString text)
{
    LOG log;
    log.level = L_ERROR;
    log.text = text;
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::traceLog(QString text)
{
    LOG log;
    log.level = L_TRACE;
    log.text = text;
    queue_log.enqueue(log);
}
//--------------------------------------------------------------------------------
void LogDock::syslog(int level,
                     const QString &src,
                     const QString &message)
{
    le->syslogLog(level, src, message);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LogDock::load_settings(void)
{
    Q_CHECK_PTR(le);
    le->load_settings();
}
//--------------------------------------------------------------------------------
void LogDock::save_settings(void)
{
    Q_CHECK_PTR(le);
    le->save_settings();
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
