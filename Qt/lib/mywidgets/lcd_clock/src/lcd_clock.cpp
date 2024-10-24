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
#include "lcd_clock.hpp"
//--------------------------------------------------------------------------------
#define P_MESSAGE   "message"
#define P_HOUR      "hour"
#define P_MIN       "min"
#define P_SEC       "sec"
//--------------------------------------------------------------------------------
LCD_clock::LCD_clock(QWidget *parent) :
    QLCDNumber(parent)
{
    init();
}
//--------------------------------------------------------------------------------
LCD_clock::~LCD_clock()
{
    if(timer)
    {
        timer->stop();
        disconnect(timer,  &QTimer::timeout,   this,   &LCD_clock::timeout);
        delete timer;
    }
    save_setting();
    delete settings;
}
//--------------------------------------------------------------------------------
void LCD_clock::init(void)
{
    timer = new QTimer(this);
    connect(timer,  &QTimer::timeout,
            this,   &LCD_clock::timeout);

    setStyleSheet("background: black; color: yellow;");
    setSegmentStyle(QLCDNumber::Flat);
    setDigitCount(8);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,   &LCD_clock::customContextMenuRequested,
            this,   &LCD_clock::popup);

    connect(this,   &LCD_clock::s_show_message,
            this,   &LCD_clock::show_message);

    correct_time();

    QString clock = QString("%1:%2:%3")
            .arg(hour, 2, 10, QChar('0'))
            .arg(min,  2, 10, QChar('0'))
            .arg(sec,  2, 10, QChar('0'));
    display(clock);

    load_setting();

    timer->start(1000);
}
//--------------------------------------------------------------------------------
void LCD_clock::correct_time(void)
{
    QTime time = QTime::currentTime();
    hour = time.hour();
    min  = time.minute();
    sec  = time.second();
}
//--------------------------------------------------------------------------------
void LCD_clock::timeout(void)
{
    sec++;
    if(sec > 59)
    {
        sec = 0;
        min++;
    }
    if(min > 59)
    {
        min = 0;
        hour++;
    }
    else
    {
        correct_time();
    }
    if(hour > 23)
    {
        hour = 0;
    }

    QString clock = QString("%1:%2:%3")
            .arg(hour, 2, 10, QChar('0'))
            .arg(min, 2, 10, QChar('0'))
            .arg(sec, 2, 10, QChar('0'));
    display(clock);

    if(timer_active == false)   return;
    if(t_hour != hour)          return;
    if(t_min  != min)           return;
    if(t_sec  != sec)           return;

    timer_active = false;
    emit s_show_message();
}
//--------------------------------------------------------------------------------
void LCD_clock::show_message(void)
{
    Timer_messagebox *msgbox = new Timer_messagebox(message);
    msgbox->setWindowTitle("Warning");
    msgbox->show();
}
//--------------------------------------------------------------------------------
void LCD_clock::popup(QPoint)
{
    QMenu *popup_menu = new QMenu();

    popup_menu->setStyleSheet("background:white;color:black;");

    QAction *clock_action   = new QAction(tr("options"),   this);

    popup_menu->addAction(clock_action);

    connect(clock_action,   &QAction::triggered,    this,   &LCD_clock::open_option);

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void LCD_clock::open_option(void)
{
    Timer_options *dlg = new Timer_options(this);
    dlg->set_message(message);

    int btn = dlg->exec();
    if(btn == QDialog::Accepted)
    {
        QTime t_time = dlg->get_time();
        t_hour = t_time.hour();
        t_min  = t_time.minute();
        t_sec  = t_time.second();
        message = dlg->get_message();
        timer_active = true;
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void LCD_clock::load_setting(void)
{
#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    settings = new QSettings(ORGNAME, app_name);
#else
    settings = new QSettings(QString("%1%2").arg(app_name).arg(".ini"), QSettings::IniFormat);
#endif

    message = settings->value(P_MESSAGE).toString();
    t_hour = settings->value(P_HOUR).toInt();
    t_min  = settings->value(P_MIN).toInt();
    t_sec  = settings->value(P_SEC).toInt();
}
//--------------------------------------------------------------------------------
void LCD_clock::save_setting(void)
{
    settings->setValue(P_MESSAGE,   QVariant::fromValue(message));
    settings->setValue(P_HOUR,      QVariant::fromValue(t_hour));
    settings->setValue(P_MIN,       QVariant::fromValue(t_min));
    settings->setValue(P_SEC,       QVariant::fromValue(t_sec));
}
//--------------------------------------------------------------------------------
