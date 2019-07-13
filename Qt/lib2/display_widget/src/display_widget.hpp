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
#define MIN_SIZE_LED    4
#define MAX_SIZE_LED    32

#define SIZE_X      64
#define SIZE_Y      32

#define MAX_BUF (SIZE_X * SIZE_Y)
//--------------------------------------------------------------------------------
class Display_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit Display_widget(QWidget *parent = nullptr);

    void set_color(int x, int y, QColor color);
    QColor get_color(int x, int y);

    bool set_size_led(int value);

    int get_size_x(void);
    int get_size_y(void);

public slots:
    void clear(void);

private:
    int d_width = 0;
    int d_height = 0;

    int max_x = 0;
    int max_y = 0;

    int led_width = 16;
    int led_height = 16;
    int led_border = 1;

    QColor leds[MAX_BUF];

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
