/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef DIGITAL_CLOCK_HPP
#define DIGITAL_CLOCK_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include<QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "timer_messagebox.hpp"
#include "timer_options.hpp"
#include "mymessages.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
class Digital_clock : public QLabel
{
    Q_OBJECT

signals:
    void s_show_message(void);

public:
    explicit Digital_clock(QWidget *parent = nullptr);
    virtual ~Digital_clock();

private:
    QTimer *timer = nullptr;

    int hour = 0;
    int min  = 0;
    int sec  = 0;

    bool timer_active = false;
    QString message;
    int t_hour = 0;
    int t_min  = 0;
    int t_sec  = 0;

    QSettings *settings = nullptr;

    void init(void);
    void correct_time(void);
    void timeout(void);
    void show_message(void);

    void popup(QPoint);
    void open_option(void);

    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // DIGITAL_CLOCK_HPP
