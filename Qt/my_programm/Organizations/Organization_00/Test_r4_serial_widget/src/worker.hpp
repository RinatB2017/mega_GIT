/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef WORKER_HPP
#define WORKER_HPP
//--------------------------------------------------------------------------------
#include <QString>
#include <QObject>
//--------------------------------------------------------------------------------
#include "r4_serial.h"
//--------------------------------------------------------------------------------
class Worker : public QObject
{
    Q_OBJECT

public:
    Worker();
    ~Worker();

public slots:
    void input(const QString &data);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void finished(void);

    void output(const QString &);

private slots:
    void write_data(void);
    void read_data(void);

private:
    int descriptor;
    R4Serial *serial;
    R4SerialSettings s;

    ssize_t len;
    char buf[15000];

    void init_timer(void);

    void init_device(void);
    void info_device(void);

};
//--------------------------------------------------------------------------------
#endif
