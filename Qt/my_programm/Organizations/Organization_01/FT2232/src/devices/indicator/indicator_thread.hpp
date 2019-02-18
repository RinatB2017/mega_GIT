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
#ifndef INDICATOR_THREAD_HPP
#define INDICATOR_THREAD_HPP
//--------------------------------------------------------------------------------
#include <QThread>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
//--------------------------------------------------------------------------------
class FT2232H;
//--------------------------------------------------------------------------------
class Indicator_thread : public QThread
{
    Q_OBJECT
    
public:
    Indicator_thread(I2C_Freq freq, QWidget *parent = nullptr);
    ~Indicator_thread();
    void run(void);
    void stop(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void log(const QString &data);

private:
    QObject *parent;
    bool is_worked;
    FT2232H *ft2232h;

    void connect_log(void);
    void init(void);

    bool read(void);
};
//--------------------------------------------------------------------------------
#endif // INDICATOR_THREAD_HPP
