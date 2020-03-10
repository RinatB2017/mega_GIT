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
#include "mytimer.hpp"
#include "ui_mytimer.h"
//--------------------------------------------------------------------------------
MyTimer::MyTimer(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MyTimer)
{
    init();
}
//--------------------------------------------------------------------------------
MyTimer::~MyTimer()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MyTimer::init(void)
{
    ui->setupUi(this);

    create_timer();

    last_time.setHMS(0, 0, 0);

    setMinimumHeight(48);
//    setMinimumHeight(48);

    QTimer::singleShot(0, [this]{
        update_timer();
    });
}
//--------------------------------------------------------------------------------
void MyTimer::create_timer(void)
{
    timer = new QTimer(this);
    connect(timer,  &QTimer::timeout,   this,   &MyTimer::update);
}
//--------------------------------------------------------------------------------
void MyTimer::update(void)
{
    emit trace(Q_FUNC_INFO);

    //int sec = last_time.hour()*60*60 + last_time.minute()*60 + last_time.second();
    int sec =  QTime(0, 0, 0).secsTo(last_time);
    if(sec > 0)
    {
        sec--;
        QTime nt(0, 0, 0);
        last_time = nt.addSecs(sec);
        update_timer();
    }
    else
    {
        timer->stop();
        emit elapsed();
    }
}
//--------------------------------------------------------------------------------
void MyTimer::update_timer(void)
{
    emit trace(Q_FUNC_INFO);

    ui->lcd_display->display(last_time.toString("hh:mm:ss"));
}
//--------------------------------------------------------------------------------
void MyTimer::set_timer(QTime new_time)
{
    emit trace(Q_FUNC_INFO);

    last_time = new_time;
    update_timer();
}
//--------------------------------------------------------------------------------
void MyTimer::start(void)
{
    emit trace(Q_FUNC_INFO);
    if(timer)
    {
        if(timer->isActive() == false)
        {
            timer->start(1000);
        }
    }
}
//--------------------------------------------------------------------------------
void MyTimer::stop(void)
{
    emit trace(Q_FUNC_INFO);
    if(timer)
    {
        if(timer->isActive() == true)
        {
            timer->stop();
        }
    }
}
//--------------------------------------------------------------------------------
void MyTimer::restart(void)
{
    emit trace(Q_FUNC_INFO);

}
//--------------------------------------------------------------------------------
void MyTimer::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MyTimer::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MyTimer::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MyTimer::save_setting(void)
{

}
//--------------------------------------------------------------------------------
