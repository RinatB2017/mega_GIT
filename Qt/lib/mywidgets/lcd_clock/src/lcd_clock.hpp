/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#ifndef LCD_CLOCK_HPP
#define LCD_CLOCK_HPP
//--------------------------------------------------------------------------------
#include <QLCDNumber>
#include <QSettings>
#include <QTimer>
#include <QLabel>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "timer_messagebox.hpp"
#include "timer_options.hpp"
#include "mymessages.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
class LCD_clock : public QLCDNumber
{
    Q_OBJECT

signals:
    void s_show_message();

public:
    explicit LCD_clock(QWidget *parent = nullptr);
    virtual ~LCD_clock();

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

    void init();
    void correct_time();
    void timeout();
    void show_message();

    void popup(QPoint);
    void open_option();

    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // LCD_CLOCK_HPP
