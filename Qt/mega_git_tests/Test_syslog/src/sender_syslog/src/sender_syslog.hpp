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
#ifndef SENDER_SYSLOG_HPP
#define SENDER_SYSLOG_HPP
//--------------------------------------------------------------------------------
#include <QPushButton>
#include <QLineEdit>
#include <QDateTime>
#include <QComboBox>
#include <QToolBar>
#include <QSpinBox>
#include <QLabel>
#include <QTimer>
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include </usr/include/syslog.h>
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
enum SYSLOG_LEVELS
{
    LOG_EMERG   = 0,       /* system is unusable */
    LOG_ALERT   = 1,       /* action must be taken immediately */
    LOG_CRIT    = 2,       /* critical conditions */
    LOG_ERR     = 3,       /* error conditions */
    LOG_WARNING = 4,       /* warning conditions */
    LOG_NOTICE  = 5,       /* normal but significant condition */
    LOG_INFO    = 6,       /* informational */
    LOG_DEBUG   = 7        /* debug-level messages */
};
#endif
//--------------------------------------------------------------------------------
class Sender_syslog : public QToolBar
{
    Q_OBJECT

public:
    Sender_syslog(const QString &title,
                  int default_src,
                  int default_level,
                  QWidget *parent = Q_NULLPTR);
    virtual ~Sender_syslog();

signals:
    void syslog(QDateTime,
                int,
                int,
                QString);

private slots:
    void start(void);
    void stop(void);
    void update(void);

private:
    int def_src = 0;
    int def_level = 0;

    QComboBox *cb_level = nullptr;

    QSpinBox *sb_src = nullptr;

    QSpinBox *sb_interval = nullptr;

    QLineEdit *le_message = nullptr;

    QPushButton *btn_start = nullptr;
    QPushButton *btn_stop = nullptr;

    QTimer *timer = nullptr;

    void init(void);
    void init_timer(void);
};
//--------------------------------------------------------------------------------
#endif // SENDER_SYSLOG_HPP
