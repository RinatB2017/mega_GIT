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
#ifndef LOGDOCK_HPP
#define LOGDOCK_HPP
//--------------------------------------------------------------------------------
#include <QApplication>
#include <QDockWidget>
//--------------------------------------------------------------------------------
#include <QQueue>
//--------------------------------------------------------------------------------
#include "logbox.hpp"
//--------------------------------------------------------------------------------
class LogThread : public QObject
{
    Q_OBJECT

public:
#if 1
    explicit LogThread(LogBox *log)
    {
        le = log;
    }
#endif

signals:
    void o_info(const QString &);
    void o_debug(const QString &);
    void o_error(const QString &);
    void o_message(const QString &);

    void finished(void);

public slots:
    void i_info(QString text)
    {
        queue_info.enqueue(text);
    }

    void i_debug(QString text)
    {
        queue_debug.enqueue(text);
    }

    void i_error(QString text)
    {
        queue_error.enqueue(text);
    }

    void i_message(QString text)
    {
        queue_message.enqueue(text);
    }

    void process(void)
    {
        while(flag)
        {
#if 1
            if(queue_info.isEmpty()    == false)    le->infoLog(queue_info.dequeue());
            if(queue_debug.isEmpty()   == false)    le->debugLog(queue_debug.dequeue());
            if(queue_error.isEmpty()   == false)    le->errorLog(queue_error.dequeue());
            if(queue_message.isEmpty() == false)    le->messageLog(queue_message.dequeue());
#endif

#if 0
            if(queue_info.isEmpty()    == false)    emit o_info(queue_info.dequeue());
            if(queue_debug.isEmpty()   == false)    emit o_debug(queue_debug.dequeue());
            if(queue_error.isEmpty()   == false)    emit o_error(queue_error.dequeue());
            if(queue_message.isEmpty() == false)    emit o_message(queue_message.dequeue());
#endif

            //QCoreApplication::processEvents();
        }
        emit finished();
    }

    void stop(void)
    {
        flag = false;
    }

private:
    QQueue<QString> queue_info;
    QQueue<QString> queue_debug;
    QQueue<QString> queue_error;
    QQueue<QString> queue_message;

    bool flag = true;
    LogBox *le;
};
//--------------------------------------------------------------------------------
class LogDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit LogDock(const QString &title,
                     QWidget *parent = nullptr);
    ~LogDock();

    void load_settings(void);
    void save_settings(void);

    void load_html(void);
    void save_html(void);

    void  set_font(QFont font);
    QFont get_font(void);

public slots:
    void infoLog(QString text);
    void debugLog(QString text);
    void errorLog(QString text);
    void messageLog(QString text);

    void syslog(int level,
                const QString &src,
                const QString &message);

    //void thread_is_finished(void);

private slots:
    void update(void);

private:
    LogBox *le;
    QTimer *timer;

    QQueue<QString> queue_info;
    QQueue<QString> queue_debug;
    QQueue<QString> queue_error;
    QQueue<QString> queue_message;
};
//--------------------------------------------------------------------------------
#endif // LOGDOCK_HPP
