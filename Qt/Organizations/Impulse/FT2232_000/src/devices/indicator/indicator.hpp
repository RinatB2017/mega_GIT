/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef INDICATOR_HPP
#define INDICATOR_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
//--------------------------------------------------------------------------------
class Indicator_thread;
//--------------------------------------------------------------------------------
class Indicator : public QWidget
{
    Q_OBJECT
public:
    explicit Indicator(I2C_Freq freq, QWidget *parent = 0);
    void run(void);
    void stop(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void finished(void);

private slots:
    void log(const QString &data);
    void measuring_finished(void);

private:
    Indicator_thread *thread;
    I2C_Freq freq;

    void connect_log(void);
    void init(void);

};
//--------------------------------------------------------------------------------
#endif // INDICATOR_HPP
