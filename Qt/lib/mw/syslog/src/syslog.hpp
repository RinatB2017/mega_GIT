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
#ifndef SYSLOG_HPP
#define SYSLOG_HPP
//--------------------------------------------------------------------------------
#include <QStandardItemModel>
#include <QDateTime>
#include <QWidget>
//--------------------------------------------------------------------------------
#define P_SYSLOG_VALUE "value"
//--------------------------------------------------------------------------------
typedef struct SYSLOG
{
    QDateTime dtime;
    int level;
    int src;
    QString message;
} syslog_t;
//--------------------------------------------------------------------------------
namespace Ui {
    class SysLog;
}
//--------------------------------------------------------------------------------
class SysLog : public QWidget
{
    Q_OBJECT

public:
    explicit SysLog(const QString &title,
                    QWidget *parent = nullptr);
    virtual ~SysLog();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void s_syslog(int level,
                  int src,
                  QString message);
    void s_syslog(QDateTime dtime,
                  int level,
                  int src,
                  QString message);

private slots:
    void seek_first(void);
    void seek_prev(void);
    void seek_next(void);
    void seek_last(void);

    void syslog_info(const QString &text);
    void syslog_debug(const QString &text);
    void syslog_error(const QString &text);
    void syslog_trace(const QString &text);

    void click(void);
    void test(void);

private:
    Ui::SysLog *ui;
    QList<syslog_t> l_syslog;

    QStandardItemModel *model = nullptr;

    QString syslog_to_str(int level);
    void add_test_data(int level);
};
//--------------------------------------------------------------------------------
#endif // SYSLOG_HPP
