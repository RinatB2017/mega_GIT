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
//--------------------------------------------------------------------------------
//enum SYSLOG
//{
//    LOG_EMERG   = 0,       /* system is unusable */
//    LOG_ALERT   = 1,       /* action must be taken immediately */
//    LOG_CRIT    = 2,       /* critical conditions */
//    LOG_ERR     = 3,       /* error conditions */
//    LOG_WARNING = 4,       /* warning conditions */
//    LOG_NOTICE  = 5,       /* normal but significant condition */
//    LOG_INFO    = 6,       /* informational */
//    LOG_DEBUG   = 7        /* debug-level messages */
//};
// чем выше уровень сообщения, тем оно менее информативно
//--------------------------------------------------------------------------------
class LogBox;
//--------------------------------------------------------------------------------
class LogDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit LogDock(const QString &title,
                     QWidget *parent = nullptr);

    void load_settings(void);
    void save_settings(void);

    void load_html(void);
    void save_html(void);

    void  set_font(QFont font);
    QFont get_font(void);

public slots:
    void infoLog(const QString &text);
    void debugLog(const QString &text);
    void errorLog(const QString &text);
    void messageLog(const QString &text);

private:
    LogBox *le;
};
//--------------------------------------------------------------------------------
#endif // LOGDOCK_HPP
