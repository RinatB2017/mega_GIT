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
#ifndef DISPLAY_WIDGET
#define DISPLAY_WIDGET
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define MAX_MP_BUF      3473408

#define MIN_SIZE_LED    4
#define MAX_SIZE_LED    32

#define MIN_SIZE_X      800
#define MAX_SIZE_X      800

#define MIN_SIZE_Y      16
#define MAX_SIZE_Y      256
//--------------------------------------------------------------------------------
class Display_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit Display_widget(QWidget *parent = nullptr);

    void set_color(int x, int y, QColor color);
    QColor get_color(int x, int y);

    void add_color(QColor color);
    bool set_size_x(int value);
    bool set_size_y(int value);
    bool set_size_led(int value);

    int get_size_x(void);
    int get_size_y(void);

public slots:
    void go_first(void);
    void go_last(void);
    void go_prev(void);
    void go_next(void);

    void save_image(void);
    void clear(void);

private:
    int d_width = 0;
    int d_height = 0;

    int cnt_x = 0;
    int cnt_y = 0;

    int led_width = 16;
    int led_height = 16;
    int led_border = 1;

    int index_add = 0;
    int show_begin_address = 0;
    int show_last_address = 0;
    QColor leds[MAX_MP_BUF];

    QColor led_color = Qt::black;
    QColor border_color = Qt::gray;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void paintEvent(QPaintEvent *);
};
#endif
