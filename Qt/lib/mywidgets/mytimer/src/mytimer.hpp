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
#ifndef MYTIMER_HPP
#define MYTIMER_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MyTimer;
}
//--------------------------------------------------------------------------------
class MyTimer : public MyWidget
{
    Q_OBJECT

signals:
    void elapsed(void);

public:
    explicit MyTimer(QWidget *parent = nullptr);
    ~MyTimer();

    void set_timer(QTime new_time);
    void start(void);
    void stop(void);
    void restart(void);

private slots:
    void update(void);

private:
    Ui::MyTimer *ui;
    QTime last_time;

    QTimer *timer = nullptr;

    void init(void);
    void create_timer(void);
    void update_timer(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MYTIMER_HPP
