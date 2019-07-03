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
#define MAX_MP_BUF  3473408
//--------------------------------------------------------------------------------
class Display_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Display_widget(QWidget *parent = nullptr);

    void set_color(int x, int y, QColor color);
    void add_color(QColor color);

public slots:
    void go_first(void);
    void go_last(void);
    void go_prev(void);
    void go_next(void);

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

protected:
    void paintEvent(QPaintEvent *);
};
#endif
