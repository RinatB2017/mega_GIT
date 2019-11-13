/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef SERIALBOX5_THREAD_HPP
#define SERIALBOX5_THREAD_HPP
//--------------------------------------------------------------------------------
#include <QDateTime>
#include <QWidget>
//--------------------------------------------------------------------------------
class SerialBox5_thread : public QObject
{
    Q_OBJECT

public:
    SerialBox5_thread(QObject *parent = nullptr);
    ~SerialBox5_thread();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void finished(void);

    void set_hour(unsigned char hour);
    void set_min(unsigned char min);
    void set_sec(unsigned char sec);

public slots:
    void process(void);

    void start(void);
    void stop(void);

    void set_time(QDateTime dt);

private:
    unsigned char hour = 0;
    unsigned char min  = 0;
    unsigned char sec  = 0;

    bool flag_exit = false;
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX5_THREAD_HPP
