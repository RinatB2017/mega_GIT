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
#include <QDockWidget>
#include <QTimer>
#include <QQueue>
//--------------------------------------------------------------------------------
class LogBox;
//--------------------------------------------------------------------------------
class LogDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit LogDock(QWidget *parent = nullptr);
    virtual ~LogDock();

public slots:
    void infoLog(QString text);
    void debugLog(QString text);
    void errorLog(QString text);
    void traceLog(QString text);

private slots:
    void update(void);

private:
    LogBox *le;
    QTimer *timer;

    bool is_busy = false;

    enum {
        L_INFO = 0,
        L_DEBUG,
        L_ERROR,
        L_TRACE
    };

    struct LOG
    {
        int level;
        QString text;
    };
    QQueue<LOG> queue_log;

};
//--------------------------------------------------------------------------------
#endif // LOGDOCK_HPP
