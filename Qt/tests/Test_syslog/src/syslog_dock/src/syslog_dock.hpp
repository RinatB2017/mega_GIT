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
#ifndef SYSLOG_DOCK_HPP
#define SYSLOG_DOCK_HPP
//--------------------------------------------------------------------------------
#include <QDockWidget>
#include <QDateTime>
//--------------------------------------------------------------------------------
typedef struct SYSLOG
{
    QDateTime dtime;
    int level;
    int src;
    QString message;
} syslog_t;
//--------------------------------------------------------------------------------
class QStandardItemModel;
class QTableView;
class QPushButton;
class QToolButton;
//--------------------------------------------------------------------------------
class SysLog_dock : public QDockWidget
{
    Q_OBJECT

public:
    SysLog_dock(const QString &title,
                QWidget *parent = 0);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void syslog(QDateTime dtime,
                int level,
                int src,
                QString message);

private slots:
    void seek_first(void);
    void seek_prev(void);
    void seek_next(void);
    void seek_last(void);


    void click(void);
    void test(void);

private:
    QList<syslog_t> l_syslog;

    QToolButton *btn_first = 0;
    QToolButton *btn_prev = 0;
    QToolButton *btn_next = 0;
    QToolButton *btn_last = 0;

    QList<QToolButton *> buttons;

    QPushButton *btn_test = 0;

    QStandardItemModel *model = 0;
    QTableView *table = 0;

    QString syslog_to_str(int level);

    void add_test_data(int level);
};
//--------------------------------------------------------------------------------
#endif // SYSLOG_DOCK_HPP
