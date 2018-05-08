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

    le = new LogBox(this);
    if(!le) return;

    setWidget(le);

    //---
#if 1
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(0);
#endif
    //---
#if 0
    thread = new QThread;

    //worker = new LogThread;
    worker = new LogThread(le);

    connect(thread, SIGNAL(started()),          worker, SLOT(process()));
    connect(worker, SIGNAL(finished()),         thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()),         worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()),         thread, SLOT(deleteLater()));

    //connect(worker, SIGNAL(o_info(QString)),    le, SLOT(infoLog(QString)));
    //connect(worker, SIGNAL(o_debug(QString)),   le, SLOT(debugLog(QString)));
    //connect(worker, SIGNAL(o_error(QString)),   le, SLOT(errorLog(QString)));
    //connect(worker, SIGNAL(o_message(QString)), le, SLOT(messageLog(QString)));

    connect(thread, SIGNAL(finished()),         this, SLOT(thread_is_finished()));

    worker->moveToThread(thread);

    setWidget(le);

    thread->start();
#endif
}
//--------------------------------------------------------------------------------
LogDock::~LogDock()
{

}
//--------------------------------------------------------------------------------
//void LogDock::thread_is_finished(void)
//{

//}
//--------------------------------------------------------------------------------
#include </usr/include/syslog.h>
void LogDock::update(void)
{
    if(queue_info.isEmpty()    == false)    le->infoLog(queue_info.dequeue());
    if(queue_debug.isEmpty()   == false)    le->debugLog(queue_debug.dequeue());
    if(queue_error.isEmpty()   == false)    le->errorLog(queue_error.dequeue());
    if(queue_message.isEmpty() == false)    le->messageLog(queue_message.dequeue());
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LogDock::infoLog(QString text)
{
//    if(worker)
//    {
//        worker->i_info(text);
//    }
    queue_info.enqueue(text);
}
//--------------------------------------------------------------------------------
void LogDock::debugLog(QString text)
{
//    if(worker)
//    {
//        worker->i_debug(text);
//    }
    queue_debug.enqueue(text);
}
//--------------------------------------------------------------------------------
void LogDock::errorLog(QString text)
{
//    if(worker)
//    {
//        worker->i_error(text);
//    }
    queue_error.enqueue(text);
}
//--------------------------------------------------------------------------------
void LogDock::messageLog(QString text)
{
//    if(worker)
//    {
//        worker->i_message(text);
//    }
    queue_message.enqueue(text);
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
