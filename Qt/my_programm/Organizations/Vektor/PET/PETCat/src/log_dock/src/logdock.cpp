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
#include <QSettings>
#include <QTextEdit>
#include <QThread>
#include <QAction>
#include <QTimer>
#include <QDebug>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "pet_defines.hpp"
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

    le = new LogBox(this);
    if(!le)
    {
        return;
    }

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
        case L_INFO:    le->infoLog(log.text);      break;
        case L_DEBUG:   le->debugLog(log.text);     break;
        case L_ERROR:   le->errorLog(log.text);     break;
        case L_TRACE:   le->traceLog(log.text);   break;
        default:
            break;
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
    le->load_settings();
}
//--------------------------------------------------------------------------------
void LogDock::save_settings(void)
{
    le->save_settings();
}
//--------------------------------------------------------------------------------
void LogDock::load_html(void)
{
    le->load_html();
}
//--------------------------------------------------------------------------------
void LogDock::save_html(void)
{
    le->save_html();
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
void LogDock::set_flag_is_shows_info(bool state)
{
    le->set_flag_is_shows_info(state);
}
//--------------------------------------------------------------------------------
void LogDock::set_flag_is_shows_debug(bool state)
{
    le->set_flag_is_shows_debug(state);
}
//--------------------------------------------------------------------------------
void LogDock::set_flag_is_shows_error(bool state)
{
    le->set_flag_is_shows_error(state);
}
//--------------------------------------------------------------------------------
void LogDock::set_flag_is_shows_trace(bool state)
{
    le->set_flag_is_shows_trace(state);
}
//--------------------------------------------------------------------------------
